#include "ControlOptionFrame.h"

wxBEGIN_EVENT_TABLE(ControlOptionFrame, wxFrame)
EVT_CLOSE(ControlOptionFrame::OnClose)
EVT_SPINCTRL(ID_SPIN_MIN_DEGREE_POSITION_M1, ControlOptionFrame::procMinDegreePositionM1)
EVT_SPINCTRL(ID_SPIN_MAX_DEGREE_POSITION_M1, ControlOptionFrame::procMaxDegreePositionM1)
EVT_SPINCTRLDOUBLE(ID_SPIN_ROTATION_SPEED_M1, ControlOptionFrame::procRotationSpeedM1)
EVT_SPINCTRL(ID_SPIN_MICRO_STEPS_M1, ControlOptionFrame::procMicroStepsStepperM1)
EVT_SPINCTRLDOUBLE(ID_SPIN_REDUCTION_RATIO_M1, ControlOptionFrame::procReductionRatioM1)
EVT_SPINCTRL(ID_SPIN_MIN_DEGREE_POSITION_M2, ControlOptionFrame::procMinDegreePositionM2)
EVT_SPINCTRL(ID_SPIN_MAX_DEGREE_POSITION_M2, ControlOptionFrame::procMaxDegreePositionM2)
EVT_SPINCTRLDOUBLE(ID_SPIN_ROTATION_SPEED_M2, ControlOptionFrame::procRotationSpeedM2)
EVT_SPINCTRL(ID_SPIN_MICRO_STEPS_M2, ControlOptionFrame::procMicroStepsStepperM2)
EVT_SPINCTRLDOUBLE(ID_SPIN_REDUCTION_RATIO_M2, ControlOptionFrame::procReductionRatioM2)
EVT_BUTTON(ID_BUTTON_RESET_POSITION, ControlOptionFrame::procResetPosition)
EVT_CHECKBOX(ID_CHECKBOX_NO_LIMIT, ControlOptionFrame::procNpLimit)
EVT_SLIDER(ID_SLIDER_MOUSE, ControlOptionFrame::procMouse)
wxEND_EVENT_TABLE()

