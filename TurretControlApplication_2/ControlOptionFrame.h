#pragma once
#ifndef CONTROL_OPTION_FRAME_WND
#define CONTROL_OPTION_FRAME_WND

#include "main.h"
#include "MainFrame.h"
#include "TurretParamCalculate.h"

#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/frame.h>
#include <wx/statline.h>
#include <wx/tglbtn.h>
#include <wx/mediactrl.h>
#include <wx/spinctrl.h>

class ControlOptionFrame : public wxFrame
{
	MotorOption_DType* m1;
	MotorOption_DType* m2;
	MouseOptoin_DType* mouse;

	wxSpinCtrl* spinMinDegreePositionM1;
	wxSpinCtrl* spinMaxDegreePositionM1;
	wxSpinCtrlDouble* spinRotationSpeedM1;
	wxSpinCtrl* spinMicroStepsStepperM1;
	wxSpinCtrlDouble* spinReductionRatioStepperM1;
	wxTextCtrl* textReadOnlyFreqM1;
	wxTextCtrl* textReadOnlyPeriodM1;
	wxTextCtrl* textReadOnlyMinStepsM1;
	wxTextCtrl* textReadOnlyMaxStepsM1;

	wxSpinCtrl* spinMinDegreePositionM2;
	wxSpinCtrl* spinMaxDegreePositionM2;
	wxSpinCtrlDouble* spinRotationSpeedM2;
	wxSpinCtrl* spinMicroStepsStepperM2;
	wxSpinCtrlDouble* spinReductionRatioStepperM2;
	wxTextCtrl* textReadOnlyFreqM2;
	wxTextCtrl* textReadOnlyPeriodM2;
	wxTextCtrl* textReadOnlyMinStepsM2;
	wxTextCtrl* textReadOnlyMaxStepsM2;

	wxButton* buttonResetPosition;
	wxCheckBox* checkBoxNoLimit;
	wxTextCtrl* textReadOnlyMouse;
	wxSlider* sliderMouse;

	wxStaticBoxSizer* DrawMotor1Param(void* parent);
	wxStaticBoxSizer* DrawMotor2Param(void* parent);

	wxDECLARE_EVENT_TABLE();

	void OnClose(wxCloseEvent& event);
	void procMinDegreePositionM1(wxSpinEvent& event);
	void procMaxDegreePositionM1(wxSpinEvent& event);
	void procRotationSpeedM1(wxSpinDoubleEvent& event);
	void procMicroStepsStepperM1(wxSpinEvent& event);
	void procReductionRatioM1(wxSpinDoubleEvent& event);
	void procMinDegreePositionM2(wxSpinEvent& event);
	void procMaxDegreePositionM2(wxSpinEvent& event);
	void procRotationSpeedM2(wxSpinDoubleEvent& event);
	void procMicroStepsStepperM2(wxSpinEvent& event);
	void procReductionRatioM2(wxSpinDoubleEvent& event);
	void procResetPosition(wxCommandEvent& event);
	void procNpLimit(wxCommandEvent& event);
	void procMouse(wxCommandEvent& event);

public:
	ControlOptionFrame(wxWindow* parent, void* m1_, void* m2_, void* mouse_);
};

#endif