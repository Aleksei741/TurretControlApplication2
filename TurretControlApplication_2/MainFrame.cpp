#include "MainFrame.h"


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)
EVT_WINDOW_DESTROY(MainFrame::OnDestroy)
EVT_BUTTON(ID_BUTTON_CONNECT_MAINFRAME, MainFrame::ButConnect)
EVT_BUTTON(ID_BUTTON_DISCONNECT_MAINFRAME, MainFrame::ButDisconnect)
EVT_TOGGLEBUTTON(ID_BUTTON_MOUSE_MAINFRAME, MainFrame::ButMouse)
EVT_TOGGLEBUTTON(ID_BUTTON_KEYBOARD_MAINFRAME, MainFrame::ButKeyboard)
EVT_SIZE(MainFrame::OnResize)
EVT_MENU(ID_MENU_OPTION_VIDEO, MainFrame::ButtonEventVideoOption)
EVT_MENU(ID_MENU_OPTION_CONTROLE, MainFrame::ButtonEventControlOption)
EVT_MENU(ID_MENU_OPTION_AIM, MainFrame::ButtonEventAimOption)
EVT_MENU(ID_MENU_OPTION_DAMAGE, MainFrame::ButtonEventDamageOption)
EVT_MENU(ID_MENU_FILE_SAVE_COFIG, MainFrame::ButtonEventSaveConfig)
EVT_MENU(ID_MENU_FILE_LOAD_COFIG, MainFrame::ButtonEventLoadConfig)
EVT_MENU(ID_MENU_FILE_SAVE_TURRET, MainFrame::ButtonEventSaveTurret)
EVT_MENU(ID_MENU_FILE_LOAD_TURRET, MainFrame::ButtonEventLoadTurret)
wxEND_EVENT_TABLE()

//====================================================================================================
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
    wxIcon ico = wxIcon(gun_xpm);
    SetIcon(ico);

    CreateMenu();
    DrawingMainWindow();    

    ParamProc.loadConfig(&param);

    InitVideo();
}

