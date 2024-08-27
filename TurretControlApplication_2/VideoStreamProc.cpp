#include "VideoStreamProc.h"

#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"postproc.lib")
#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"swscale.lib")

VideoStreamProc::VideoStreamProc(MainFrame* parent, unsigned short port, unsigned char VideoCodecH265)
{
    running = true;
    set_port = port;
    codec = static_cast<VSP_CodecH26X_DType>(VideoCodecH265);
    parent_class = parent;

    threadReadSocket = std::thread(&VideoStreamProc::ReciveData_Procedure, this);
    threadDecodeH265 = std::thread(&VideoStreamProc::DecodeH26X_Procedure, this);
}

VideoStreamProc::~VideoStreamProc()
{
    running = false;
    closesocket(RTPSocket);
    
    if (threadReadSocket.joinable())
        threadReadSocket.join();

    cvDecode.notify_one();

    if (threadDecodeH265.joinable())
        threadDecodeH265.join();
}

int VideoStreamProc::ReciveData_Procedure(void)
{
    unsigned short port = 0;
    int SocketOption;
    sockaddr_in RTPSocketAddr;
    int rcvResult = 0;
    static char RecvBuf[2000];
    static int BufLen = 2000;
    RTPData_DType packet;
    
    while (running)
    {       
        port = set_port;

        RTPSocketAddr.sin_family = AF_INET;
        RTPSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        RTPSocketAddr.sin_port = htons(port);

        RTPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (RTPSocket == INVALID_SOCKET)
        {
            continue;
        }

        SocketOption = 1024 * 128;
        if (setsockopt(RTPSocket, SOL_SOCKET, SO_RCVBUF, (char*)&SocketOption, sizeof(SocketOption)) < 0)
        {
            continue;
        }

        SocketOption = true;
        if (setsockopt(RTPSocket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&SocketOption, sizeof(SocketOption)) < 0)
        {
            continue;
        }

        if (bind(RTPSocket, (sockaddr*)&RTPSocketAddr, sizeof(RTPSocketAddr)) == SOCKET_ERROR)
        {
            closesocket(RTPSocket);
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        while (port == set_port && running)
        {
            rcvResult = recvfrom(RTPSocket, RecvBuf, BufLen, 0, NULL, NULL);
            if (rcvResult == SOCKET_ERROR)
                break;

            packet.data = new unsigned char[rcvResult]();
            std::memcpy(packet.data, &RecvBuf, rcvResult);
            packet.size = rcvResult;

            MutexSocketData.lock();
            qPacket.push(packet);
            MutexSocketData.unlock();

            cvDecode.notify_one();
        }

        closesocket(RTPSocket);
    }

    return 0;
}

int VideoStreamProc::DecodeH26X_Procedure(void)
{
    RTPData_DType packet;
    
    if(codec == VSP_CODEC_H264)
        pAVCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    else
        pAVCodec = avcodec_find_decoder(AV_CODEC_ID_HEVC);

    if (!pAVCodec) {
        std::cerr << "Ошибка поиска декодера H.265!" << std::endl;
        return 1;
    }

    pAVCodecContext = avcodec_alloc_context3(pAVCodec);
    if (!pAVCodecContext) {
        std::cerr << "Ошибка выделения памяти для контекста декодера!" << std::endl;
        return 1;
    }

    // Настройка контекста декодера
    av_dict_set(&dict, "tune", "zerolatency", 0);
    av_dict_set(&dict, "preset", "ultrafast", 0);
    av_dict_set(&dict, "fflags", "nobuffer", 0);

    if (avcodec_open2(pAVCodecContext, pAVCodec, &dict) < 0) {
        std::cerr << "Ошибка открытия контекста декодера!" << std::endl;
        return 1;
    }

    pAVPacket = av_packet_alloc();
    if (!pAVPacket)
    {
        std::cerr << "Ошибка инициализации пакета!" << std::endl;
        return 1;
    }

    pAVFrame = av_frame_alloc();
    if (!pAVFrame)
    {
        std::cerr << "Ошибка инициализации frame!" << std::endl;
        return 1;
    }

    pAVFrameRGB = av_frame_alloc();
    if (!pAVFrameRGB)
    {
        std::cerr << "Ошибка инициализации frameRGB!" << std::endl;
        return 1;
    }

    pAVCodecParserContext = av_parser_init(pAVCodec->id);
    if (!pAVCodecParserContext)
    {
        std::cerr << "Ошибка инициализации Parser" << std::endl;
        return 1;
    }
    //pAVCodecParserContext->flags = PARSER_FLAG_COMPLETE_FRAMES;

    while (running)
    {
        std::unique_lock<std::mutex> ul(MutexDecode);
        cvDecode.wait(ul, [&] { return !qPacket.empty() || !running; });

        while (1)
        {
            MutexSocketData.lock();
            if (qPacket.empty())
            {
                MutexSocketData.unlock();
                break;
            }
            packet = qPacket.front();
            qPacket.pop();
            MutexSocketData.unlock();
            
            process_rtp_packet(packet.data, packet.size);                        
            decode_H26X_process();

            delete[] packet.data;
        }
    }

    av_dict_free(&dict);
    avformat_free_context(pAVFormatContext);
    av_frame_free(&pAVFrame);
    av_frame_free(&pAVFrameRGB);
    avcodec_close(pAVCodecContext);
    av_packet_free(&pAVPacket);

    return 0;
}

// Функция для обработки RTP пакета и отправки в декодер libav
int VideoStreamProc::process_rtp_packet(unsigned char* data, size_t data_size) 
{
    RTP_Header RTPHeader = parse_rtp_header(data);
    
    // Поиск повторных пакетов
    if (std::find(WatchRepet.begin(), WatchRepet.end(), RTPHeader.sequence_number) != WatchRepet.end())
        return 0;
    WatchRepet.push_front(RTPHeader.sequence_number);
    while (WatchRepet.size() > 15)
        WatchRepet.pop_back();

    // Отслеживание источника
    if (clock() > timeSourceID)
        SourceID = RTPHeader.SSRC;
    else if (SourceID != RTPHeader.SSRC)
        return 0;
    timeSourceID = clock() + 2000;

    //Скипаем RTPHeader
    data = data + 12;
    data_size = data_size - 12;    

    if (RTPHeader.X)
    {
        //profile extension
        short define;
        short length;
        length = *(data + 3);//suppose not so long extension
        data += 4;
        data_size -= 4;
        data += (length * 4);
        data_size -= (length * 4);
    }

    // Извлечение NALUs из данных пакета
    if (codec == CODEC_H264)
        extract_nalus_H264(data, data_size);
    else
        extract_nalus_H265(data, data_size);

    return 0;
}

// Функция для извлечения NALUs из RTP пакета
void VideoStreamProc::extract_nalus_H265(unsigned char* inData, size_t data_size)
{
    RTPData_DType NALU;
    unsigned char* temp = nullptr;
    unsigned char PayloadHdr;
    unsigned char FUType;
    unsigned char fStart;
    unsigned char fStop;
    unsigned long size;
    const unsigned char* data;
    data = inData;

    PayloadHdr = (unsigned char)((*data >> 1) & 0x3f); //Get NALU TYPE

    if (PayloadHdr == 49) //fragment packet mode
    {
        //PayloadHdr 
        data = data + 2; //skip PayloadHdr 
        data_size -= 2;
        //FU header

        FUType = *data & 0x3F;
        fStart = *data & 0x80;
        fStop = *data & 0x40;
        
        unsigned char nalu_header0 = (inData[0] & 0x81) | (FUType << 1);
        unsigned char nalu_header1 = inData[1];

        //FU header
        data = data + 1; //skip FU header
        data_size -= 1;
        //FU Payload

        if (fStart != 0) //The first fragment
        {
            if (NALU_FU.data)
            {
                delete[] NALU_FU.data;
                NALU_FU.data = nullptr;
            }

            NALU_FU.size = data_size + 6;
            NALU_FU.data = new unsigned char[NALU_FU.size]();
            std::memcpy(NALU_FU.data, &inteval, 4);
            std::memcpy(NALU_FU.data + 4, &nalu_header0, 1);
            std::memcpy(NALU_FU.data + 5, &nalu_header1, 1);
            std::memcpy(NALU_FU.data + 6, data, data_size);
            NALU_FU.size = data_size + 6;
        }
        else //Intermediate fragment
        {
            temp = new unsigned char[NALU_FU.size + data_size]();
            std::memcpy(temp, NALU_FU.data, NALU_FU.size);
            std::memcpy(temp + NALU_FU.size, data, data_size);
            delete[] NALU_FU.data;
            NALU_FU.data = temp;
            NALU_FU.size += data_size;
        }

        if (fStop != 0) //The last fragment
        {
            NALU.data = NALU_FU.data;
            NALU.size = NALU_FU.size;
            
            qNalu.push(NALU);

            NALU_FU.data = nullptr;
            NALU_FU.size = 0;
        }
    }
    else if (PayloadHdr == 48) //AP packet
    {
        //PayloadHdr 
        data = data + 2; //skip PayloadHdr
        data_size -= 2;
        //NALU size

        while (data_size > 2)
        {
            size = ((*data << 8) | *(data + 1));
            
            data = data + 2; //skip NALU size
            data_size -= 2;
            //NALU hdr + Payload

            NALU.size = size + 4;
            NALU.data = new unsigned char[NALU.size]();
            
            std::memcpy(NALU.data, &inteval, 4);
            std::memcpy(NALU.data + 4, data, size);

            qNalu.push(NALU);

            data = data + size;
            data_size -= size;
        }
    }
    else if (PayloadHdr == 1 || PayloadHdr == 19 || PayloadHdr == 32 || PayloadHdr == 33 ||
        PayloadHdr == 34 || PayloadHdr == 39)
    {
        NALU.size = data_size + 4;
        NALU.data = new unsigned char[NALU.size]();

        std::memcpy(NALU.data, &inteval, 4);
        std::memcpy(NALU.data + 4, data, NALU.size);
        
        qNalu.push(NALU);
    }
}

// Функция для извлечения NALUs из RTP пакета
void VideoStreamProc::extract_nalus_H264(unsigned char* inData, size_t data_size)
{
    RTPData_DType NALU;
    unsigned char* temp = nullptr;
    unsigned char NALU_Type;
    unsigned char FUType;
    unsigned char fStart;
    unsigned char fStop;
    unsigned long size;
    const unsigned char* data;
    data = inData;

    NALU_Type = (unsigned char)(*data & 0x1f); //Get NALU TYPE

    if (NALU_Type == 28) //FU-A
    {
        //FU indicator 
        data = data + 1; //skip FU indicator
        data_size -= 1;
        //FU header

        FUType = *data & 0x1F;
        fStart = *data & 0x80;
        fStop = *data & 0x40;

        unsigned char nalu_header0 = (inData[0] & 0xe0) | (FUType & 0x1f);

        //FU header
        data = data + 1; //skip FU header
        data_size -= 1;
        //FU Payload

        if (fStart != 0) //The first fragment
        {
            if (NALU_FU.data)
            {
                delete[] NALU_FU.data;
                NALU_FU.data = nullptr;
            }

            NALU_FU.size = data_size + 5;
            NALU_FU.data = new unsigned char[NALU_FU.size]();
            std::memcpy(NALU_FU.data, &inteval, 4);
            std::memcpy(NALU_FU.data + 4, &nalu_header0, 1);
            std::memcpy(NALU_FU.data + 5, data, data_size);
        }
        else //Intermediate fragment
        {
            temp = new unsigned char[NALU_FU.size + data_size]();
            std::memcpy(temp, NALU_FU.data, NALU_FU.size);
            std::memcpy(temp + NALU_FU.size, data, data_size);
            delete[] NALU_FU.data;
            NALU_FU.data = temp;
            NALU_FU.size += data_size;
        }

        if (fStop != 0) //The last fragment
        {
            NALU.data = NALU_FU.data;
            NALU.size = NALU_FU.size;

            qNalu.push(NALU);

            NALU_FU.data = nullptr;
            NALU_FU.size = 0;
        }
    }
    else if (NALU_Type == 24) //STAP-A
    {
        //STAP-A NAL HDR 
        data = data + 1; //skip NAL HDR
        data_size -= 1;
        //NALU size

        while (data_size > 0)
        {
            size = ((*data << 8) | *(data + 1));

            data = data + 2; //skip NALU size
            data_size -= 2;
            //NALU hdr + Payload

            NALU.size = size + 4;
            NALU.data = new unsigned char[NALU.size]();

            std::memcpy(NALU.data, &inteval, 4);
            std::memcpy(NALU.data + 4, data, size);

            qNalu.push(NALU);

            data = data + size;
            data_size -= size;
        }
    }
    else
    {
        data = data + 1; //NALU payload
        data_size -= 1;

        NALU.size = data_size + 4;
        NALU.data = new unsigned char[NALU.size + 1]();

        std::memcpy(NALU.data, &inteval, 4);
        std::memcpy(NALU.data + 4, data-1, data_size);

        qNalu.push(NALU);
    }
}

// Функция Декодирования
void VideoStreamProc::decode_H26X_process(void)
{
    RTPData_DType NALU;
    int size = 0;
    unsigned char* data = nullptr;
    int len = 0;
    int ret;
    int width, height;

    if (qNalu.size() > 10)
    {
        while (!qNalu.empty())
            qNalu.pop();
    }

    while (!qNalu.empty())
    {
        NALU = qNalu.front();
        qNalu.pop();
        
        data = NALU.data;
        size = NALU.size;
        
        while (size && running)
        {
            len = av_parser_parse2(pAVCodecParserContext, pAVCodecContext, &pAVPacket->data, &pAVPacket->size,
                (unsigned char*)data, size,
                AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

            data = len ? data + len : data;
            size -= len;

            if (pAVPacket->size)
            {
                ret = avcodec_send_packet(pAVCodecContext, pAVPacket);
                if (ret < 0)
                {
                    //std::memcpy(test_data, pAVPacket->data, std::max(pAVPacket->size, (int)sizeof(test_data)));
                    if(ret == AVERROR(ENOMEM))
                        std::cerr << "ENOMEM" << std::endl;
                    if (ret == AVERROR(EINVAL))
                        std::cerr << "EINVAL" << std::endl;
                    if (ret == AVERROR(EAGAIN))
                        std::cerr << "EAGAIN" << std::endl;
                    if (ret == AVERROR_EOF)
                        std::cerr << "AVERROR_EOF" << std::endl;
                    std::cerr << "Ошибка отправки пакета в декодер!" << std::endl;
                }

                av_packet_unref(pAVPacket);

                while (ret >= 0)
                {
                    ret = avcodec_receive_frame(pAVCodecContext, pAVFrame);
                    if (ret >= 0)
                    {
                        // Обработка декодированного видеокадра
                        width = pAVCodecContext->width;
                        height = pAVCodecContext->height;
                        parent_class->SizeImgWindowCallback(&width, &height);
                        

                        pSwsContext = sws_getContext(pAVCodecContext->width, pAVCodecContext->height, (AVPixelFormat)pAVFrame->format,
                            width, height, AV_PIX_FMT_RGB24, SWS_SPLINE,
                            0, 0, 0);

                        sws_scale_frame(pSwsContext, pAVFrameRGB, pAVFrame);

                        parent_class->DrawImgWindowCallback(pAVFrameRGB->data[0], pAVFrameRGB->height, pAVFrameRGB->width, pAVFrameRGB->linesize[0]);

                        sws_freeContext(pSwsContext);
                    }
                    else if (ret == AVERROR(EAGAIN))
                    {
                        std::cerr << "Декодер ожидает больше пакетов" << std::endl;
                    }
                    else
                    {
                        std::cerr << "Ошибка декодирования кадра!" << std::endl;
                    }

                    av_frame_unref(pAVFrame);
                    av_frame_unref(pAVFrameRGB);
                }
            }
        }
        delete[] NALU.data;
    }
}

// Функция для парсинга RTP заголовка
RTP_Header VideoStreamProc::parse_rtp_header(unsigned char* data)
{
    RTP_Header RTPHeader;

    RTPHeader.V = data[0] >> 6;
    RTPHeader.P = data[0] >> 5;
    RTPHeader.X = data[0] >> 4;
    RTPHeader.CC = data[0];

    RTPHeader.M = data[1] >> 7;
    RTPHeader.PT = data[1];

    RTPHeader.sequence_number = data[2] << 8 | data[3];

    RTPHeader.time_stamp = data[4] << 24 | data[5] << 16 | data[6] << 8 | data[7];
    RTPHeader.SSRC = data[8] << 24 | data[9] << 16 | data[10] << 8 | data[11];

    return RTPHeader;
}
