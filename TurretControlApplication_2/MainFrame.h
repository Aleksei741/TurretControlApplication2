#pragma once
#ifndef MAIN_FRAME_WND
#define MAIN_FRAME_WND

#include "main.h"
#include "VideoStreamProc.h"
#include "ParametersPrc.h"
#include "VideoOptionFrame.h"
#include "ControlOptionFrame.h"
#include "AimOptionFrame.h"
#include "DamageOptionFrame.h"

#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/frame.h>
#include <wx/statline.h>
#include <wx/tglbtn.h>
#include <wx/mediactrl.h>

class VideoStreamProc;
class VideoOptionFrame;
class ControlOptionFrame;
class DamageOptionFrame;
class AimOptionFrame;


class MainFrame: public wxFrame
{
	wxComboBox* combobxAvailableTurrets;
	wxButton* buttonConnect;
	wxButton* buttonDisconnect;
	wxStaticText* textHP;
	wxStaticText* textMaxHP;
	wxGauge* gaugeHP;
	wxButton* buttonResetHP;
	wxToggleButton* toggleBtnMouse;
	wxToggleButton* toggleBtnKeyboard;
	wxStaticText* textSourceControl;
	wxPanel* panelVideo;
	wxMediaCtrl* animationPositionUP;
	wxMediaCtrl* animationPositionDOWN;
	wxMediaCtrl* animationPositionLEFT;
	wxMediaCtrl* animationPositionRIGHT;
	wxTextCtrl* textPositionX;
	wxTextCtrl* textPositionY;

	VideoStreamProc* video = nullptr;
	ParametersPrc ParamProc;
	

	const int Border = FromDIP(5);

	int flagClearVideoPanel;

	void DrawingMainWindow();
	void CreateMenu();

	void OnResize(wxSizeEvent& event);
	void ButConnect(wxCommandEvent& event);
	void ButDisconnect(wxCommandEvent& event);
	void ButMouse(wxCommandEvent& event);
	void ButKeyboard(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnDestroy(wxWindowDestroyEvent& event);

	void ButtonEventVideoOption(wxCommandEvent& WXUNUSED(event));
	void ButtonEventControlOption(wxCommandEvent& WXUNUSED(event));
	void ButtonEventAimOption(wxCommandEvent& WXUNUSED(event));
	void ButtonEventDamageOption(wxCommandEvent& WXUNUSED(event));
	void ButtonEventSaveConfig(wxCommandEvent& WXUNUSED(event));
	void ButtonEventLoadConfig(wxCommandEvent& WXUNUSED(event));
	void ButtonEventSaveTurret(wxCommandEvent& WXUNUSED(event));
	void ButtonEventLoadTurret(wxCommandEvent& WXUNUSED(event));

	void AimDraw(wxMemoryDC &mdc, int width, int height);

	wxDECLARE_EVENT_TABLE();

public:
	VideoOptionFrame* frameVideoOption = nullptr;
	ControlOptionFrame* frameControlOption = nullptr;
	AimOptionFrame* frameAimOption = nullptr;
	DamageOptionFrame* frameDamageOption = nullptr;

	UserParameters_DType param;

	MainFrame(const wxString & title, const wxPoint & pos, const wxSize & size);
	void InitVideo(void);
	void SizeImgWindowCallback(int* w, int* h);
	void DrawImgWindowCallback(unsigned char* RGB, int height, int width, int linesize);	

};
#endif