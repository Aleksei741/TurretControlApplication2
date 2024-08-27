#include "DamageOptionFrame.h"

wxBEGIN_EVENT_TABLE(DamageOptionFrame, wxFrame)
EVT_CLOSE(DamageOptionFrame::OnClose)
EVT_SPINCTRL(ID_SPIN_DAMAGE_DELAY_HP, DamageOptionFrame::procSpinHP)
EVT_SPINCTRL(ID_SPIN_DAMAGE_DELAY_DAMAGE_MIN, DamageOptionFrame::procSpinDelayDamage_min)
EVT_SPINCTRL(ID_SPIN_DAMAGE_DELAY_DAMAGE_SEC, DamageOptionFrame::procSpinDelayDamage_sec)
EVT_SPINCTRL(ID_SPIN_DAMAGE_DELAY_SENSOR_MS, DamageOptionFrame::procSpinDelaySensor_ms)
EVT_CHECKBOX(ID_CHECKBOX_DAMAGE_MOVEMENT_OFF, DamageOptionFrame::procCheckBoxMovementOff)
EVT_CHECKBOX(ID_CHECKBOX_DAMAGE_WEAPON_OFF, DamageOptionFrame::procCheckBoxWeaponOff)
EVT_CHECKBOX(ID_CHECKBOX_DAMAGE_VIDEO_OFF, DamageOptionFrame::procCheckBoxVideoOff)
wxEND_EVENT_TABLE()

