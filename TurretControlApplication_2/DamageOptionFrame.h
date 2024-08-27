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

class DamageOptionFrame : public wxFrame
{
	DamageOption_DType* dmg;

	wxSpinCtrl* spinOptionHP;
	wxSpinCtrl* spinOptionDamageMinute;
	wxSpinCtrl* spinOptionDamageSecunde;
	wxCheckBox* checkBoxMovementOff;
	wxCheckBox* checkBoxWeaponOff;
	wxCheckBox* checkBoxVideoOff;
	wxSpinCtrl* spinOptionDelaySensor;

	wxDECLARE_EVENT_TABLE();

	void OnClose(wxCloseEvent& event);
	void procSpinHP(wxSpinEvent& event);
	void procSpinDelayDamage_min(wxSpinEvent& event);
	void procSpinDelayDamage_sec(wxSpinEvent& event);
	void procSpinDelaySensor_ms(wxSpinEvent& event);
	void procCheckBoxMovementOff(wxCommandEvent& event);
	void procCheckBoxWeaponOff(wxCommandEvent& event);
	void procCheckBoxVideoOff(wxCommandEvent& event);

public:
	DamageOptionFrame(wxWindow* parent, void* dmg_);
};

