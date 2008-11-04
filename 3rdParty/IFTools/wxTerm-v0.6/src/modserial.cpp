/////////////////////////////////////////////////////////////////////////////
// Name:        modserial.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: modserial.cpp,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "dbgprint.h"
#include "menu.h"
#include "modserial.h"
#include <wx/awx-0.4/led.h>
#include <wx/ctb-0.13/serport.h>
#ifdef __WIN32__
# include <wx/msw/winundef.h>
#endif

wxModuleRS232::~wxModuleRS232()
{
    int i = 0;
    for(i=0;i<6;i++) {
	   delete m_led[i];
    }
};

void wxModuleRS232::EnableChildren(bool enable)
{
    wxWindow* widget = FindWindowById(ID_SWITCHBAUD);
    if(widget) {
	   widget->Enable(enable);
    }
};

int wxModuleRS232::GetEOSSelection()
{
    wxChoice* widget = (wxChoice*)FindWindow(ID_EOS);
    return widget->GetSelection();
};

wxString wxModuleRS232::GetEOSString()
{
    const char* eos[5] = {"\n","\r","\r\n","\n\r",""};
    return wxString(eos[GetEOSSelection()]);
};

int wxModuleRS232::GetInputMode()
{
    wxChoice* widget = (wxChoice*)FindWindow(ID_IMODE);
    return widget->GetSelection();
};

void wxModuleRS232::InitBase()
{
    int i = 0;
    m_rts = m_dtr = false;
    wxWindow* widget;
    m_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* boxleft = new wxStaticBoxSizer(
	   new wxStaticBox(this,-1,_("Status")),
	   wxHORIZONTAL);
    wxStaticBoxSizer* boxmiddle = new wxStaticBoxSizer(
	   new wxStaticBox(this,-1,_("Baudrate")),
	   wxHORIZONTAL);
    wxStaticBoxSizer* boxright = new wxStaticBoxSizer(
	   new wxStaticBox(this,-1,_("Input")),
	   wxHORIZONTAL);
    wxGridSizer* bl = new wxGridSizer(6,0,0);
    wxFlexGridSizer* br = new wxFlexGridSizer(2,2,2,10);

    // RTS and DTR can be switched on/off
    widget = new wxButton(this,ID_TOGGLE_RTS,"RTS",
 					 wxDefaultPosition,wxSize(40,-1));
    widget->SetToolTip(_("Toggle RTS linestate (Ctrl-r)"));
    bl->Add(widget,0,wxALIGN_CENTER,0);
    widget->SetToolTip(_("Toggle RTS linestate (Ctrl-r)"));
    widget = new wxButton(this,ID_TOGGLE_DTR,"DTR",
					 wxDefaultPosition,wxSize(40,-1));
    widget->SetToolTip(_("Toggle DTR linestate (Ctrl-d)"));
    bl->Add(widget,0,wxALIGN_CENTER,0);
    const char* labels[6] = {"RTS","DTR","CTS","DSR","DCD"," RI"};
    for(i=LED_CTS;i<=LED_RI;i++) {
	   bl->Add(new wxStaticText(this,-1,labels[i],
						   wxDefaultPosition,
						   wxSize(30,-1),
						   wxST_NO_AUTORESIZE|wxALIGN_CENTRE),
			 0,wxALIGN_CENTER,0);
    }
    for(i=LED_RTS;i<=LED_RI;i++) {
	   m_led[i] = new awxLed(this,-1,wxDefaultPosition);
	   bl->Add(m_led[i],0,wxALIGN_CENTER,0);
    }
    boxleft->Add(bl,0,wxLEFT,0);
    m_sizer->Add(boxleft,0,wxLEFT,0);

    // Baudrate Quick Switch
    wxString baudrates[15] = {
	   wxT("921600"),
        wxT("460800"), 
        wxT("345600"), 
        wxT("230400"), 
        wxT("115200"), 
        wxT("57600"), 
        wxT("38400"), 
        wxT("19200"), 
        wxT("9600"), 
        wxT("4800"), 
        wxT("2400"), 
        wxT("1200"),
        wxT("600"),
        wxT("300"),
        wxT("150")
    };
    
    widget = new wxChoice(this,ID_SWITCHBAUD,
					 wxDefaultPosition,wxDefaultSize,
					 14,baudrates,
					 wxNO_FULL_REPAINT_ON_RESIZE);
    boxmiddle->Add(widget,0,wxLEFT,0); 
    m_sizer->Add(boxmiddle,0,wxLEFT,0);
    // EOS (default is lf)
    wxString eossym[5] = {"<lf>","<cr>","<crlf>","<lfcr>","<none"};
    br->Add(new wxStaticText(this,-1,_("EOS is")),0,wxALIGN_CENTRE,0);
    widget = new wxChoice(this,ID_EOS,
					 wxDefaultPosition,wxSize(100,-1),
					 5,eossym,
					 wxNO_FULL_REPAINT_ON_RESIZE);
    widget->SetToolTip(
	   _("The EOS (end of string) follows every line, you send "
		"in cooked mode. And it force a linebreak in the "
		"output window."));
    br->Add(widget,0,wxRIGHT,0);
    wxChoice* choice_eos = (wxChoice*)widget;

    // Input Mode (cooked, raw - default is cooked)
    br->Add(new wxStaticText(this,-1,_("Input is")),0,wxALIGN_CENTRE,0);
    wxString imode[] = {"cooked","cooked hex","raw"};
    widget = new wxChoice(this,ID_IMODE,
					 wxDefaultPosition,wxSize(100,-1),3,imode,
					 wxNO_FULL_REPAINT_ON_RESIZE); 
    widget->SetToolTip(_("In cooked mode, the inputed chars of a line will be "
					"send after pressing Return. In raw mode, all chars "
					"will be send immediatelly"));
    br->Add(widget,0,wxRIGHT,0);
    wxChoice* choice_term = (wxChoice*)widget;
    m_sizer->Add(10,0,wxEXPAND|wxALL,1);
    boxright->Add(br,0,wxRIGHT,0);
    m_sizer->Add(boxright,0,wxRIGHT,0);

    SetSizerAndFit(m_sizer);

    EnableChildren(m_dev.IsOpen());
};