DamageOptionFrame::DamageOptionFrame(wxWindow* parent, void* dmg_)
    : wxFrame(parent, wxID_ANY, "Настройка урона турели", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxTOPLEVEL_EX_DIALOG | wxFRAME_FLOAT_ON_PARENT | wxFRAME_NO_TASKBAR)
{
    wxIcon ico = wxIcon(gun_xpm);
    SetIcon(ico);

    dmg = (DamageOption_DType*)dmg_;

    auto sizerGeneral = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* panelMain = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    auto sizerPanelMain = new wxBoxSizer(wxVERTICAL);

    //HP
    auto sizerHP = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionHP = new wxStaticText(panelMain, wxID_ANY, "    Жизни турели. 1 попадание по мишени снимает одну жизнь.\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionHP->SetMinSize(wxSize(250, -1));
    sizerHP->Add(textOptionHP, 1, wxGROW | wxALL, 5);

    spinOptionHP = new wxSpinCtrl(panelMain, ID_SPIN_DAMAGE_DELAY_HP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, dmg->HP);
    spinOptionHP->SetMinSize(wxSize(80, -1));
    sizerHP->Add(spinOptionHP, 0, wxALL, 5);

    wxStaticText* textUnitHP = new wxStaticText(panelMain, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    textUnitHP->SetMinSize(wxSize(40, -1));
    sizerHP->Add(textUnitHP, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerHP, 1, wxGROW, 0);

    //DamageTime
    auto sizerDamageTime = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionDamageTime = new wxStaticText(panelMain, wxID_ANY, "    Время сбороса еденицы урона с момента его нанесения\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionDamageTime->SetMinSize(wxSize(250, -1));
    sizerDamageTime->Add(textOptionDamageTime, 1, wxGROW | wxALL, 5);

    spinOptionDamageMinute = new wxSpinCtrl(panelMain, ID_SPIN_DAMAGE_DELAY_DAMAGE_MIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 59, dmg->DamageDelay_min);
    spinOptionDamageMinute->SetMinSize(wxSize(50, -1));
    sizerDamageTime->Add(spinOptionDamageMinute, 0, wxALL, 5);

    wxStaticText* textUnitDamageMin = new wxStaticText(panelMain, wxID_ANY, "мин.", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    textUnitDamageMin->SetMinSize(wxSize(30, -1));
    sizerDamageTime->Add(textUnitDamageMin, 0, wxTOP | wxBOTTOM, 10);

    spinOptionDamageSecunde = new wxSpinCtrl(panelMain, ID_SPIN_DAMAGE_DELAY_DAMAGE_SEC, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 59, dmg->DamageDelay_sec);
    spinOptionDamageSecunde->SetMinSize(wxSize(50, -1));
    sizerDamageTime->Add(spinOptionDamageSecunde, 0, wxALL, 5);

    wxStaticText* textUnitDamageSec = new wxStaticText(panelMain, wxID_ANY, "сек.", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    textUnitDamageSec->SetMinSize(wxSize(30, -1));
    sizerDamageTime->Add(textUnitDamageSec, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerDamageTime, 1, wxGROW, 0);

    //Critical damage actions
    wxStaticBoxSizer* sizerBoxCalc = new wxStaticBoxSizer(new wxStaticBox(panelMain, wxID_ANY, "При получении критического урона"), wxVERTICAL);

    checkBoxWeaponOff = new wxCheckBox(panelMain, ID_CHECKBOX_DAMAGE_WEAPON_OFF, "Отключать стрельбу", wxDefaultPosition, wxDefaultSize, 0);
    checkBoxWeaponOff->SetValue(dmg->WeaponOff);
    sizerBoxCalc->Add(checkBoxWeaponOff, 1, wxALL, 5);

    checkBoxMovementOff = new wxCheckBox(panelMain, ID_CHECKBOX_DAMAGE_MOVEMENT_OFF, "Отключать движение", wxDefaultPosition, wxDefaultSize, 0);
    checkBoxMovementOff->SetValue(dmg->MovementOff);
    sizerBoxCalc->Add(checkBoxMovementOff, 1, wxALL, 5);

    checkBoxVideoOff = new wxCheckBox(panelMain, ID_CHECKBOX_DAMAGE_VIDEO_OFF, "Отключать изображение", wxDefaultPosition, wxDefaultSize, 0);
    checkBoxVideoOff->SetValue(dmg->VideoOff);
    sizerBoxCalc->Add(checkBoxVideoOff, 1, wxALL, 5);
    sizerPanelMain->Add(sizerBoxCalc, 0, wxGROW | wxALL, 10);

    //DelaySensor
    auto sizerDelaySensor = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textOptionDelaySensor = new wxStaticText(panelMain, wxID_ANY, "    Задержка повторного срабатывания сенсора\n", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textOptionDelaySensor->SetMinSize(wxSize(250, -1));
    sizerDelaySensor->Add(textOptionDelaySensor, 1, wxGROW | wxALL, 5);

    spinOptionDelaySensor = new wxSpinCtrl(panelMain, ID_SPIN_DAMAGE_DELAY_SENSOR_MS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, dmg->DelaySensor_ms);
    spinOptionDelaySensor->SetMinSize(wxSize(80, -1));
    sizerDelaySensor->Add(spinOptionDelaySensor, 0, wxALL, 5);

    wxStaticText* textUnitDelaySensor = new wxStaticText(panelMain, wxID_ANY, " мс", wxDefaultPosition, wxDefaultSize, 0);
    textUnitDelaySensor->SetMinSize(wxSize(40, -1));
    sizerDelaySensor->Add(textUnitDelaySensor, 0, wxTOP | wxBOTTOM, 10);
    sizerPanelMain->Add(sizerDelaySensor, 1, wxGROW, 0);

    panelMain->SetSizerAndFit(sizerPanelMain);

    sizerGeneral->Add(panelMain, 1, wxGROW);
    this->SetSizerAndFit(sizerGeneral);
}
//====================================================================================================
//====================================================================================================
//====================================================================================================
void DamageOptionFrame::OnClose(wxCloseEvent& event)
{
    MainFrame* parent = (MainFrame*)GetParent();
    if (parent)
    {
        parent->frameDamageOption = nullptr;
    }
    this->Destroy();
    event.Skip();
}
//====================================================================================================
void DamageOptionFrame::procSpinHP(wxSpinEvent& event)
{
    dmg->HP = spinOptionHP->GetValue();
}
//====================================================================================================
void DamageOptionFrame::procSpinDelayDamage_min(wxSpinEvent& event)
{
    dmg->DamageDelay_min = spinOptionDamageMinute->GetValue();
}
//====================================================================================================
void DamageOptionFrame::procSpinDelayDamage_sec(wxSpinEvent& event)
{
    dmg->DamageDelay_sec = spinOptionDamageSecunde->GetValue();
}
//====================================================================================================
void DamageOptionFrame::procSpinDelaySensor_ms(wxSpinEvent& event)
{
    dmg->DelaySensor_ms = spinOptionDelaySensor->GetValue();
}
//====================================================================================================
void DamageOptionFrame::procCheckBoxMovementOff(wxCommandEvent& event)
{
    dmg->MovementOff = checkBoxMovementOff->GetValue();
}
//====================================================================================================
void DamageOptionFrame::procCheckBoxWeaponOff(wxCommandEvent& event)
{
    dmg->WeaponOff = checkBoxWeaponOff->GetValue();
}
//====================================================================================================
void DamageOptionFrame::procCheckBoxVideoOff(wxCommandEvent& event)
{
    dmg->VideoOff = checkBoxVideoOff->GetValue();
}
//====================================================================================================