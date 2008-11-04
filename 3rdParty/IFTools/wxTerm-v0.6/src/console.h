/////////////////////////////////////////////////////////////////////////////
// Name:        console.h
// Purpose:     
// Author:      Joachim Buermann
// Id:          $Id: console.h,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#if !defined ( __CONSOLE_H )
#define __CONSOLE_H

#include "wx/wx.h"

class wxConsoleEvent : public wxEvent
{
    DECLARE_DYNAMIC_CLASS( wxConsoleEvent ) 
public:
    wxConsoleEvent(wxWindow* win = (wxWindow*)NULL);
    wxEvent* Clone(void) const {return new wxConsoleEvent(*this); };
    char GetChar() {return m_char;}
    wxString& GetLine() {return m_line;};
    void SetChar(char ch) {m_char = ch;};
    void SetLine(wxString line) {m_line = line;};
private:
    char m_char;
    wxString m_line;
};

typedef void (wxEvtHandler::*wxConsoleEventFunction)(wxConsoleEvent&);

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_CONSOLE_CHAR, 1)
    DECLARE_EVENT_TYPE(wxEVT_CONSOLE_LINE, 2)
END_DECLARE_EVENT_TYPES()

#define EVT_CONSOLE_CHAR(func) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_CONSOLE_CHAR, \
                              -1, \
                              -1, \
                              (wxObjectEventFunction) \
                              (wxConsoleEventFunction) & func, \
                              (wxObject *) NULL ),

#define EVT_CONSOLE_LINE(func) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_CONSOLE_LINE, \
                              -1, \
                              -1, \
                              (wxObjectEventFunction) \
                              (wxConsoleEventFunction) & func, \
                              (wxObject *) NULL ),

class Console : public wxTextCtrl
{
public:
    Console(wxWindow* parent,
	    wxWindowID id,
	    const wxString &value,
	    const wxPoint &pos,
	    const wxSize &size,
	    int style);
    void OnChar(wxKeyEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void SetRawMode(bool enable) {
	   m_raw = enable;
    };
private:
//////////////////////////////////////////////////
// private data members
    bool m_raw;
    wxFont* m_font;
    DECLARE_EVENT_TABLE()
};    

#endif