ControlOptionFrame::ControlOptionFrame(wxWindow* parent, void* m1_, void* m2_, void* mouse_)
    : wxFrame(parent, wxID_ANY, "Настройка управления", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxTOPLEVEL_EX_DIALOG | wxFRAME_FLOAT_ON_PARENT | wxFRAME_NO_TASKBAR)
{
    wxIcon ico = wxIcon(gun_xpm);
    SetIcon(ico);

    m1 = (MotorOption_DType*)m1_;
    m2 = (MotorOption_DType*)m2_;
    mouse = (MouseOptoin_DType*)mouse_;

    auto sizerGeneral = new wxBoxSizer(wxHORIZONTAL);
    
    wxScrolledWindow* scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(1000, 800), wxVSCROLL);
    scrolledWindow->SetScrollRate(0, 10); // Задаем скорость прокрутки по горизонтали и вертикали

    wxPanel* panelMain = new wxPanel(scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    auto sizerPanelMain = new wxBoxSizer(wxVERTICAL);

    auto sizerBoxM1 = DrawMotor1Param(panelMain);
    auto sizerBoxM2 = DrawMotor2Param(panelMain);

    //Настройки мотора 1
    sizerPanelMain->Add(sizerBoxM1, 1, wxGROW | wxALL, 5);

    //Настройки мотора 2
    sizerPanelMain->Add(sizerBoxM2, 1, wxGROW | wxALL, 5);

    //Сброс положения
    auto sizerResetPosition = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* MaxDegreePosition = new wxStaticText(panelMain, wxID_ANY, "    Сброс текущей позиции турели", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    MaxDegreePosition->SetMinSize(wxSize(250, -1));
    sizerResetPosition->Add(MaxDegreePosition, 1, wxGROW | wxALL, 5);
    
    buttonResetPosition = new wxButton(panelMain, ID_BUTTON_RESET_POSITION, "Сброс", wxDefaultPosition, wxSize(80, 25), 0);
    sizerResetPosition->Add(buttonResetPosition, 0, wxALL, 5);
        
    sizerPanelMain->Add(sizerResetPosition, 0, wxGROW | wxALL, 5);

    //Движение без ограничений
    auto sizerNoLimit = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textNoLimit = new wxStaticText(panelMain, wxID_ANY, "    Не ограничивать движение турели", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textNoLimit->SetMinSize(wxSize(250, -1));
    sizerNoLimit->Add(textNoLimit, 1, wxGROW | wxALL, 5);

    checkBoxNoLimit = new wxCheckBox(panelMain, ID_CHECKBOX_NO_LIMIT, wxEmptyString, wxDefaultPosition, wxSize(80, 25), 0);
    sizerNoLimit->Add(checkBoxNoLimit, 0, wxALL | wxALIGN_CENTER, 5);

    sizerPanelMain->Add(sizerNoLimit, 0, wxGROW | wxALL, 5);

    //Чувствительность мыши
    auto sizerMouse = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textMouse = new wxStaticText(panelMain, wxID_ANY, "    Чувствительность мыши", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textMouse->SetMinSize(wxSize(250, -1));
    sizerMouse->Add(textMouse, 1, wxGROW | wxALL, 5);

    textReadOnlyMouse = new wxTextCtrl(panelMain, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTER | wxTE_READONLY);
    wxString tMouse = wxString::Format("%1.2f", mouse->Sensitivity);
    textReadOnlyMouse->SetValue(tMouse);
    textReadOnlyMouse->SetMinSize(wxSize(80, -1));
    sizerMouse->Add(textReadOnlyMouse, 0, wxALL, 5);

    sliderMouse = new wxSlider(panelMain, ID_SLIDER_MOUSE, (int)(mouse->Sensitivity*100), 0, 250, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    
    sizerPanelMain->Add(sizerMouse, 0, wxGROW | wxALL, 5);
    sizerPanelMain->Add(sliderMouse, 0, wxGROW | wxALL, 5);

    panelMain->SetSizerAndFit(sizerPanelMain);

    sizerGeneral->Add(panelMain, 1, wxGROW);
    scrolledWindow->SetSizerAndFit(sizerGeneral);

    wxSize s = panelMain->GetSize();
    s = wxSize(s.GetX() + 25, 450);
    this->SetSize(s);
}
//====================================================================================================
wxStaticBoxSizer* ControlOptionFrame::DrawMotor1Param(void* parent_)
{
    wxWindow* parent = (wxWindow *) parent_;

    wxStaticBoxSizer* sizerBoxM1 = new wxStaticBoxSizer(new wxStaticBox(parent, wxID_ANY, "Движение влево, вправо"), wxVERTICAL);

    //Ограничение вращения влево
    auto sizerMinDegreePosition = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textMinDegreePosition = new wxStaticText(parent, wxID_ANY, "    Ограничение вращения влево", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textMinDegreePosition->SetMinSize(wxSize(250, -1));
    sizerMinDegreePosition->Add(textMinDegreePosition, 1, wxGROW | wxALL, 5);

    spinMinDegreePositionM1 = new wxSpinCtrl(parent, ID_SPIN_MIN_DEGREE_POSITION_M1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinMinDegreePositionM1->SetMinSize(wxSize(80, -1));
    spinMinDegreePositionM1->SetMax(180);
    spinMinDegreePositionM1->SetMin(0);
    spinMinDegreePositionM1->SetValue(m1->MinDegreePosition);
    sizerMinDegreePosition->Add(spinMinDegreePositionM1, 0, wxALL, 5);

    wxStaticText* textUnitMinDegreePosition = new wxStaticText(parent, wxID_ANY, "град.", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMinDegreePosition->SetMinSize(wxSize(60, -1));
    sizerMinDegreePosition->Add(textUnitMinDegreePosition, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM1->Add(sizerMinDegreePosition, 0, wxGROW, 0);

    //Ограничение вращения вправо
    auto sizerMaxDegreePosition = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* MaxDegreePosition = new wxStaticText(parent, wxID_ANY, "    Ограничение вращения вправо", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    MaxDegreePosition->SetMinSize(wxSize(250, -1));
    sizerMaxDegreePosition->Add(MaxDegreePosition, 1, wxGROW | wxALL, 5);

    spinMaxDegreePositionM1 = new wxSpinCtrl(parent, ID_SPIN_MAX_DEGREE_POSITION_M1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinMaxDegreePositionM1->SetMinSize(wxSize(80, -1));
    spinMaxDegreePositionM1->SetMax(180);
    spinMaxDegreePositionM1->SetMin(0);
    spinMaxDegreePositionM1->SetValue(m1->MaxDegreePosition);
    sizerMaxDegreePosition->Add(spinMaxDegreePositionM1, 0, wxALL, 5);

    wxStaticText* textUnitMaxDegreePosition = new wxStaticText(parent, wxID_ANY, "град.", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMaxDegreePosition->SetMinSize(wxSize(60, -1));
    sizerMaxDegreePosition->Add(textUnitMaxDegreePosition, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM1->Add(sizerMaxDegreePosition, 0, wxGROW, 0);

    //Скорость вращения
    auto sizerRotationSpeed = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textRotationSpeed = new wxStaticText(parent, wxID_ANY, "    Скорость вращения", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textRotationSpeed->SetMinSize(wxSize(250, -1));
    sizerRotationSpeed->Add(textRotationSpeed, 1, wxGROW | wxALL, 5);

    spinRotationSpeedM1 = new wxSpinCtrlDouble(parent, ID_SPIN_ROTATION_SPEED_M1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.001, 20, m1->RotationSpeed, 1);
    spinRotationSpeedM1->SetDigits(3);
    spinRotationSpeedM1->SetMinSize(wxSize(80, -1));
    sizerRotationSpeed->Add(spinRotationSpeedM1, 0, wxALL, 5);

    wxStaticText* textUnitRotationSpeed = new wxStaticText(parent, wxID_ANY, "град/сек", wxDefaultPosition, wxDefaultSize, 0);
    textUnitRotationSpeed->SetMinSize(wxSize(60, -1));
    sizerRotationSpeed->Add(textUnitRotationSpeed, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM1->Add(sizerRotationSpeed, 0, wxGROW, 0);

    //Микрошагов на один оборот двигателя
    auto sizerMicroStepsStepperMotor = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textMaxMicroStepsStepperMotor = new wxStaticText(parent, wxID_ANY, "    Микрошагов на один оборот двигателя", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textMaxMicroStepsStepperMotor->SetMinSize(wxSize(250, -1));
    sizerMicroStepsStepperMotor->Add(textMaxMicroStepsStepperMotor, 1, wxGROW | wxALL, 5);

    spinMicroStepsStepperM1 = new wxSpinCtrl(parent, ID_SPIN_MICRO_STEPS_M1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinMicroStepsStepperM1->SetMinSize(wxSize(80, -1));
    spinMicroStepsStepperM1->SetMax(500000);
    spinMicroStepsStepperM1->SetMin(1000);
    spinMicroStepsStepperM1->SetValue(m1->MicroStepsStepperMotor);
    sizerMicroStepsStepperMotor->Add(spinMicroStepsStepperM1, 0, wxALL, 5);

    wxStaticText* textUnitMicroStepsStepperMotor = new wxStaticText(parent, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMicroStepsStepperMotor->SetMinSize(wxSize(60, -1));
    sizerMicroStepsStepperMotor->Add(textUnitMicroStepsStepperMotor, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM1->Add(sizerMicroStepsStepperMotor, 0, wxGROW, 0);

    //Коэффициент редукции
    auto sizerReductionRatioStepperMotor = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textReductionRatioStepperMotor = new wxStaticText(parent, wxID_ANY, "    Коэффициент редукции", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textReductionRatioStepperMotor->SetMinSize(wxSize(250, -1));
    sizerReductionRatioStepperMotor->Add(textReductionRatioStepperMotor, 1, wxGROW | wxALL, 5);

    spinReductionRatioStepperM1 = new wxSpinCtrlDouble(parent, ID_SPIN_REDUCTION_RATIO_M1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.001, 500, m1->ReductionRatioStepperMotor, 1);
    spinReductionRatioStepperM1->SetDigits(3);
    spinReductionRatioStepperM1->SetMinSize(wxSize(80, -1));
    //spinRotationSpeed->SetValue(m1->ReductionRatioStepperMotor);
    sizerReductionRatioStepperMotor->Add(spinReductionRatioStepperM1, 0, wxALL, 5);

    wxStaticText* textUnitReductionRatioStepperMotor = new wxStaticText(parent, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    textUnitReductionRatioStepperMotor->SetMinSize(wxSize(60, -1));
    sizerReductionRatioStepperMotor->Add(textUnitReductionRatioStepperMotor, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM1->Add(sizerReductionRatioStepperMotor, 0, wxGROW, 0);

    wxStaticBoxSizer* sizerBoxCalc = new wxStaticBoxSizer(new wxStaticBox(parent, wxID_ANY, "Расчетные параметры"), wxVERTICAL);
    //Ограничение шагов влево
    auto sizerStepsPosition = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textMinSteps = new wxStaticText(parent, wxID_ANY, "Шагов влево", wxDefaultPosition, wxDefaultSize, 0);
    textMinSteps->SetMinSize(wxSize(100, -1));
    sizerStepsPosition->Add(textMinSteps, 1, wxGROW | wxALL, 5);

    textReadOnlyMinStepsM1 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    wxString tMinSteps = wxString::Format("%d", m1->MinStepsPosition);
    textReadOnlyMinStepsM1->SetValue(tMinSteps);
    textReadOnlyMinStepsM1->SetMinSize(wxSize(60, -1));
    sizerStepsPosition->Add(textReadOnlyMinStepsM1, 0, wxALL, 5);

    wxStaticText* textUnitMinSteps = new wxStaticText(parent, wxID_ANY, "шаг", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMinSteps->SetMinSize(wxSize(60, -1));
    sizerStepsPosition->Add(textUnitMinSteps, 0, wxTOP | wxBOTTOM, 10);

    //Ограничение шагов вправо
    wxStaticText* textMaxSteps = new wxStaticText(parent, wxID_ANY, "Шагов вправо", wxDefaultPosition, wxDefaultSize, 0);
    textMaxSteps->SetMinSize(wxSize(100, -1));
    sizerStepsPosition->Add(textMaxSteps, 1, wxGROW | wxALL, 5);

    textReadOnlyMaxStepsM1 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    wxString tMaxSteps = wxString::Format("%d", m1->MaxStepsPosition);
    textReadOnlyMaxStepsM1->SetValue(tMaxSteps);
    textReadOnlyMaxStepsM1->SetMinSize(wxSize(60, -1));
    sizerStepsPosition->Add(textReadOnlyMaxStepsM1, 0, wxALL, 5);

    wxStaticText* textUnitMaxSteps = new wxStaticText(parent, wxID_ANY, "шаг", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMaxSteps->SetMinSize(wxSize(60, -1));
    sizerStepsPosition->Add(textUnitMaxSteps, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxCalc->Add(sizerStepsPosition, 1, wxGROW, 0);

    //Частота ШИМ
    auto sizerFreqPeriod = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textFreq = new wxStaticText(parent, wxID_ANY, "Частота ШИМ", wxDefaultPosition, wxDefaultSize, 0);
    textFreq->SetMinSize(wxSize(100, -1));
    sizerFreqPeriod->Add(textFreq, 1, wxGROW | wxALL, 5);

    textReadOnlyFreqM1 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    wxString tFreq = wxString::Format("%03.0f", m1->Freq);
    textReadOnlyFreqM1->SetValue(tFreq);
    textReadOnlyFreqM1->SetMinSize(wxSize(60, -1));
    //spinRotationSpeed->SetValue(m1->ReductionRatioStepperMotor);
    sizerFreqPeriod->Add(textReadOnlyFreqM1, 0, wxALL, 5);

    wxStaticText* textUnitFreq = new wxStaticText(parent, wxID_ANY, "Гц", wxDefaultPosition, wxDefaultSize, 0);
    textUnitFreq->SetMinSize(wxSize(60, -1));
    sizerFreqPeriod->Add(textUnitFreq, 0, wxTOP | wxBOTTOM, 10);
    //sizerBoxCalc->Add(sizerFreqPeriod, 1, wxGROW, 0);

    //Период
    wxStaticText* textPeriod = new wxStaticText(parent, wxID_ANY, "Период ШИМ", wxDefaultPosition, wxDefaultSize, 0);
    textPeriod->SetMinSize(wxSize(100, -1));
    sizerFreqPeriod->Add(textPeriod, 1, wxGROW | wxALL, 5);

    textReadOnlyPeriodM1 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    wxString tPeriod = wxString::Format("%.6f", m1->Period);
    textReadOnlyPeriodM1->SetValue(tPeriod);
    textReadOnlyPeriodM1->SetMinSize(wxSize(60, -1));
    sizerFreqPeriod->Add(textReadOnlyPeriodM1, 0, wxALL, 5);

    wxStaticText* textUnitPeriod = new wxStaticText(parent, wxID_ANY, "сек", wxDefaultPosition, wxDefaultSize, 0);
    textUnitPeriod->SetMinSize(wxSize(60, -1));
    sizerFreqPeriod->Add(textUnitPeriod, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxCalc->Add(sizerFreqPeriod, 1, wxGROW, 0);

    sizerBoxM1->Add(sizerBoxCalc, 1, wxGROW | wxALL, 5);

    return sizerBoxM1;
}

//====================================================================================================
wxStaticBoxSizer* ControlOptionFrame::DrawMotor2Param(void* parent_)
{
    wxWindow* parent = (wxWindow*)parent_;

    wxStaticBoxSizer* sizerBoxM2 = new wxStaticBoxSizer(new wxStaticBox(parent, wxID_ANY, "Движение вверх, вниз"), wxVERTICAL);

    //Ограничение вращения влево
    auto sizerMinDegreePosition = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textMinDegreePosition = new wxStaticText(parent, wxID_ANY, "    Ограничение вращения вверх", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textMinDegreePosition->SetMinSize(wxSize(250, -1));
    sizerMinDegreePosition->Add(textMinDegreePosition, 1, wxGROW | wxALL, 5);

    spinMinDegreePositionM2 = new wxSpinCtrl(parent, ID_SPIN_MIN_DEGREE_POSITION_M2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinMinDegreePositionM2->SetMinSize(wxSize(80, -1));
    spinMinDegreePositionM2->SetMax(180);
    spinMinDegreePositionM2->SetMin(0);
    spinMinDegreePositionM2->SetValue(m2->MinDegreePosition);
    sizerMinDegreePosition->Add(spinMinDegreePositionM2, 0, wxALL, 5);

    wxStaticText* textUnitMinDegreePosition = new wxStaticText(parent, wxID_ANY, "град.", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMinDegreePosition->SetMinSize(wxSize(60, -1));
    sizerMinDegreePosition->Add(textUnitMinDegreePosition, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM2->Add(sizerMinDegreePosition, 0, wxGROW, 0);

    //Ограничение вращения вправо
    auto sizerMaxDegreePosition = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* MaxDegreePosition = new wxStaticText(parent, wxID_ANY, "    Ограничение вращения вниз", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    MaxDegreePosition->SetMinSize(wxSize(250, -1));
    sizerMaxDegreePosition->Add(MaxDegreePosition, 1, wxGROW | wxALL, 5);

    spinMaxDegreePositionM2 = new wxSpinCtrl(parent, ID_SPIN_MAX_DEGREE_POSITION_M2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinMaxDegreePositionM2->SetMinSize(wxSize(80, -1));
    spinMaxDegreePositionM2->SetMax(180);
    spinMaxDegreePositionM2->SetMin(0);
    spinMaxDegreePositionM2->SetValue(m2->MaxDegreePosition);
    sizerMaxDegreePosition->Add(spinMaxDegreePositionM2, 0, wxALL, 5);

    wxStaticText* textUnitMaxDegreePosition = new wxStaticText(parent, wxID_ANY, "град.", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMaxDegreePosition->SetMinSize(wxSize(60, -1));
    sizerMaxDegreePosition->Add(textUnitMaxDegreePosition, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM2->Add(sizerMaxDegreePosition, 0, wxGROW, 0);

    //Скорость вращения
    auto sizerRotationSpeed = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textRotationSpeed = new wxStaticText(parent, wxID_ANY, "    Скорость вращения", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textRotationSpeed->SetMinSize(wxSize(250, -1));
    sizerRotationSpeed->Add(textRotationSpeed, 1, wxGROW | wxALL, 5);

    spinRotationSpeedM2 = new wxSpinCtrlDouble(parent, ID_SPIN_ROTATION_SPEED_M2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.001, 20, m2->RotationSpeed, 1);
    spinRotationSpeedM2->SetDigits(3);
    spinRotationSpeedM2->SetMinSize(wxSize(80, -1));
    sizerRotationSpeed->Add(spinRotationSpeedM2, 0, wxALL, 5);

    wxStaticText* textUnitRotationSpeed = new wxStaticText(parent, wxID_ANY, "град/сек", wxDefaultPosition, wxDefaultSize, 0);
    textUnitRotationSpeed->SetMinSize(wxSize(60, -1));
    sizerRotationSpeed->Add(textUnitRotationSpeed, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM2->Add(sizerRotationSpeed, 0, wxGROW, 0);

    //Микрошагов на один оборот двигателя
    auto sizerMicroStepsStepperMotor = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textMaxMicroStepsStepperMotor = new wxStaticText(parent, wxID_ANY, "    Микрошагов на один оборот двигателя", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textMaxMicroStepsStepperMotor->SetMinSize(wxSize(250, -1));
    sizerMicroStepsStepperMotor->Add(textMaxMicroStepsStepperMotor, 1, wxGROW | wxALL, 5);

    spinMicroStepsStepperM2 = new wxSpinCtrl(parent, ID_SPIN_MICRO_STEPS_M2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
    spinMicroStepsStepperM2->SetMinSize(wxSize(80, -1));
    spinMicroStepsStepperM2->SetMax(500000);
    spinMicroStepsStepperM2->SetMin(1000);
    spinMicroStepsStepperM2->SetValue(m2->MicroStepsStepperMotor);
    sizerMicroStepsStepperMotor->Add(spinMicroStepsStepperM2, 0, wxALL, 5);

    wxStaticText* textUnitMicroStepsStepperMotor = new wxStaticText(parent, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMicroStepsStepperMotor->SetMinSize(wxSize(60, -1));
    sizerMicroStepsStepperMotor->Add(textUnitMicroStepsStepperMotor, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM2->Add(sizerMicroStepsStepperMotor, 0, wxGROW, 0);

    //Коэффициент редукции
    auto sizerReductionRatioStepperMotor = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textReductionRatioStepperMotor = new wxStaticText(parent, wxID_ANY, "    Коэффициент редукции", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
    textReductionRatioStepperMotor->SetMinSize(wxSize(250, -1));
    sizerReductionRatioStepperMotor->Add(textReductionRatioStepperMotor, 1, wxGROW | wxALL, 5);

    spinReductionRatioStepperM2 = new wxSpinCtrlDouble(parent, ID_SPIN_REDUCTION_RATIO_M2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.001, 500, m2->ReductionRatioStepperMotor, 1);
    spinReductionRatioStepperM2->SetDigits(3);
    spinReductionRatioStepperM2->SetMinSize(wxSize(80, -1));
    //spinRotationSpeed->SetValue(m2->ReductionRatioStepperMotor);
    sizerReductionRatioStepperMotor->Add(spinReductionRatioStepperM2, 0, wxALL, 5);

    wxStaticText* textUnitReductionRatioStepperMotor = new wxStaticText(parent, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, 0);
    textUnitReductionRatioStepperMotor->SetMinSize(wxSize(60, -1));
    sizerReductionRatioStepperMotor->Add(textUnitReductionRatioStepperMotor, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxM2->Add(sizerReductionRatioStepperMotor, 0, wxGROW, 0);

    wxStaticBoxSizer* sizerBoxCalc = new wxStaticBoxSizer(new wxStaticBox(parent, wxID_ANY, "Расчетные параметры"), wxVERTICAL);
    //Ограничение шагов 
    auto sizerStepsPosition = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textMinSteps = new wxStaticText(parent, wxID_ANY, "Шагов вниз", wxDefaultPosition, wxDefaultSize, 0);
    textMinSteps->SetMinSize(wxSize(100, -1));
    sizerStepsPosition->Add(textMinSteps, 1, wxGROW | wxALL, 5);

    textReadOnlyMinStepsM2 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    wxString tMinSteps = wxString::Format("%d", m2->MinStepsPosition);
    textReadOnlyMinStepsM2->SetValue(tMinSteps);
    textReadOnlyMinStepsM2->SetMinSize(wxSize(60, -1));
    sizerStepsPosition->Add(textReadOnlyMinStepsM2, 0, wxALL, 5);

    wxStaticText* textUnitMinSteps = new wxStaticText(parent, wxID_ANY, "шаг", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMinSteps->SetMinSize(wxSize(60, -1));
    sizerStepsPosition->Add(textUnitMinSteps, 0, wxTOP | wxBOTTOM, 10);

    //Ограничение шагов 
    wxStaticText* textMaxSteps = new wxStaticText(parent, wxID_ANY, "Шагов вверх", wxDefaultPosition, wxDefaultSize, 0);
    textMaxSteps->SetMinSize(wxSize(100, -1));
    sizerStepsPosition->Add(textMaxSteps, 1, wxGROW | wxALL, 5);

    textReadOnlyMaxStepsM2 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    wxString tMaxSteps = wxString::Format("%d", m2->MaxStepsPosition);
    textReadOnlyMaxStepsM2->SetValue(tMaxSteps);
    textReadOnlyMaxStepsM2->SetMinSize(wxSize(60, -1));
    sizerStepsPosition->Add(textReadOnlyMaxStepsM2, 0, wxALL, 5);

    wxStaticText* textUnitMaxSteps = new wxStaticText(parent, wxID_ANY, "шаг", wxDefaultPosition, wxDefaultSize, 0);
    textUnitMaxSteps->SetMinSize(wxSize(60, -1));
    sizerStepsPosition->Add(textUnitMaxSteps, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxCalc->Add(sizerStepsPosition, 1, wxGROW, 0);

    //Частота ШИМ
    auto sizerFreqPeriod = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* textFreq = new wxStaticText(parent, wxID_ANY, "Частота ШИМ", wxDefaultPosition, wxDefaultSize, 0);
    textFreq->SetMinSize(wxSize(100, -1));
    sizerFreqPeriod->Add(textFreq, 1, wxGROW | wxALL, 5);

    textReadOnlyFreqM2 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    wxString tFreq = wxString::Format("%03.0f", m2->Freq);
    textReadOnlyFreqM2->SetValue(tFreq);
    textReadOnlyFreqM2->SetMinSize(wxSize(60, -1));
    //spinRotationSpeed->SetValue(m2->ReductionRatioStepperMotor);
    sizerFreqPeriod->Add(textReadOnlyFreqM2, 0, wxALL, 5);

    wxStaticText* textUnitFreq = new wxStaticText(parent, wxID_ANY, "Гц", wxDefaultPosition, wxDefaultSize, 0);
    textUnitFreq->SetMinSize(wxSize(60, -1));
    sizerFreqPeriod->Add(textUnitFreq, 0, wxTOP | wxBOTTOM, 10);
    //sizerBoxCalc->Add(sizerFreqPeriod, 1, wxGROW, 0);

    //Период
    wxStaticText* textPeriod = new wxStaticText(parent, wxID_ANY, "Период ШИМ", wxDefaultPosition, wxDefaultSize, 0);
    textPeriod->SetMinSize(wxSize(100, -1));
    sizerFreqPeriod->Add(textPeriod, 1, wxGROW | wxALL, 5);

    textReadOnlyPeriodM2 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    wxString tPeriod = wxString::Format("%.6f", m2->Period);
    textReadOnlyPeriodM2->SetValue(tPeriod);
    textReadOnlyPeriodM2->SetMinSize(wxSize(60, -1));
    sizerFreqPeriod->Add(textReadOnlyPeriodM2, 0, wxALL, 5);

    wxStaticText* textUnitPeriod = new wxStaticText(parent, wxID_ANY, "сек", wxDefaultPosition, wxDefaultSize, 0);
    textUnitPeriod->SetMinSize(wxSize(60, -1));
    sizerFreqPeriod->Add(textUnitPeriod, 0, wxTOP | wxBOTTOM, 10);
    sizerBoxCalc->Add(sizerFreqPeriod, 1, wxGROW, 0);

    sizerBoxM2->Add(sizerBoxCalc, 1, wxGROW | wxALL, 5);

    return sizerBoxM2;
}

//====================================================================================================
//====================================================================================================
//====================================================================================================
void ControlOptionFrame::OnClose(wxCloseEvent& event)
{
    MainFrame* parent = (MainFrame*)GetParent();
    if (parent)
    {
        parent->frameControlOption = nullptr;
    }
    this->Destroy();
    event.Skip();
}
//====================================================================================================
void ControlOptionFrame::procMinDegreePositionM1(wxSpinEvent& event)
{
    m1->MinDegreePosition = spinMinDegreePositionM1->GetValue();
    m1->MinStepsPosition = TurretParamCalculate::DegreeToSteps(m1->MinDegreePosition, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);
    wxString tMinStepsPosition = wxString::Format("%d", m1->MinStepsPosition);
    textReadOnlyMinStepsM1->SetValue(tMinStepsPosition);
}
//====================================================================================================
void ControlOptionFrame::procMaxDegreePositionM1(wxSpinEvent& event)
{
    m1->MaxDegreePosition = spinMaxDegreePositionM1->GetValue();
    m1->MaxStepsPosition = TurretParamCalculate::DegreeToSteps(m1->MaxDegreePosition, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);
    wxString tMaxStepsPosition = wxString::Format("%d", m1->MaxStepsPosition);
    textReadOnlyMaxStepsM1->SetValue(tMaxStepsPosition);
}
//====================================================================================================
void ControlOptionFrame::procRotationSpeedM1(wxSpinDoubleEvent& event)
{
    m1->RotationSpeed = spinRotationSpeedM1->GetValue();
    m1->Freq = TurretParamCalculate::Freq(m1->RotationSpeed, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);
    m1->Period = TurretParamCalculate::PeriodSignal(m1->RotationSpeed, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);

    wxString tFreq = wxString::Format("%03.0f", m1->Freq);
    wxString tPeriod = wxString::Format("%.6f", m1->Period);
    textReadOnlyFreqM1->SetValue(tFreq);
    textReadOnlyPeriodM1->SetValue(tPeriod);
}
//====================================================================================================
void ControlOptionFrame::procMicroStepsStepperM1(wxSpinEvent& event)
{
    m1->MicroStepsStepperMotor = spinMicroStepsStepperM1->GetValue();
    m1->MinStepsPosition = TurretParamCalculate::DegreeToSteps(m1->MinDegreePosition, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);
    m1->MaxStepsPosition = TurretParamCalculate::DegreeToSteps(m1->MaxDegreePosition, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);
    m1->Freq = TurretParamCalculate::Freq(m1->RotationSpeed, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);
    m1->Period = TurretParamCalculate::PeriodSignal(m1->RotationSpeed, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);

    wxString tMinStepsPosition = wxString::Format("%d", m1->MinStepsPosition);
    wxString tMaxStepsPosition = wxString::Format("%d", m1->MaxStepsPosition);
    wxString tFreq = wxString::Format("%03.0f", m1->Freq);
    wxString tPeriod = wxString::Format("%.6f", m1->Period);

    textReadOnlyMinStepsM1->SetValue(tMinStepsPosition);
    textReadOnlyMaxStepsM1->SetValue(tMaxStepsPosition);
    textReadOnlyFreqM1->SetValue(tFreq);
    textReadOnlyPeriodM1->SetValue(tPeriod);
}
//====================================================================================================
void ControlOptionFrame::procReductionRatioM1(wxSpinDoubleEvent& event)
{
    m1->ReductionRatioStepperMotor = spinReductionRatioStepperM1->GetValue();
    m1->MinStepsPosition = TurretParamCalculate::DegreeToSteps(m1->MinDegreePosition, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);
    m1->MaxStepsPosition = TurretParamCalculate::DegreeToSteps(m1->MaxDegreePosition, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);
    m1->Freq = TurretParamCalculate::Freq(m1->RotationSpeed, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);
    m1->Period = TurretParamCalculate::PeriodSignal(m1->RotationSpeed, m1->MicroStepsStepperMotor, m1->ReductionRatioStepperMotor);

    wxString tMinStepsPosition = wxString::Format("%d", m1->MinStepsPosition);    
    wxString tMaxStepsPosition = wxString::Format("%d", m1->MaxStepsPosition);
    wxString tFreq = wxString::Format("%03.0f", m1->Freq);
    wxString tPeriod = wxString::Format("%.6f", m1->Period);

    textReadOnlyMinStepsM1->SetValue(tMinStepsPosition);
    textReadOnlyMaxStepsM1->SetValue(tMaxStepsPosition);
    textReadOnlyFreqM1->SetValue(tFreq);
    textReadOnlyPeriodM1->SetValue(tPeriod);
}
//====================================================================================================
void ControlOptionFrame::procMinDegreePositionM2(wxSpinEvent& event)
{
    m2->MinDegreePosition = spinMinDegreePositionM2->GetValue();
    m2->MinStepsPosition = TurretParamCalculate::DegreeToSteps(m2->MinDegreePosition, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);
    wxString tMinStepsPosition = wxString::Format("%d", m2->MinStepsPosition);
    textReadOnlyMinStepsM2->SetValue(tMinStepsPosition);
}
//====================================================================================================
void ControlOptionFrame::procMaxDegreePositionM2(wxSpinEvent& event)
{
    m2->MaxDegreePosition = spinMaxDegreePositionM2->GetValue();
    m2->MaxStepsPosition = TurretParamCalculate::DegreeToSteps(m2->MaxDegreePosition, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);
    wxString tMaxStepsPosition = wxString::Format("%d", m2->MaxStepsPosition);
    textReadOnlyMaxStepsM2->SetValue(tMaxStepsPosition);
}
//====================================================================================================
void ControlOptionFrame::procRotationSpeedM2(wxSpinDoubleEvent& event)
{
    m2->RotationSpeed = spinRotationSpeedM2->GetValue();
    m2->Freq = TurretParamCalculate::Freq(m2->RotationSpeed, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);
    m2->Period = TurretParamCalculate::PeriodSignal(m2->RotationSpeed, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);

    wxString tFreq = wxString::Format("%03.0f", m2->Freq);
    wxString tPeriod = wxString::Format("%.6f", m2->Period);
    textReadOnlyFreqM2->SetValue(tFreq);
    textReadOnlyPeriodM2->SetValue(tPeriod);
}
//====================================================================================================
void ControlOptionFrame::procMicroStepsStepperM2(wxSpinEvent& event)
{
    m2->MicroStepsStepperMotor = spinMicroStepsStepperM2->GetValue();
    m2->MinStepsPosition = TurretParamCalculate::DegreeToSteps(m2->MinDegreePosition, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);
    m2->MaxStepsPosition = TurretParamCalculate::DegreeToSteps(m2->MaxDegreePosition, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);
    m2->Freq = TurretParamCalculate::Freq(m2->RotationSpeed, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);
    m2->Period = TurretParamCalculate::PeriodSignal(m2->RotationSpeed, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);

    wxString tFreq = wxString::Format("%03.0f", m2->Freq);
    wxString tPeriod = wxString::Format("%.6f", m2->Period);
    wxString tMinStepsPosition = wxString::Format("%d", m2->MinStepsPosition);
    wxString tMaxStepsPosition = wxString::Format("%d", m2->MaxStepsPosition);

    textReadOnlyMinStepsM2->SetValue(tMinStepsPosition);
    textReadOnlyMaxStepsM2->SetValue(tMaxStepsPosition);
    textReadOnlyFreqM2->SetValue(tFreq);
    textReadOnlyPeriodM2->SetValue(tPeriod);
}
//====================================================================================================
void ControlOptionFrame::procReductionRatioM2(wxSpinDoubleEvent& event)
{
    m2->ReductionRatioStepperMotor = spinReductionRatioStepperM2->GetValue();
    m2->MinStepsPosition = TurretParamCalculate::DegreeToSteps(m2->MinDegreePosition, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);
    m2->MaxStepsPosition = TurretParamCalculate::DegreeToSteps(m2->MaxDegreePosition, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);
    m2->Freq = TurretParamCalculate::Freq(m2->RotationSpeed, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);
    m2->Period = TurretParamCalculate::PeriodSignal(m2->RotationSpeed, m2->MicroStepsStepperMotor, m2->ReductionRatioStepperMotor);

    wxString tFreq = wxString::Format("%03.0f", m2->Freq);
    wxString tPeriod = wxString::Format("%.6f", m2->Period);
    wxString tMinStepsPosition = wxString::Format("%d", m2->MinStepsPosition);
    wxString tMaxStepsPosition = wxString::Format("%d", m2->MaxStepsPosition);
    
    textReadOnlyMinStepsM2->SetValue(tMinStepsPosition);   
    textReadOnlyMaxStepsM2->SetValue(tMaxStepsPosition);
    textReadOnlyFreqM2->SetValue(tFreq);
    textReadOnlyPeriodM2->SetValue(tPeriod);

}
//====================================================================================================
void ControlOptionFrame::procResetPosition(wxCommandEvent& event)
{
    MainFrame* parent = (MainFrame*)GetParent();
}
//====================================================================================================
void ControlOptionFrame::procNpLimit(wxCommandEvent& event)
{
    MainFrame* parent = (MainFrame*)GetParent();
}
//====================================================================================================
void ControlOptionFrame::procMouse(wxCommandEvent& event)
{
    mouse->Sensitivity = static_cast<float>(sliderMouse->GetValue()) / 100.0;

    wxString tSens = wxString::Format("%1.2f", mouse->Sensitivity);
    textReadOnlyMouse->SetValue(tSens);
}
//====================================================================================================