/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _PWUPDATER_H_
#define _PWUPDATER_H_

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/aui/framemanager.h>
#include <wx/vector.h>

class TftpdServerThread;
class TftpdTransmissionThread;

// ========================================================================
// Application class
// ========================================================================
class PWUpdaterApp : public wxApp
{
public:
    PWUpdaterApp() { Init(); }
    ~PWUpdaterApp() {}

    /* tftpd server thread managment */
    TftpdServerThread *m_pTftpdServerThread;
    wxCriticalSection m_serverCS;

    /* tftpd transmission threads management */
    wxVector<TftpdTransmissionThread *> m_tftpdTransmissionThreads;
    wxCriticalSection m_transmissionCS;

private:
    void Init();
    virtual bool OnInit();
};

DECLARE_APP(PWUpdaterApp)

// ========================================================================
// GUI (main frame) class
// ========================================================================
class PWUpdaterFrame : public wxFrame
{
public:
    PWUpdaterFrame() { Init(); }
    PWUpdaterFrame(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString &caption = wxT("PWUpdater"),
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);
    ~PWUpdaterFrame();
    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxString &caption = wxT("PWUpdater"),
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);

private:
    /* helper functions */
    void Init();
    void CreateControls();
    
    /* event handler functions */
    void OnClose(wxCloseEvent &event);
    void OnQuit(wxCommandEvent &event);

    /* data members */
    wxAuiManager _auiMgr;

    DECLARE_EVENT_TABLE()
};

#endif /* _PWUPDATER_H_ */

