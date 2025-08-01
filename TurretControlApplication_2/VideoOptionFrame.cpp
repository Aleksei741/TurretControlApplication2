#include "VideoOptionFrame.h"

wxBEGIN_EVENT_TABLE(VideoOptionFrame, wxFrame)
EVT_CLOSE(VideoOptionFrame::OnClose)
EVT_COMBOBOX(ID_COMBOBOX_VIDEO_RESOLUTION_OPTION, VideoOptionFrame::procVideoResolution)
EVT_SPINCTRL(ID_SPIN_BITRATE_OPTION, VideoOptionFrame::procBitrate)
EVT_SPINCTRL(ID_SPIN_FRAMERATE_OPTION, VideoOptionFrame::procFramerate)
EVT_COMBOBOX(ID_COMBOBOX_CODEC_OPTION, VideoOptionFrame::procCodec)
EVT_SPINCTRL(ID_SPIN_PORT_OPTION, VideoOptionFrame::procPort)
wxEND_EVENT_TABLE()

//====================================================================================================
VideoOptionFrame::VideoOptionFrame(wxWindow* parent, void* VideoParam_)
    : wxFrame(parent, wxID_ANY, "Настройка видео", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxTOPLEVEL_EX_DIALOG | wxFRAME_FLOAT_ON_PARENT | wxFRAME_NO_TASKBAR)
{
    wxIcon ico = wxIcon(gun_xpm);
    SetIcon(ico);

    VideoParam = (VideoOption_DType*)VideoParam_;

    auto sizerGeneral = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* panelMain = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    auto sizerPanelMain = new wxBoxSizer(wxVERTICAL);

    //Разрешение
    auto sizerSolution = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionSolution = new wxStaticText(panelMain, wxID_ANY, "    Разрешение\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionSolution->SetMinSize(wxSize(200, -1));
    sizerSolution->Add(textOptionSolution, 1, wxGROW | wxALL, 5);

    combobxVideoResolution = new wxComboBox(panelMain, ID_COMBOBOX_VIDEO_RESOLUTION_OPTION, wxEmptyString, wxDefaultPosition, wxSize(FromDIP(170), -1), 0, NULL, wxCB_READONLY);
    combobxVideoResolution->SetMinSize(wxSize(80, -1));
    sizerSolution->Add(combobxVideoResolution, 0, wxALL, 5);
    combobxVideoResolution->Append("VGA 480p");
    combobxVideoResolution->Append("HD 720p");
    combobxVideoResolution->Append("FHD 1080p");
    combobxVideoResolution->Append("2K 1440p");

    if(VideoParam->width == 640 && VideoParam->height == 480)
        combobxVideoResolution->SetSelection(0);
    else if (VideoParam->width == 1280 && VideoParam->height == 720)
        combobxVideoResolution->SetSelection(1);
    else if (VideoParam->width == 1920 && VideoParam->height == 1080)
        combobxVideoResolution->SetSelection(2);
    else if (VideoParam->width == 2560 && VideoParam->height == 1440)
        combobxVideoResolution->SetSelection(3);

    wxStaticText* textUnitResolution = new wxStaticText(panelMain, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    textUnitResolution->SetMinSize(wxSize(40, -1));
    sizerSolution->Add(textUnitResolution, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerSolution, 1, wxGROW, 0);

    //Битрейт
    auto sizerBitrate = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionBitrate = new wxStaticText(panelMain, wxID_ANY, "    Битрейт\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionBitrate->SetMinSize(wxSize(200, -1));
    sizerBitrate->Add(textOptionBitrate, 1, wxGROW | wxALL, 5);

    spinOptionBitrate = new wxSpinCtrl(panelMain, ID_SPIN_BITRATE_OPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionBitrate->SetMinSize(wxSize(80, -1));
    spinOptionBitrate->SetMax(50000);
    spinOptionBitrate->SetMin(200);
    spinOptionBitrate->SetValue(VideoParam->bitrate);
    sizerBitrate->Add(spinOptionBitrate, 0, wxALL, 5);

    wxStaticText* textUnitBitrate = new wxStaticText(panelMain, wxID_ANY, " Kбайт", wxDefaultPosition, wxDefaultSize, 0);
    textUnitBitrate->SetMinSize(wxSize(40, -1));
    sizerBitrate->Add(textUnitBitrate, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerBitrate, 1, wxGROW, 0);

    //framerate
    auto sizerFramerate = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionFramerate = new wxStaticText(panelMain, wxID_ANY, "    Кадров в секунду\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionFramerate->SetMinSize(wxSize(200, -1));
    sizerFramerate->Add(textOptionFramerate, 1, wxGROW | wxALL, 5);

    spinOptionFramerate = new wxSpinCtrl(panelMain, ID_SPIN_FRAMERATE_OPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionFramerate->SetMinSize(wxSize(80, -1));
    spinOptionFramerate->SetMax(60);
    spinOptionFramerate->SetMin(10);
    spinOptionFramerate->SetValue(VideoParam->framerate);
    sizerFramerate->Add(spinOptionFramerate, 0, wxALL, 5);

    wxStaticText* textUnitFramerate = new wxStaticText(panelMain, wxID_ANY, " Кадр/сек", wxDefaultPosition, wxDefaultSize, 0);
    textUnitFramerate->SetMinSize(wxSize(40, -1));
    sizerFramerate->Add(textUnitFramerate, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerFramerate, 1, wxGROW, 0);

    //-------
    wxStaticLine *m_staticline1 = new wxStaticLine(panelMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    sizerPanelMain->Add(m_staticline1, 0, wxEXPAND | wxALL, 5);
    //-------

    //Codec
    auto sizerH26X = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionH26X = new wxStaticText(panelMain, wxID_ANY, "    Cтандарт сжатия видео\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionH26X->SetMinSize(wxSize(200, -1));
    sizerH26X->Add(textOptionH26X, 1, wxGROW | wxALL, 5);

    combobxVideoH26X = new wxComboBox(panelMain, ID_COMBOBOX_CODEC_OPTION, wxEmptyString, wxDefaultPosition, wxSize(FromDIP(170), -1), 0, NULL, wxCB_READONLY);
    combobxVideoH26X->SetMinSize(wxSize(80, -1));
    sizerH26X->Add(combobxVideoH26X, 0, wxALL, 5);
    combobxVideoH26X->Append("h264");
    combobxVideoH26X->Append("h265");

    if (VideoParam->H26X == CODEC_H264)
        combobxVideoH26X->SetSelection(0);
    else if (VideoParam->H26X == CODEC_H265)
        combobxVideoH26X->SetSelection(1);

    wxStaticText* textUnitH26X = new wxStaticText(panelMain, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    textUnitH26X->SetMinSize(wxSize(40, -1));
    sizerH26X->Add(textUnitH26X, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerH26X, 1, wxGROW, 0);

    //port
    auto sizerVideoPort = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionVideoPort = new wxStaticText(panelMain, wxID_ANY, "    Порт\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionVideoPort->SetMinSize(wxSize(200, -1));
    sizerVideoPort->Add(textOptionVideoPort, 1, wxGROW | wxALL, 5);

    spinOptionVideoPort = new wxSpinCtrl(panelMain, ID_SPIN_PORT_OPTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionVideoPort->SetMinSize(wxSize(80, -1));
    spinOptionVideoPort->SetMax(65535);
    spinOptionVideoPort->SetMin(10000);
    spinOptionVideoPort->SetValue(VideoParam->port);
    sizerVideoPort->Add(spinOptionVideoPort, 0, wxALL, 5);

    wxStaticText* textUnitVideoPort = new wxStaticText(panelMain, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    textUnitVideoPort->SetMinSize(wxSize(40, -1));
    sizerVideoPort->Add(textUnitVideoPort, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerVideoPort, 1, wxGROW, 0);

    panelMain->SetSizerAndFit(sizerPanelMain);
    
    sizerGeneral->Add(panelMain, 1, wxGROW);
    this->SetSizerAndFit(sizerGeneral);
}


//====================================================================================================
//====================================================================================================
//====================================================================================================

void VideoOptionFrame::OnClose(wxCloseEvent& event)
{
    ParentFrame* parent = (ParentFrame*)GetParent();
    if (parent)
    {
        parent->frameVideoOption = nullptr;
    }
    this->Destroy();
    event.Skip();
}
//====================================================================================================
void VideoOptionFrame::procVideoResolution(wxCommandEvent& event)
{
    int ID_select = combobxVideoResolution->GetSelection();

    switch (ID_select)
    {
    case 0:
        VideoParam->width = 640;
        VideoParam->height = 480;
        VideoParam->bitrate = 832;
        break;
    case 1:
        VideoParam->width = 1280;
        VideoParam->height = 720;
        VideoParam->bitrate = 2048;
        break;
    case 2:
        VideoParam->width = 1920;
        VideoParam->height = 1080;
        VideoParam->bitrate = 4096;
        break;
    case 3:
        VideoParam->width = 2560;
        VideoParam->height = 1440;
        VideoParam->bitrate = 8192;
        break;
    default:
        VideoParam->width = 1280;
        VideoParam->height = 720;
        VideoParam->bitrate = 2048;
        break;
    }

    spinOptionBitrate->SetValue(VideoParam->bitrate);
}
//====================================================================================================
void VideoOptionFrame::procBitrate(wxSpinEvent& event)
{
    VideoParam->bitrate = spinOptionBitrate->GetValue();
}
//====================================================================================================
void VideoOptionFrame::procFramerate(wxSpinEvent& event)
{
    VideoParam->framerate = spinOptionFramerate->GetValue();
}
//====================================================================================================
void VideoOptionFrame::procCodec(wxCommandEvent& event)
{
    int ID_select = combobxVideoH26X->GetSelection();
    VideoParam->H26X = static_cast<CodecH26X_DType>(ID_select);
    ParentFrame* parent = (ParentFrame*)GetParent();
    parent->InitVideo();
}
//====================================================================================================
void VideoOptionFrame::procPort(wxSpinEvent& event)
{
    VideoParam->port = spinOptionVideoPort->GetValue();
    ParentFrame* parent = (ParentFrame*)GetParent();
    parent->InitVideo();
}