int wxModuleRS232::ReadDevice(char* buf,size_t len)
{
    int rd = m_dev.Read(buf,len);
    return rd;
};

void wxModuleRS232::ResetDevice()
{
    if(m_dev.IsOpen()) m_dev.SendBreak(0);
};

bool wxModuleRS232::SetBaudrate(wxBaud baudrate)
{
    if(m_dev.IsOpen()) {
	   return m_dev.SetBaudRate(baudrate) == 0;
    }
    return false;
};

void wxModuleRS232::SetEOSSelection(int selection)
{
    wxChoice* widget = (wxChoice*)FindWindow(ID_EOS);
    widget->SetSelection(selection);
};

void wxModuleRS232::SetEOSString(const wxString& string)
{
    wxChoice* widget = (wxChoice*)FindWindow(ID_EOS);
    widget->SetStringSelection(string);
};

void wxModuleRS232::SetDTR(bool state) 
{
    if(!state) m_dev.ClrLineState(wxSERIAL_LINESTATE_DTR);
    else m_dev.SetLineState(wxSERIAL_LINESTATE_DTR);
    // windows doesn't allow to query the state of
    // RTS and DTR, so wie must notice the state of both
    m_dtr = state;
    ShowState(true);
};

void wxModuleRS232::SetInputMode(int mode)
{
    wxChoice* widget = (wxChoice*)FindWindow(ID_IMODE);
    widget->SetSelection(mode);
};

void wxModuleRS232::SetRTS(bool state)
{
    if(!state) m_dev.ClrLineState(wxSERIAL_LINESTATE_RTS);
    else m_dev.SetLineState(wxSERIAL_LINESTATE_RTS);
    // windows doesn't allow to query the state of
    // RTS and DTR, so wie must notice the state of both
    m_rts = state;
    ShowState(true);
};

void wxModuleRS232::ShowState(bool force)
{
    static int last_flags = 0;
    if(!m_dev.IsOpen()) {
	   for(int i=LED_RTS;i<=LED_RI;i++) {
		  m_led[i]->SetState(awxLED_OFF);
	   }
    }
    else {
	   int flags = m_dev.GetLineState();
	   if(flags != -1) {
		  if((flags == last_flags) && !force) {
			 return;
		  }
		  last_flags = flags;
		  // now set LEDs, which line state is active (red)
		  // RTS
		  if((flags & wxSERIAL_LINESTATE_RTS) || m_rts) {
			 m_led[LED_RTS]->SetColour(awxLED_RED);
		  }
		  else { 
			 m_led[LED_RTS]->SetColour(awxLED_GREEN);
		  }
		  // DTR
		  if((flags & wxSERIAL_LINESTATE_DTR) || m_dtr) {
			 m_led[LED_DTR]->SetColour(awxLED_RED);
		  }
		  else { 
			 m_led[LED_DTR]->SetColour(awxLED_GREEN);
		  }
		  // CTS
		  if(flags & wxSERIAL_LINESTATE_CTS) {
			 m_led[LED_CTS]->SetColour(awxLED_RED);
		  }
		  else { 
			 m_led[LED_CTS]->SetColour(awxLED_GREEN);
		  }
		  // DSR
		  if(flags & wxSERIAL_LINESTATE_DSR) {
			 m_led[LED_DSR]->SetColour(awxLED_RED);
		  }
		  else { 
			 m_led[LED_DSR]->SetColour(awxLED_GREEN);
		  }
		  // DCD
		  if(flags & wxSERIAL_LINESTATE_DCD) {
			 m_led[LED_DCD]->SetColour(awxLED_RED);
		  }
		  else { 
			 m_led[LED_DCD]->SetColour(awxLED_GREEN);
		  }
		  // RI
		  if(flags & wxSERIAL_LINESTATE_RING) {
			 m_led[LED_RI]->SetColour(awxLED_RED);
		  }
		  else { 
			 m_led[LED_RI]->SetColour(awxLED_GREEN);
		  }
		  for(int i=LED_RTS;i<=LED_RI;i++) {
			 m_led[i]->SetState(awxLED_ON);
		  }
	   }
    }
};

int wxModuleRS232::WriteDevice(char* buf,size_t len)
{
    int rd = m_dev.Write(buf,len);
    return rd;
};
