#include "AimOptionFrame.h"

wxBEGIN_EVENT_TABLE(AimOptionFrame, wxFrame)
EVT_CLOSE(AimOptionFrame::OnClose)
EVT_FONTPICKER_CHANGED(ID_BUTTON_AIM_FONT, AimOptionFrame::procButtonFont)
EVT_COLOURPICKER_CHANGED(ID_BUTTON_AIM_COLOR, AimOptionFrame::procButtonColor)
EVT_SPINCTRL(ID_SPIN_AIM_X, AimOptionFrame::procSpinX)
EVT_SPINCTRL(ID_SPIN_AIM_Y, AimOptionFrame::procSpinY)
EVT_SPINCTRL(ID_SPIN_AIM_WIDTH, AimOptionFrame::procSpinWidth)
EVT_SPINCTRL(ID_SPIN_AIM_HEIGHT, AimOptionFrame::procSpinHeight)
EVT_SPINCTRL(ID_SPIN_AIM_PIXEL_LINE, AimOptionFrame::procSpinPixelLine)

EVT_CHECKBOX(ID_BUTTON_AIM_TEXT1_ACTIVE, AimOptionFrame::procCheckBoxText1Active)
EVT_SPINCTRL(ID_BUTTON_AIM_TEXT1_OFFSET, AimOptionFrame::procSpinText1Offset)
EVT_SPINCTRL(ID_BUTTON_AIM_TEXT1_LEN, AimOptionFrame::procSpinText1Len)
EVT_TEXT(ID_BUTTON_AIM_TEXT1_TEXT, AimOptionFrame::procTextBoxText1Text)

EVT_CHECKBOX(ID_BUTTON_AIM_TEXT2_ACTIVE, AimOptionFrame::procCheckBoxText2Active)
EVT_SPINCTRL(ID_BUTTON_AIM_TEXT2_OFFSET, AimOptionFrame::procSpinText2Offset)
EVT_SPINCTRL(ID_BUTTON_AIM_TEXT2_LEN, AimOptionFrame::procSpinText2Len)
EVT_TEXT(ID_BUTTON_AIM_TEXT2_TEXT, AimOptionFrame::procTextBoxText2Text)

EVT_CHECKBOX(ID_BUTTON_AIM_TEXT3_ACTIVE, AimOptionFrame::procCheckBoxText3Active)
EVT_SPINCTRL(ID_BUTTON_AIM_TEXT3_OFFSET, AimOptionFrame::procSpinText3Offset)
EVT_SPINCTRL(ID_BUTTON_AIM_TEXT3_LEN, AimOptionFrame::procSpinText3Len)
EVT_TEXT(ID_BUTTON_AIM_TEXT3_TEXT, AimOptionFrame::procTextBoxText3Text)

wxEND_EVENT_TABLE()

