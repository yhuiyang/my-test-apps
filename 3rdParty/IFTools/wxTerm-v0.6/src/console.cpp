/////////////////////////////////////////////////////////////////////////////
// Name:        console.cpp
// Purpose:     
// Author:      Joachim Buermann
// Id:          $Id: console.cpp,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "console.h"

IMPLEMENT_DYNAMIC_CLASS( wxConsoleEvent, wxEvent )
DEFINE_EVENT_TYPE( wxEVT_CONSOLE_CHAR )
DEFINE_EVENT_TYPE( wxEVT_CONSOLE_LINE )

wxConsoleEvent::wxConsoleEvent(wxWindow* win)
{
    SetEventType(wxEVT_CONSOLE_CHAR);
    SetEventObject(win);
};

BEGIN_EVENT_TABLE(Console, wxTextCtrl)
    EVT_CHAR(Console::OnChar)
#ifdef __WXGTK20__
    EVT_KEY_UP(Console::OnKeyUp)
#endif
END_EVENT_TABLE()

Console::Console(wxWindow* parent,
			  wxWindowID id,
			  const wxString &value,
			  const wxPoint &pos,
			  const wxSize &size,
			  int style) :
    wxTextCtrl(parent,id,value,pos,size,style)
{
    m_raw = false;
};

//
// Unter gtk2 loesen nur Spezialtasten wie Enter, Shift etc.
// ein EVT_CHAR aus. Die normalen Ascii Tasten koennen weder
// mit EVT_CHAR noch mit EVT_KEY_DOWN abgefangen werden.
// Dies Funktioniert auch nicht, wenn wxTextCtrl bzw. davon
// abgeleitete Klassen Teil eines wxPanel sind.
//
void Console::OnChar(wxKeyEvent& event)
{
    wxConsoleEvent ev;
    if(m_raw) {
#ifndef __WXGTK20__
	   ev.SetEventType(wxEVT_CONSOLE_CHAR);
	   ev.SetChar(event.KeyCode());
	   GetGrandParent()->GetEventHandler()->ProcessEvent(ev);
#endif
    }
    else {
	   if(event.GetKeyCode() == WXK_RETURN) {
		  long x,y;
		  PositionToXY(GetInsertionPoint(),&x,&y);
		  ev.SetLine(GetLineText(y));
		  ev.SetEventType(wxEVT_CONSOLE_LINE);
		  GetGrandParent()->GetEventHandler()->ProcessEvent(ev);
	   }
    }
    event.Skip();
};

//
// WORKAROUND!!! Um einen raw Modus zu implementieren, der
// jedes einzelne Zeichen behandelt, muss EVT_KEY_UP abgefangen
// werden. Besser waere EVT_KEY_DOWN, dies funktioniert allerdings
// weder mit EVT_KEY_DOWN noch mit EVT_CHAR!!!
//
void Console::OnKeyUp(wxKeyEvent& event)
{
    wxConsoleEvent ev;
    if(m_raw) {
	   ev.SetEventType(wxEVT_CONSOLE_CHAR);
	   ev.SetChar(event.GetKeyCode());
	   GetGrandParent()->GetEventHandler()->ProcessEvent(ev);
    }
    event.Skip();
};
