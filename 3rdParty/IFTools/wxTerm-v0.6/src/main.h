/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     
// Author:      Joachim Buermann
// Id:          $Id: main.h,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#if !defined ( __MAIN_H )
#define __MAIN_H

#include <wx/awx-0.4/awx.h>
#include "wx/ctb-0.13/serport.h"
#include "wx/ctb-0.13/timer.h"
#include "dbgprint.h"
#include "wx/wxhtml.h"
#include "wx/notebook.h"
#include "wx/wxprec.h"
#include "wx/splitter.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "console.h"
#include "dataview.h"
#include "menu.h"
#include "modbase.h"
#include "prefs.h"
#include "statbar.h"

////////////////////////////////////////////////////////////
// Application
class MyApp : public wxApp
{
protected:
    wxLocale m_locale;
    wxString GetAppPath(char* argv0);
public:
    virtual bool OnInit();
};

////////////////////////////////////////////////////////////
// Application Frame
class MyFrame : public wxFrame
{
protected:
//////////////////////////////////////////////////
//  not public data members
    // existence of GPIB
    bool m_gpib_exist;
    // Toolbar
    awxToolbar* m_tb;
    // output window
    wxDView* m_dump;
    // input window
    Console *m_input;
    // splitter window
    wxSplitterWindow* m_split;
    // statusline
    MyStatusBar *m_sb;
    // ticker
    wxTimer* m_ticker;
    
    enum {
	   VIEW_AS_ASCII = 0,
	   VIEW_AS_HEX
    };

    FILE* m_logfd;
    int m_logcounter;
    int m_rcounter;

    wxModuleBase* m_modules[2];
    wxModuleBase* m_module;
    // Online Help Controller
    wxHtmlHelpController* m_help;
protected:
//////////////////////////////////////////////////
//  not public methodes
    // Adjust the serial comport (port, baudrate)
    // returns 0 if ok (no error)
    bool CheckForGPIB();
    int ClearCachedFiles(wxString dir,wxString pattern);
    int Connect(prefs_t& prefs);
    int Disconnect();
    void OnAbout(wxCommandEvent& event);
    void OnBaudrateChanged(wxCommandEvent& event);
    void OnBreak(wxCommandEvent& event) {
	   if(m_module->IsOpen()) m_module->ResetDevice();
    };
    void OnCommandLine(wxConsoleEvent& event);
    void OnConnect(wxCommandEvent& event);
    void OnConnectOnOff(wxCommandEvent& event);
    void OnDisconnect(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnRecord(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSetInputMode(wxCommandEvent& event) {
	   m_input->SetRawMode(event.GetInt() == 2);
	   Update();
    };
    void OnToolbarText(wxCommandEvent& event);
    int SaveFile(wxString file);
    int SendFile(wxString file);
    void ShowLinestate(bool force = false) {
	   m_module->ShowState(force);
    };
    int StartLogging();
    int StopLogging();
    void Update();
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MyFrame();
    void OnClose(wxCloseEvent& event);
    void OnEmpty(wxCommandEvent& event){
	   m_dump->Clear();
	   m_dump->Redraw();
	   m_rcounter = 0;
    };
    void OnQuit(wxCommandEvent& event);
    void OnIdle(wxUpdateUIEvent& event);
    void OnPageChanged(wxNotebookEvent& event);
    void OnPageChanging(wxNotebookEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnToggleDTR(wxCommandEvent& event);
    void OnToggleRTS(wxCommandEvent& event);
    void OnToolbar(wxCommandEvent& event);
protected:
    DECLARE_EVENT_TABLE()
};

//----------------------------------------------------------------------------
// Ticker
//----------------------------------------------------------------------------

class Ticker : public wxTimer
{
protected:
    wxFrame *frame;
public:
    Ticker(wxFrame *frame_) : wxTimer() {
	   frame = frame_;
    };
    void Notify() {
	   wxUpdateUIEvent ev(ID_TICKER);
	   frame->GetEventHandler()->ProcessEvent(ev);
    };
};

#endif