AimOptionFrame::AimOptionFrame(wxWindow* parent, void* aim_)
    : wxFrame(parent, wxID_ANY, "Настройка прицела", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxTOPLEVEL_EX_DIALOG | wxFRAME_FLOAT_ON_PARENT | wxFRAME_NO_TASKBAR)
{
    wxIcon ico = wxIcon(gun_xpm);
    SetIcon(ico);

    aim = (AimOption_DType*)aim_;

    auto sizerGeneral = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* panelMain = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    auto sizerPanelMain = new wxBoxSizer(wxVERTICAL);

    //X
    auto sizerX = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionX = new wxStaticText(panelMain, wxID_ANY, "    Координаты X\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionX->SetMinSize(wxSize(200, -1));
    sizerX->Add(textOptionX, 1, wxGROW | wxALL, 5);

    spinOptionX = new wxSpinCtrl(panelMain, ID_SPIN_AIM_X, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionX->SetMinSize(wxSize(80, -1));
    spinOptionX->SetMax(10000);
    spinOptionX->SetMin(1);
    spinOptionX->SetValue(aim->X);
    sizerX->Add(spinOptionX, 0, wxALL, 5);

    wxStaticText* textUnitX = new wxStaticText(panelMain, wxID_ANY, " пиксель", wxDefaultPosition, wxDefaultSize, 0);
    textUnitX->SetMinSize(wxSize(60, -1));
    sizerX->Add(textUnitX, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerX, 1, wxGROW, 0);

    //Y
    auto sizerY = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionY = new wxStaticText(panelMain, wxID_ANY, "    Координаты Y\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionY->SetMinSize(wxSize(200, -1));
    sizerY->Add(textOptionY, 1, wxGROW | wxALL, 5);

    spinOptionY = new wxSpinCtrl(panelMain, ID_SPIN_AIM_Y, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionY->SetMinSize(wxSize(80, -1));
    spinOptionY->SetMax(10000);
    spinOptionY->SetMin(1);
    spinOptionY->SetValue(aim->Y);
    sizerY->Add(spinOptionY, 0, wxALL, 5);

    wxStaticText* textUnitY = new wxStaticText(panelMain, wxID_ANY, " пиксель", wxDefaultPosition, wxDefaultSize, 0);
    textUnitY->SetMinSize(wxSize(60, -1));
    sizerY->Add(textUnitY, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerY, 1, wxGROW, 0);

    //Width
    auto sizerWidth = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionWidth = new wxStaticText(panelMain, wxID_ANY, "    Ширина прицела\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionWidth->SetMinSize(wxSize(200, -1));
    sizerWidth->Add(textOptionWidth, 1, wxGROW | wxALL, 5);

    spinOptionWidth = new wxSpinCtrl(panelMain, ID_SPIN_AIM_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionWidth->SetMinSize(wxSize(80, -1));
    spinOptionWidth->SetMax(10000);
    spinOptionWidth->SetMin(1);
    spinOptionWidth->SetValue(aim->width);
    sizerWidth->Add(spinOptionWidth, 0, wxALL, 5);

    wxStaticText* textUnitWidth = new wxStaticText(panelMain, wxID_ANY, " пиксель", wxDefaultPosition, wxDefaultSize, 0);
    textUnitWidth->SetMinSize(wxSize(60, -1));
    sizerWidth->Add(textUnitWidth, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerWidth, 1, wxGROW, 0);

    //Height
    auto sizerHeight = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionHeight = new wxStaticText(panelMain, wxID_ANY, "    Высота прицела\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionHeight->SetMinSize(wxSize(200, -1));
    sizerHeight->Add(textOptionHeight, 1, wxGROW | wxALL, 5);

    spinOptionHeight = new wxSpinCtrl(panelMain, ID_SPIN_AIM_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionHeight->SetMinSize(wxSize(80, -1));
    spinOptionHeight->SetMax(10000);
    spinOptionHeight->SetMin(1);
    spinOptionHeight->SetValue(aim->height);
    sizerHeight->Add(spinOptionHeight, 0, wxALL, 5);

    wxStaticText* textUnitHeight = new wxStaticText(panelMain, wxID_ANY, " пиксель", wxDefaultPosition, wxDefaultSize, 0);
    textUnitHeight->SetMinSize(wxSize(60, -1));
    sizerHeight->Add(textUnitHeight, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerHeight, 1, wxGROW, 0);

    //PixelLine
    auto sizerPixelLine = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionPixelLine = new wxStaticText(panelMain, wxID_ANY, "    Толщина линий\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionPixelLine->SetMinSize(wxSize(200, -1));
    sizerPixelLine->Add(textOptionPixelLine, 1, wxGROW | wxALL, 5);

    spinOptionPixelLine = new wxSpinCtrl(panelMain, ID_SPIN_AIM_PIXEL_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionPixelLine->SetMinSize(wxSize(80, -1));
    spinOptionPixelLine->SetMax(20);
    spinOptionPixelLine->SetMin(1);
    spinOptionPixelLine->SetValue(aim->PixelLine);
    sizerPixelLine->Add(spinOptionPixelLine, 0, wxALL, 5);

    wxStaticText* textUnitPixelLine = new wxStaticText(panelMain, wxID_ANY, " пиксель", wxDefaultPosition, wxDefaultSize, 0);
    textUnitPixelLine->SetMinSize(wxSize(60, -1));
    sizerPixelLine->Add(textUnitPixelLine, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerPixelLine, 1, wxGROW, 0);

    //Color
    auto sizerColor = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionColor = new wxStaticText(panelMain, wxID_ANY, "    Цвет прицела\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionColor->SetMinSize(wxSize(200, -1));
    sizerColor->Add(textOptionColor, 1, wxGROW | wxALL, 5);

    buttonColor = new wxColourPickerCtrl(panelMain, ID_BUTTON_AIM_COLOR, *wxBLACK, wxDefaultPosition, wxSize(100, 25), wxCLRP_DEFAULT_STYLE);
    wxColour color;
    color.SetRGBA(aim->Color);
    buttonColor->SetColour(color);
    sizerColor->Add(buttonColor, 0, wxALL, 10);
    sizerPanelMain->Add(sizerColor, 1, wxGROW, 0);

    //Font
    auto sizerFont = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionFont = new wxStaticText(panelMain, wxID_ANY, "    Шрифт надписей\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionFont->SetMinSize(wxSize(200, -1));
    sizerFont->Add(textOptionFont, 1, wxGROW | wxALL, 5);

    wxFont font;
    if (!font.SetNativeFontInfoUserDesc(aim->FontInfo))
        font = wxFont(12,
            wxFONTFAMILY_DEFAULT,
            wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL,
            false, wxEmptyString, wxFONTENCODING_DEFAULT);
    buttonFont = new wxFontPickerCtrl(panelMain, ID_BUTTON_AIM_FONT, font, wxDefaultPosition, wxSize(100, 25), wxFNTP_FONTDESC_AS_LABEL);
    sizerFont->Add(buttonFont, 0, wxALL, 10);
    sizerPanelMain->Add(sizerFont, 1, wxGROW, 0);

    //text1
    auto sizerText1 = new wxBoxSizer(wxHORIZONTAL);
    checkBoxText1Active = new wxCheckBox(panelMain, ID_BUTTON_AIM_TEXT1_ACTIVE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    checkBoxText1Active->SetValue(aim->AimText[0].Active);
    sizerText1->Add(checkBoxText1Active, 0, wxALL, 10);

    wxStaticText* textText1Len = new wxStaticText(panelMain, wxID_ANY, "длина\n", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizerText1->Add(textText1Len, 1, wxGROW | wxALL, 5);

    spinOptionText1LenLine = new wxSpinCtrl(panelMain, ID_BUTTON_AIM_TEXT1_LEN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionText1LenLine->SetMinSize(wxSize(50, -1));
    spinOptionText1LenLine->SetMax(500);
    spinOptionText1LenLine->SetMin(1);
    spinOptionText1LenLine->SetValue(aim->AimText[0].LenLine);
    sizerText1->Add(spinOptionText1LenLine, 0, wxALL, 5);

    wxStaticText* textText1Offset = new wxStaticText(panelMain, wxID_ANY, "смещение\n", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizerText1->Add(textText1Offset, 1, wxGROW | wxALL, 5);

    spinOptionText1Offset = new wxSpinCtrl(panelMain, ID_BUTTON_AIM_TEXT1_OFFSET, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionText1Offset->SetMinSize(wxSize(50, -1));
    spinOptionText1Offset->SetMax(500);
    spinOptionText1Offset->SetMin(1);
    spinOptionText1Offset->SetValue(aim->AimText[0].Offset);
    sizerText1->Add(spinOptionText1Offset, 0, wxALL, 5);

    wxStaticText* textText1Text = new wxStaticText(panelMain, wxID_ANY, "текст\n", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizerText1->Add(textText1Text, 1, wxGROW | wxALL, 5);

    textOptionText1Text = new wxTextCtrl(panelMain, ID_BUTTON_AIM_TEXT1_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    textOptionText1Text->SetMinSize(wxSize(80, -1));
    textOptionText1Text->SetValue(aim->AimText[0].Text);
    sizerText1->Add(textOptionText1Text, 0, wxALL, 5);

    sizerPanelMain->Add(sizerText1, 1, wxGROW, 0);

    //text2
    auto sizerText2 = new wxBoxSizer(wxHORIZONTAL);
    checkBoxText2Active = new wxCheckBox(panelMain, ID_BUTTON_AIM_TEXT2_ACTIVE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    checkBoxText2Active->SetValue(aim->AimText[1].Active);
    sizerText2->Add(checkBoxText2Active, 0, wxALL, 10);

    wxStaticText* textText2Len = new wxStaticText(panelMain, wxID_ANY, "длина\n", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizerText2->Add(textText2Len, 1, wxGROW | wxALL, 5);

    spinOptionText2LenLine = new wxSpinCtrl(panelMain, ID_BUTTON_AIM_TEXT2_LEN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionText2LenLine->SetMinSize(wxSize(50, -1));
    spinOptionText2LenLine->SetMax(500);
    spinOptionText2LenLine->SetMin(1);
    spinOptionText2LenLine->SetValue(aim->AimText[1].LenLine);
    sizerText2->Add(spinOptionText2LenLine, 0, wxALL, 5);

    wxStaticText* textText2Offset = new wxStaticText(panelMain, wxID_ANY, "смещение\n", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizerText2->Add(textText2Offset, 1, wxGROW | wxALL, 5);

    spinOptionText2Offset = new wxSpinCtrl(panelMain, ID_BUTTON_AIM_TEXT2_OFFSET, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionText2Offset->SetMinSize(wxSize(50, -1));
    spinOptionText2Offset->SetMax(500);
    spinOptionText2Offset->SetMin(1);
    spinOptionText2Offset->SetValue(aim->AimText[1].Offset);
    sizerText2->Add(spinOptionText2Offset, 0, wxALL, 5);

    wxStaticText* textText2Text = new wxStaticText(panelMain, wxID_ANY, "текст\n", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizerText2->Add(textText2Text, 1, wxGROW | wxALL, 5);

    textOptionText2Text = new wxTextCtrl(panelMain, ID_BUTTON_AIM_TEXT2_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    textOptionText2Text->SetMinSize(wxSize(80, -1));
    textOptionText2Text->SetValue(aim->AimText[1].Text);
    sizerText2->Add(textOptionText2Text, 0, wxALL, 5);

    sizerPanelMain->Add(sizerText2, 1, wxGROW, 0);

    //text3
    auto sizerText3 = new wxBoxSizer(wxHORIZONTAL);
    checkBoxText3Active = new wxCheckBox(panelMain, ID_BUTTON_AIM_TEXT3_ACTIVE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    checkBoxText3Active->SetValue(aim->AimText[2].Active);
    sizerText3->Add(checkBoxText3Active, 0, wxALL, 10);

    wxStaticText* textText3Len = new wxStaticText(panelMain, wxID_ANY, "длина\n", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizerText3->Add(textText3Len, 1, wxGROW | wxALL, 5);

    spinOptionText3LenLine = new wxSpinCtrl(panelMain, ID_BUTTON_AIM_TEXT3_LEN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionText3LenLine->SetMinSize(wxSize(50, -1));
    spinOptionText3LenLine->SetMax(500);
    spinOptionText3LenLine->SetMin(1);
    spinOptionText3LenLine->SetValue(aim->AimText[2].LenLine);
    sizerText3->Add(spinOptionText3LenLine, 0, wxALL, 5);

    wxStaticText* textText3Offset = new wxStaticText(panelMain, wxID_ANY, "смещение\n", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizerText3->Add(textText3Offset, 1, wxGROW | wxALL, 5);

    spinOptionText3Offset = new wxSpinCtrl(panelMain, ID_BUTTON_AIM_TEXT3_OFFSET, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinOptionText3Offset->SetMinSize(wxSize(50, -1));
    spinOptionText3Offset->SetMax(500);
    spinOptionText3Offset->SetMin(1);
    spinOptionText3Offset->SetValue(aim->AimText[2].Offset);
    sizerText3->Add(spinOptionText3Offset, 0, wxALL, 5);

    wxStaticText* textText3Text = new wxStaticText(panelMain, wxID_ANY, "текст\n", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizerText3->Add(textText3Text, 1, wxGROW | wxALL, 5);

    textOptionText3Text = new wxTextCtrl(panelMain, ID_BUTTON_AIM_TEXT3_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    textOptionText3Text->SetMinSize(wxSize(80, -1));
    textOptionText3Text->SetValue(aim->AimText[2].Text);
    sizerText3->Add(textOptionText3Text, 0, wxALL, 5);

    sizerPanelMain->Add(sizerText3, 1, wxGROW, 0);

    panelMain->SetSizerAndFit(sizerPanelMain);

    sizerGeneral->Add(panelMain, 1, wxGROW);
    this->SetSizerAndFit(sizerGeneral);
}
//====================================================================================================
void AimOptionFrame::UpdateValue(void)
{

    spinOptionX->SetValue(aim->X);
    spinOptionY->SetValue(aim->Y);
    spinOptionWidth->SetValue(aim->width);
    spinOptionHeight->SetValue(aim->height);
    spinOptionPixelLine->SetValue(aim->PixelLine);
    
    wxColour color;
    color.SetRGBA(aim->Color);
    buttonColor->SetColour(color);
    
    wxFont font;
    if (!font.SetNativeFontInfoUserDesc(aim->FontInfo))
        font = wxFont(12,
            wxFONTFAMILY_DEFAULT,
            wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL,
            false, wxEmptyString, wxFONTENCODING_DEFAULT);
    buttonFont->SetFont(font);

    //checkBoxText1Active->SetValue(aim->AimText[0].Active);

}
//====================================================================================================
//====================================================================================================
//====================================================================================================
void AimOptionFrame::OnClose(wxCloseEvent& event)
{
    MainFrame* parent = (MainFrame*)GetParent();
    if (parent)
    {
        parent->frameAimOption = nullptr;
    }
    this->Destroy();
    event.Skip();
}
//====================================================================================================
void AimOptionFrame::procButtonFont(wxFontPickerEvent& event)
{
    wxFont font = buttonFont->GetSelectedFont();
    wxString native_font_info = font.GetNativeFontInfoUserDesc();
    std::strcpy(aim->FontInfo, native_font_info.c_str());
}
//====================================================================================================
void AimOptionFrame::procButtonColor(wxColourPickerEvent& event)
{
    wxColour color;
    color = buttonColor->GetColour();
    aim->Color = color.GetRGBA();    
}
//====================================================================================================
void AimOptionFrame::procSpinX(wxSpinEvent& event)
{
    aim->X = spinOptionX->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinY(wxSpinEvent& event)
{
    aim->Y = spinOptionY->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinWidth(wxSpinEvent& event)
{
    aim->width = spinOptionWidth->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinHeight(wxSpinEvent& event)
{
    aim->height = spinOptionHeight->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinPixelLine(wxSpinEvent& event)
{
    aim->PixelLine = spinOptionPixelLine->GetValue();
}



//====================================================================================================
void AimOptionFrame::procCheckBoxText1Active(wxCommandEvent& event)
{
    aim->AimText[0].Active = checkBoxText1Active->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinText1Offset(wxSpinEvent& event)
{
    aim->AimText[0].Offset = spinOptionText1Offset->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinText1Len(wxSpinEvent& event)
{
    aim->AimText[0].LenLine = spinOptionText1LenLine->GetValue();
}
//====================================================================================================
void AimOptionFrame::procTextBoxText1Text(wxCommandEvent& event)
{
    std::strcpy(aim->AimText[0].Text, textOptionText1Text->GetValue());
}
//====================================================================================================



//====================================================================================================
void AimOptionFrame::procCheckBoxText2Active(wxCommandEvent& event)
{
    aim->AimText[1].Active = checkBoxText2Active->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinText2Offset(wxSpinEvent& event)
{
    aim->AimText[1].Offset = spinOptionText2Offset->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinText2Len(wxSpinEvent& event)
{
    aim->AimText[1].LenLine = spinOptionText2LenLine->GetValue();
}
//====================================================================================================
void AimOptionFrame::procTextBoxText2Text(wxCommandEvent& event)
{
    std::strcpy(aim->AimText[1].Text, textOptionText2Text->GetValue());
}
//====================================================================================================


//====================================================================================================
void AimOptionFrame::procCheckBoxText3Active(wxCommandEvent& event)
{
    aim->AimText[2].Active = checkBoxText3Active->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinText3Offset(wxSpinEvent& event)
{
    aim->AimText[2].Offset = spinOptionText3Offset->GetValue();
}
//====================================================================================================
void AimOptionFrame::procSpinText3Len(wxSpinEvent& event)
{
    aim->AimText[2].LenLine = spinOptionText3LenLine->GetValue();
}
//====================================================================================================
void AimOptionFrame::procTextBoxText3Text(wxCommandEvent& event)
{
    std::strcpy(aim->AimText[2].Text, textOptionText3Text->GetValue());
}
//====================================================================================================


