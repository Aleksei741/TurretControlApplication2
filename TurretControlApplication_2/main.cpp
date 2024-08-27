#include "wx/wxprec.h"

#include "gun.xpm"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "main.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(TurretControlApp);

bool TurretControlApp::OnInit()
{
    MainFrame* frame = new MainFrame("Turret Control", wxDefaultPosition, wxDefaultSize);
    SetTopWindow(frame);
    frame->Show(true);
    frame->SetFocus();

    return true;
}