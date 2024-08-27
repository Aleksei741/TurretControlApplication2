#pragma once
#ifndef VIDEO_SREAM_PROC
#define VIDEO_SREAM_PROC

#include "MainFrame.h"

#include <winsock2.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <queue>
#include <list>
#include <chrono>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class MainFrame;

// —труктура RTP заголовка
struct RTP_Header 
{
    unsigned char V : 2;               //¬ерси€
    unsigned char P : 1;               //заполнение
    unsigned char X : 1;               //расширение
    unsigned char CC : 4;              //количество CSRC

    unsigned char M : 1;               //маркер (флаг последнего пакета AU),
    unsigned char PT : 7;              //полезна€ нагрузка (тип данных носител€ полезной нагрузки RTP, H264 = 96)

    unsigned short sequence_number;  //ѕор€дковый номер: пор€дковый номер пакета RTP, увеличенный на 1.
    unsigned int time_stamp;         //временна€ метка выборки медиа. 
    unsigned int SSRC;               //ѕакет данных имеет одинаковое происхождение.
};

typedef struct {
    unsigned char* data;
    int size;
}RTPData_DType;

typedef enum {
    VSP_CODEC_H264,
    VSP_CODEC_H265,
}VSP_CodecH26X_DType;

class VideoStreamProc
{
    std::thread threadReadSocket;
    std::thread threadDecodeH265;
    std::mutex MutexSocketData;

    std::mutex MutexDecode;
    std::condition_variable cvDecode;

    AVFormatContext* pAVFormatContext;
    AVCodecContext* pAVCodecContext;
    const AVCodec* pAVCodec;
    AVFrame* pAVFrame;
    AVFrame* pAVFrameRGB;
    SwsContext* pSwsContext;
    AVPacket* pAVPacket;
    AVDictionary* dict = nullptr;
    AVCodecParserContext* pAVCodecParserContext;

    unsigned short set_port;
    VSP_CodecH26X_DType codec;
    bool running = false;
    SOCKET RTPSocket;
    RTPData_DType NALU_FU;
    unsigned char test_data[200] = { 0 };
    
    clock_t timeSourceID;
    unsigned int SourceID = 0xFFFFFFFF;

    const int inteval = 0x01000000;

    std::queue<RTPData_DType> qNalu;
    std::queue<RTPData_DType> qPacket;
    std::list<unsigned short> WatchRepet;

    int ReciveData_Procedure(void);
    int DecodeH26X_Procedure(void);
    int process_rtp_packet(unsigned char* data, size_t data_size);
    void decode_H26X_process(void);
    void extract_nalus_H264(unsigned char* data, size_t data_size);
    void extract_nalus_H265(unsigned char* data, size_t data_size);
    RTP_Header parse_rtp_header(unsigned char* data);
    
    MainFrame* parent_class;

public:
    VideoStreamProc(MainFrame* parent, unsigned short port, unsigned char VideoCodecH265);
    ~VideoStreamProc();
};

#endif