#pragma once

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
#include <wx/clrpicker.h>
#include <wx/fontpicker.h>

class MainFrame;

class AimOptionFrame : public wxFrame
{
	AimOption_DType* aim;

	wxSpinCtrl* spinOptionX;
	wxSpinCtrl* spinOptionY;
	wxSpinCtrl* spinOptionWidth;
	wxSpinCtrl* spinOptionHeight;
	wxSpinCtrl* spinOptionPixelLine;
	wxColourPickerCtrl* buttonColor;
	wxFontPickerCtrl* buttonFont;

	wxCheckBox* checkBoxText1Active;
	wxSpinCtrl* spinOptionText1LenLine;
	wxSpinCtrl* spinOptionText1Offset;
	wxTextCtrl* textOptionText1Text;

	wxCheckBox* checkBoxText2Active;
	wxSpinCtrl* spinOptionText2LenLine;
	wxSpinCtrl* spinOptionText2Offset;
	wxTextCtrl* textOptionText2Text;

	wxCheckBox* checkBoxText3Active;
	wxSpinCtrl* spinOptionText3LenLine;
	wxSpinCtrl* spinOptionText3Offset;
	wxTextCtrl* textOptionText3Text;

	wxDECLARE_EVENT_TABLE();

	void OnClose(wxCloseEvent& event);
	void procButtonFont(wxFontPickerEvent& event);
	void procButtonColor(wxColourPickerEvent& event);
	void procSpinX(wxSpinEvent& event);
	void procSpinY(wxSpinEvent& event);
	void procSpinWidth(wxSpinEvent& event);
	void procSpinHeight(wxSpinEvent& event);
	void procSpinPixelLine(wxSpinEvent& event);

	void procCheckBoxText1Active(wxCommandEvent& event);
	void procSpinText1Offset(wxSpinEvent& event);
	void procSpinText1Len(wxSpinEvent& event);
	void procTextBoxText1Text(wxCommandEvent& event);

	void procCheckBoxText2Active(wxCommandEvent& event);
	void procSpinText2Offset(wxSpinEvent& event);
	void procSpinText2Len(wxSpinEvent& event);
	void procTextBoxText2Text(wxCommandEvent& event);

	void procCheckBoxText3Active(wxCommandEvent& event);
	void procSpinText3Offset(wxSpinEvent& event);
	void procSpinText3Len(wxSpinEvent& event);
	void procTextBoxText3Text(wxCommandEvent& event);

public:
	AimOptionFrame(wxWindow* parent, void* aim_);
	void UpdateValue(void);
};

