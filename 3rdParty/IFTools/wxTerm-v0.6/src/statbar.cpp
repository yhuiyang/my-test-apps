/////////////////////////////////////////////////////////////////////////////
// Name:        statbar.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: statbar.cpp,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "statbar.h"

BEGIN_EVENT_TABLE(MyStatusBar, wxStatusBar)
    EVT_ERASE_BACKGROUND(MyStatusBar::OnEraseBackground)
    EVT_SIZE(MyStatusBar::OnSize)
    EVT_BUTTON(wxID_CANCEL, MyStatusBar::OnCancel)
END_EVENT_TABLE()

MyStatusBar::MyStatusBar(wxWindow* parent) :
    wxStatusBar(parent,-1)
{
    m_blink = false;
    int widths[8] = {50,-2,100,-1,-1,80,50};
    SetFieldsCount(8);
    SetStatusWidths(8,widths);

    m_button = new wxButton(this,wxID_CANCEL,_("Start Log"),
					   wxDefaultPosition,wxSize(80,20));
    m_led = new awxLed(this,-1);
    SetMinHeight(20);
};

MyStatusBar::~MyStatusBar()
{
};

void MyStatusBar::OnCancel(wxCommandEvent& event)
{
    static int logging = 0;
    logging ^= 1;
    if(logging) m_button->SetLabel(_("Stop Log"));
    else m_button->SetLabel(_("Start Log"));
    SetBlink(logging);
};

void MyStatusBar::OnEraseBackground(wxEraseEvent& event)
{
    Resize();
    event.Skip();
};

void MyStatusBar::OnSize(wxSizeEvent& event)
{
    Resize();
    event.Skip();
};

void MyStatusBar::Resize()
{
    wxRect rect;

    GetFieldRect(5,rect);
    m_button->SetSize(rect.x,rect.y,rect.width,rect.height);

    GetFieldRect(6,rect);
    m_led->SetSize(rect.x,rect.y,rect.width,rect.height);
};

void MyStatusBar::SetBlink(bool blink)
{
    if(blink) m_led->SetState(awxLED_BLINK);
    else m_led->SetState(awxLED_OFF);
};