//====================================================================================================
void MainFrame::DrawingMainWindow()
{
    //Изображаем GUI
    auto sizerGeneral = new wxBoxSizer(wxHORIZONTAL);
    wxPanel* panelControl = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    panelControl->SetMaxSize(wxSize(200, -1));
    auto sizerControlsPanel = new wxBoxSizer(wxVERTICAL);

    //-----------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------
    wxStaticLine* staticline1 = new wxStaticLine(panelControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    sizerControlsPanel->Add(staticline1, 0, wxGROW | wxALL, Border);
    //-----------------------------------------------------------------------------------------------------------------------
    //панель подключения
    combobxAvailableTurrets = new wxComboBox(panelControl, wxID_ANY, "Турель", wxDefaultPosition, wxSize(FromDIP(170), -1), 0, NULL, 0);
    sizerControlsPanel->Add(combobxAvailableTurrets, 0, wxGROW | wxALL, Border);
    auto sizerButtonConnectDisconnect = new wxBoxSizer(wxHORIZONTAL);
    buttonConnect = new wxButton(panelControl, ID_BUTTON_CONNECT_MAINFRAME, "Подключить", wxDefaultPosition, wxDefaultSize, 0);
    sizerButtonConnectDisconnect->Add(buttonConnect, 1, wxGROW | wxALL, Border);
    buttonDisconnect = new wxButton(panelControl, ID_BUTTON_DISCONNECT_MAINFRAME, "Отключить", wxDefaultPosition, wxDefaultSize, 0);
    sizerButtonConnectDisconnect->Add(buttonDisconnect, 1, wxGROW | wxALL, Border);
    sizerControlsPanel->Add(sizerButtonConnectDisconnect, 0, 0);
    
    //-----------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------
    wxStaticLine* staticline2 = new wxStaticLine(panelControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    sizerControlsPanel->Add(staticline2, 0, wxGROW | wxALL, Border);
    //-----------------------------------------------------------------------------------------------------------------------
    //Панель HP
    auto sizerHPIndicator = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* staticTextHP = new wxStaticText(panelControl, wxID_ANY, "Жизни", wxDefaultPosition, wxDefaultSize, 0);
    staticTextHP->Wrap(-1);
    sizerHPIndicator->Add(staticTextHP, 1, wxALL, Border);
    textHP = new wxStaticText(panelControl, wxID_ANY, "20", wxDefaultPosition, wxDefaultSize, 0);
    textHP->Wrap(-1);
    sizerHPIndicator->Add(textHP, 0, wxTOP | wxBOTTOM, Border);
    wxStaticText* staticTextSlash = new wxStaticText(panelControl, wxID_ANY, "/", wxDefaultPosition, wxDefaultSize, 0);
    sizerHPIndicator->Add(staticTextSlash, 0, wxTOP | wxBOTTOM, Border);
    textMaxHP = new wxStaticText(panelControl, wxID_ANY, "20", wxDefaultPosition, wxDefaultSize, 0);
    sizerHPIndicator->Add(textMaxHP, 0, wxTOP | wxBOTTOM | wxRIGHT, Border);
    sizerControlsPanel->Add(sizerHPIndicator, 0, wxGROW | wxALL, Border);
    gaugeHP = new wxGauge(panelControl, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL);
    gaugeHP->SetValue(50);
    gaugeHP->Pulse();
    sizerControlsPanel->Add(gaugeHP, 0, wxLEFT | wxRIGHT, Border);
    buttonResetHP = new wxButton(panelControl, ID_BUTTON_RESET_HP_MAINFRAME, "Сбросить повреждения", wxDefaultPosition, wxDefaultSize, 0);
    sizerControlsPanel->Add(buttonResetHP, 0, wxGROW | wxALL, Border);

    //-----------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------
    wxStaticLine* staticline3 = new wxStaticLine(panelControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    sizerControlsPanel->Add(staticline3, 0, wxGROW | wxALL, Border);
    //-----------------------------------------------------------------------------------------------------------------------
    //Панель имточника управления
    wxStaticText* staticTextSourceControl = new wxStaticText(panelControl, wxID_ANY, "Источник управлеия", wxDefaultPosition, wxDefaultSize, 0);
    sizerControlsPanel->Add(staticTextSourceControl, 0, wxALL, Border);
    auto sizerButtonControlSource = new wxBoxSizer(wxHORIZONTAL);
    toggleBtnMouse = new wxToggleButton(panelControl, ID_BUTTON_MOUSE_MAINFRAME, "Мышь", wxDefaultPosition, wxDefaultSize, 0);
    toggleBtnKeyboard = new wxToggleButton(panelControl, ID_BUTTON_KEYBOARD_MAINFRAME, "Клавиатура", wxDefaultPosition, wxDefaultSize, 0);
    toggleBtnKeyboard->SetValue(true);
    toggleBtnKeyboard->Disable();
    sizerButtonControlSource->Add(toggleBtnMouse, 1, wxLEFT | wxRIGHT, Border);
    sizerButtonControlSource->Add(toggleBtnKeyboard, 1, wxLEFT | wxRIGHT, Border);
    sizerControlsPanel->Add(sizerButtonControlSource, 0, wxGROW, 0);
    textSourceControl = new wxStaticText(panelControl, wxID_ANY, "Нажмите ESC", wxDefaultPosition, wxDefaultSize, 0);
    sizerControlsPanel->Add(textSourceControl, 0, wxGROW | wxALL, 5);
    textSourceControl->SetLabelText(" ");

    //-----------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------
    wxStaticLine* staticline4 = new wxStaticLine(panelControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    sizerControlsPanel->Add(staticline4, 0, wxGROW | wxALL, Border);
    //-----------------------------------------------------------------------------------------------------------------------    
    panelControl->SetSizerAndFit(sizerControlsPanel);

    //-----------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------
    auto sizerPositionY = new wxBoxSizer(wxVERTICAL);
    wxPanel* panelPositionY = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    panelPositionY->SetMaxSize(wxSize(50, -1));
    animationPositionUP = new wxMediaCtrl(panelPositionY, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(FromDIP(20), (20)));
    textPositionY = new wxTextCtrl(panelPositionY, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(FromDIP(40), -1));
    animationPositionDOWN = new wxMediaCtrl(panelPositionY, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(FromDIP(20), (20)));
    wxStaticText* textPositionY1 = new wxStaticText(panelPositionY, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    wxStaticText* textPositionY2 = new wxStaticText(panelPositionY, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    sizerPositionY->Add(textPositionY1, 1, 0, 0);
    sizerPositionY->Add(animationPositionUP, 0, wxCENTER | wxALL, 5);
    sizerPositionY->Add(textPositionY, 0, wxCENTER | wxALL, 5);
    sizerPositionY->Add(animationPositionDOWN, 0, wxCENTER | wxALL, 5);
    sizerPositionY->Add(textPositionY2, 1, 0, 0);
    panelPositionY->SetSizerAndFit(sizerPositionY);

    auto sizerVideoAndPositionX = new wxBoxSizer(wxVERTICAL);
    auto sizerPositionX = new wxBoxSizer(wxHORIZONTAL);
    wxPanel* panelVideoAndPositionX = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    
    panelVideo = new wxPanel(panelVideoAndPositionX, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    panelVideo->SetBackgroundColour(*wxWHITE);
    panelVideo->SetMinSize(wxSize(FromDIP(330), FromDIP(280)));
    
    wxPanel* panelPositionX = new wxPanel(panelVideoAndPositionX, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    panelPositionX->SetMaxSize(wxSize(-1, 50));
    animationPositionLEFT = new wxMediaCtrl(panelPositionX, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(FromDIP(20), (20)));
    textPositionX = new wxTextCtrl(panelPositionX, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(FromDIP(40), -1));
    animationPositionRIGHT = new wxMediaCtrl(panelPositionX, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(FromDIP(20), (20)));
    wxStaticText* textPositionX1 = new wxStaticText(panelPositionX, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    wxStaticText* textPositionX2 = new wxStaticText(panelPositionX, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    
    sizerPositionX->Add(textPositionX1, 1, wxCENTER | wxALL, 5);
    sizerPositionX->Add(animationPositionLEFT, 0, wxCENTER | wxALL, 5);
    sizerPositionX->Add(textPositionX, 0, wxCENTER | wxALL, 5);
    sizerPositionX->Add(animationPositionRIGHT,0, wxCENTER | wxALL, 5);
    sizerPositionX->Add(textPositionX2, 1, wxCENTER | wxALL, 5);
    panelPositionX->SetSizerAndFit(sizerPositionX);
    
    sizerVideoAndPositionX->Add(panelVideo, 1, wxGROW, 0);
    sizerVideoAndPositionX->Add(panelPositionX, 0, wxGROW, 0);
    panelVideoAndPositionX->SetSizerAndFit(sizerVideoAndPositionX);
    //-----------------------------------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------
    //Финал
    sizerGeneral->Add(panelControl, 1, wxGROW);
    sizerGeneral->Add(panelPositionY, 0, wxGROW);
    sizerGeneral->Add(panelVideoAndPositionX, 1, wxGROW);
    this->SetSizerAndFit(sizerGeneral);
}

//====================================================================================================
void MainFrame::CreateMenu(void)
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_MENU_FILE_SAVE_COFIG, "Сохранить конфигурацию", "Сохранить конфигурацию турели");
    menuFile->Append(ID_MENU_FILE_LOAD_COFIG, "Загрузить конфигурацию", "Загрузить конфигурацию турели");
    menuFile->AppendSeparator();
    menuFile->Append(ID_MENU_FILE_SAVE_TURRET, "Сохранить турели", "Сохранить текущий список турелей");
    menuFile->Append(ID_MENU_FILE_LOAD_TURRET, "Загрузить турели", "Загрузить текущий список турелей");

    wxMenu* menuOption = new wxMenu;
    menuOption->Append(ID_MENU_OPTION_VIDEO, "Видео", "Настройки видео");
    menuOption->Append(ID_MENU_OPTION_DAMAGE, "Урон", "Настройка жизней турели");
    menuOption->Append(ID_MENU_OPTION_CONTROLE, "Управление", "Настройки управления");
    menuOption->Append(ID_MENU_OPTION_AIM, "Прицел", "Настройки прицела");
    
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuMain = new wxMenuBar;

    menuMain->Append(menuFile, "&Файл");
    menuMain->Append(menuOption, "&Настройки");
    menuMain->Append(menuHelp, "&Помощь");

    // Associate the menu bar with this frame
    SetMenuBar(menuMain);
}

//====================================================================================================
void MainFrame::InitVideo(void)
{
    if (video)
        delete video;

    video = new VideoStreamProc(this, param.Video.port, param.Video.H26X);
}

//====================================================================================================
void MainFrame::AimDraw(wxMemoryDC &mdc, int width, int height)
{
    int cnt;
    float Kx;
    float Ky;

    static unsigned int Xleft;
    static unsigned int Xright;
    static unsigned int Xcenter;
    static unsigned int Ytop;
    static unsigned int Ybot;
    static unsigned int pixel_line;
    static wxColor color;
    static wxFont font;
    static int font_height;



    color.SetRGBA(param.Aim.Color);

    
    
    
    if (!font.SetNativeFontInfoUserDesc(param.Aim.FontInfo))
        font = wxFont(12, 
            wxFONTFAMILY_DEFAULT, 
            wxFONTSTYLE_NORMAL, 
            wxFONTWEIGHT_NORMAL,
            false, wxEmptyString, wxFONTENCODING_DEFAULT);
    

    Kx = (float)width / (float)param.Video.width;
    Ky = (float)height / (float)param.Video.height;

    font_height = font.GetPointSize();    
    font_height = round(Ky * (float)font_height);
    font.SetPointSize(font_height);
    
    pixel_line = round(param.Aim.PixelLine * Kx);
    if (!pixel_line)
        pixel_line = 1;

    wxPen pen(color, pixel_line, wxSOLID);
    mdc.SetPen(pen);
    mdc.SetTextForeground(color);
    mdc.SetFont(font);

    Xleft = round((float)param.Aim.X * Kx) - round((float)param.Aim.width * Kx) / 2;
    Xright = round((float)param.Aim.X * Kx) + round((float)param.Aim.width * Kx) / 2;
    Xcenter = round((float)param.Aim.X * Kx);
    Ytop = round((float)param.Aim.Y * Ky);
    Ybot = round((float)param.Aim.Y * Ky) + round((float)param.Aim.height * Ky);

    mdc.DrawLine(Xleft, Ytop, Xright, Ytop);
    mdc.DrawLine(Xcenter, Ytop, Xcenter, Ybot);

    for (cnt = 0; cnt < sizeof(param.Aim.AimText) / sizeof(param.Aim.AimText[0]); cnt++)
    {
        if (param.Aim.AimText[cnt].Active)
        {
            Xleft = round((float)param.Aim.X * Kx) - round((float)param.Aim.AimText[cnt].LenLine * Kx) / 2;
            Xright = round((float)param.Aim.X * Kx) + round((float)param.Aim.AimText[cnt].LenLine * Kx) / 2;
            Ytop = round((float)param.Aim.Y * Ky) + round((float)param.Aim.AimText[cnt].Offset * Ky);
            Xcenter = round((float)param.Aim.X * Kx);
            
            mdc.DrawLine(Xleft, Ytop, Xright, Ytop);
            mdc.DrawText(wxString(param.Aim.AimText[cnt].Text), wxPoint(Xright + 5, Ytop - font_height / 2));
        }
    }

    mdc.SetPen(wxNullPen);
    mdc.SetFont(wxNullFont);
    mdc.SetTextForeground(wxNullColour);
}

//====================================================================================================
//====================================================================================================
void MainFrame::SizeImgWindowCallback(int* width, int* height)
{
    float proportion;
    int w = *width;
    int h = *height;
    int panelWight, panelHeight;
    int resWight, resHeight;

    wxSize wxS = panelVideo->GetSize();
    panelWight = wxS.x;
    panelHeight = wxS.y;

    proportion = (float)w / (float)h;
    if (panelWight < w)
    {
        resWight = panelWight;
        resHeight = panelWight / proportion;

        if (resHeight > panelHeight)
        {
            resWight = panelHeight * proportion;
            resHeight = panelHeight;
        }
    }
    else if (panelHeight < h)
    {
        resWight = panelHeight * proportion;
        resHeight = panelHeight;

        if (resWight > panelWight)
        {
            resWight = panelWight;
            resHeight = panelWight / proportion;
        }
    }
    else
    {
        resWight = w;
        resHeight = h;
    }

    *width = resWight;
    *height = resHeight;
}
//====================================================================================================
void MainFrame::DrawImgWindowCallback(unsigned char* RGB, int height, int width, int linesize)
{
    unsigned long cnt;
    int line;
    int index;
    int PixelIndex;


    wxImage image(width, height, true);
    unsigned char* PixelData = image.GetData();
    for (line = 0; line < height; line++)
    {
        for (cnt = 0; cnt < width; cnt++)
        {
            PixelIndex = (line)*width + cnt;

            index = line * linesize + cnt * 3;

            PixelData[PixelIndex * 3 + 0] = RGB[index + 0]; //Blue
            PixelData[PixelIndex * 3 + 1] = RGB[index + 1];	//Green
            PixelData[PixelIndex * 3 + 2] = RGB[index + 2];	//Red            
        }
    }

    wxSize sizePanel = panelVideo->GetSize();

    wxClientDC dc(panelVideo);
    wxMemoryDC mdc;

    wxBitmap cat(image);

    mdc.SelectObject(cat);

    AimDraw(mdc, width, height);
    //mdc.SetPen(*wxRED_PEN);
    //mdc.DrawText("3232", wxPoint(10, 10));

    if (flagClearVideoPanel)
    {
        dc.Clear();
        flagClearVideoPanel--;
    }

    dc.Blit((sizePanel.x - width) / 2, (sizePanel.y - height) / 2, width, height, &mdc, 0, 0, wxCOPY, 0);

    mdc.SelectObject(wxNullBitmap);

}
//====================================================================================================
//====================================================================================================
//====================================================================================================
void MainFrame::ButDisconnect(wxCommandEvent& event)
{
    textSourceControl->SetLabelText("Disconnect");
    this->SetFocus();
}

//====================================================================================================
void MainFrame::ButConnect(wxCommandEvent& event)
{
    textSourceControl->SetLabelText("Connect");
    this->SetFocus();
}

//====================================================================================================
void MainFrame::ButMouse(wxCommandEvent& event)
{
    toggleBtnKeyboard->SetValue(false);
    toggleBtnKeyboard->Enable();
    toggleBtnMouse->Disable();
    textSourceControl->SetLabelText("Для выхода нажмите ESC");
    this->SetFocus();
}

//====================================================================================================
void MainFrame::ButKeyboard(wxCommandEvent& event)
{
    toggleBtnMouse->SetValue(false);
    toggleBtnMouse->Enable();
    toggleBtnKeyboard->Disable();
    textSourceControl->SetLabelText("");
    this->SetFocus();
}

//====================================================================================================
void MainFrame::OnClose(wxCloseEvent& event)
{
    delete video;
    ParamProc.saveConfig(&param);
    event.Skip();
}

//====================================================================================================
void MainFrame::OnDestroy(wxWindowDestroyEvent& event)
{
    event.Skip();
}
//====================================================================================================
void MainFrame::OnResize(wxSizeEvent& event)
{
    flagClearVideoPanel = 5;
    event.Skip();
}
//====================================================================================================

//====================================================================================================
void MainFrame::ButtonEventVideoOption(wxCommandEvent& WXUNUSED(event))
{
    if(!frameVideoOption)
        frameVideoOption = new VideoOptionFrame(this, &param.Video);
    
    if(!frameVideoOption->IsShown())
        frameVideoOption->Show();
    
    frameVideoOption->SetFocus();        
}
//====================================================================================================
void MainFrame::ButtonEventControlOption(wxCommandEvent& WXUNUSED(event))
{
    if (!frameControlOption)
        frameControlOption = new ControlOptionFrame(this, &param.M1, &param.M2, &param.Mouse);

    if (!frameControlOption->IsShown())
        frameControlOption->Show();

    frameControlOption->SetFocus();
}
//====================================================================================================
void MainFrame::ButtonEventAimOption(wxCommandEvent& WXUNUSED(event))
{
    if (!frameAimOption)
        frameAimOption = new AimOptionFrame(this, &param.Aim);

    if (!frameAimOption->IsShown())
        frameAimOption->Show();

    frameAimOption->SetFocus();
}
//====================================================================================================
void MainFrame::ButtonEventDamageOption(wxCommandEvent& WXUNUSED(event))
{
    if (!frameDamageOption)
        frameDamageOption = new DamageOptionFrame(this, &param.Damage);

    if (!frameDamageOption->IsShown())
        frameDamageOption->Show();

    frameDamageOption->SetFocus();
}
//====================================================================================================
void MainFrame::ButtonEventSaveConfig(wxCommandEvent& WXUNUSED(event))
{
    wxString file_path;
    wxFileDialog saveFileDialog(this, "Сохранить конфигурацию", "", "", "*.cfg", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    file_path = saveFileDialog.GetPath();

    ParamProc.saveConfig(&param, file_path.c_str());

}
//====================================================================================================
void MainFrame::ButtonEventLoadConfig(wxCommandEvent& WXUNUSED(event))
{
    wxString file_path;
    wxFileDialog openFileDialog(this, "Загрузить конфигурацию", "", "", "*.cfg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    file_path = openFileDialog.GetPath();
    ParamProc.loadConfig(&param, file_path.c_str());
}
//====================================================================================================
void MainFrame::ButtonEventSaveTurret(wxCommandEvent& WXUNUSED(event))
{

}
//====================================================================================================
void MainFrame::ButtonEventLoadTurret(wxCommandEvent& WXUNUSED(event))
{

}
//====================================================================================================