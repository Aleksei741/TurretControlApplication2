#pragma once
#ifndef VIDEO_OPTION_FRAME_WND
#define VIDEO_OPTION_FRAME_WND

#include "main.h"
#include "MainFrame.h"

#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/frame.h>
#include <wx/statline.h>
#include <wx/tglbtn.h>
#include <wx/mediactrl.h>
#include <wx/spinctrl.h>


class VideoOptionFrame : public wxFrame
{
	VideoOption_DType* VideoParam;

	wxComboBox* combobxVideoResolution;
	wxSpinCtrl* spinOptionBitrate;
	wxSpinCtrl* spinOptionFramerate;
	wxComboBox* combobxVideoH26X;
	wxSpinCtrl* spinOptionVideoPort;

	void OnClose(wxCloseEvent& event);
	void procVideoResolution(wxCommandEvent& event);
	void procBitrate(wxSpinEvent& event);
	void procFramerate(wxSpinEvent& event);
	void procCodec(wxCommandEvent& event);
	void procPort(wxSpinEvent& event);

	wxDECLARE_EVENT_TABLE();

public:
	VideoOptionFrame(wxWindow* parent, void* VideoParam);
};

#endif

