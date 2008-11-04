/////////////////////////////////////////////////////////////////////////////
// Name:        statbar.h
// Purpose:     a statusbar with a additional button 
// Author:      Joachim Buermann
// Id:          $Id: statbar.h,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "wx/wx.h"
#include <wx/awx-0.4/led.h>

class MyStatusBar : public wxStatusBar
{
protected:
    bool m_blink;
    wxButton* m_button;
    awxLed* m_led;
    void OnCancel(wxCommandEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnSize(wxSizeEvent& event);
    void Resize();
public:
    MyStatusBar(wxWindow* parent);
    virtual ~MyStatusBar();
    void SetBlink(bool blink);
    DECLARE_EVENT_TABLE()
};
