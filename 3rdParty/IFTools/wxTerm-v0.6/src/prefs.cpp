/////////////////////////////////////////////////////////////////////////////
// Name:        prefs.cpp
// Purpose:     the complete preference dialog
// Author:      Joachim Buermann
// Id:          $Id: prefs.cpp,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "portsel.h"
#include "prefs.h"
#include <wx/awx-0.4/obdlg.h>

#include "../icons/48x48/colors.xpm"
#include "../icons/48x48/connect.xpm"
#include "../icons/48x48/ni.xpm"
#include "../icons/48x48/logging.xpm"
#include "color_wdr.h"
#include "connection_wdr.h"
#if ( GPIB )
# include "gpib_wdr.h"
#endif
#include "logging_wdr.h"

#ifdef __WIN32__
# include <wx/msw/winundef.h>
#endif

# pragma warning (disable : 4800)

wxString prefs_t::GetDeviceSettings()
{
    char tmpbuf[32];
    wxString s;
    if(m_device == 0) {		  // rs232
	   s.Printf("%s %s", m_s.GetSettings(), m_port.c_str());
    }
#if ( GPIB )
    else {
	   s.Printf("%s GPIB1", m_g.GetSettings() );
    }
#endif
    return s;
};


bool IsColorRule(colors_t rule,unsigned char c,int& cindex) {
    if(rule.m_use && (c >= rule.m_min) && (c <= rule.m_max)) {
	   cindex = rule.m_color;
	   return true;
    }
    return false;
};

class rs232 : public wxOutbarPanel
{
public:
    rs232(wxWindow* parent,
		 wxWindowID id,
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize) :
	   wxOutbarPanel(parent,id,pos,size)
	   {
		  new wxPortSelector(this,ID_PORT,
						 wxDefaultPosition,wxDefaultSize);
 		  SetupConnection(this);
	   };
    bool Get(prefs_t& prefs);
    bool Set(prefs_t& prefs);
};

bool rs232::Get(prefs_t& prefs)
{
    // temporarily used for some conversion
    long l;
    // port
    prefs.m_port = ((wxChoice*)FindWindow(ID_PORT))->GetStringSelection();
    // baudrate
    ((wxChoice*)FindWindow(ID_BAUDRATE))->GetStringSelection().ToLong(&l);
    prefs.m_s.baud = wxBaud(l);
    // wordlen
    ((wxChoice*)FindWindow(ID_WORDLEN))->GetStringSelection().ToLong(&l);
    prefs.m_s.wordlen = (unsigned char)l;
    // parity
    prefs.m_s.parity = (wxParity)((wxChoice*)FindWindow(ID_PARITY))
	   ->GetSelection();
    // stopbits
    prefs.m_s.stopbits = ((wxChoice*)FindWindow(ID_STOPBITS))
	   ->GetSelection()+1;
    // RTS/CTS protocol
    prefs.m_s.rtscts = ((wxCheckBox*)FindWindow(ID_RTSCTS))->IsChecked();
    // Xon/Xoff protocol
    prefs.m_s.xonxoff = ((wxCheckBox*)FindWindow(ID_XONXOFF))->IsChecked();
    return true;
};

bool rs232::Set(prefs_t& prefs)
{
    // port
    wxChoice* choice = (wxChoice*)FindWindow(ID_PORT);
    if(IsEnabled()) {
	   choice->SetStringSelection(prefs.m_port);
    }
    else {
	   choice->Clear();
	   choice->Append(prefs.m_port);
	   choice->SetSelection(0);
    }
    // baudrate
    wxString s;
    s.Printf("%i",prefs.m_s.baud);
    ((wxChoice*)FindWindow(ID_BAUDRATE))->SetStringSelection(s);
    // wordlen
    s.Printf("%i",prefs.m_s.wordlen);
    ((wxChoice*)FindWindow(ID_WORDLEN))->SetStringSelection(s);
    // parity
    ((wxChoice*)FindWindow(ID_PARITY))->SetSelection(prefs.m_s.parity);
    // stopbits
    ((wxChoice*)FindWindow(ID_STOPBITS))->SetSelection(prefs.m_s.stopbits-1);
    // RTC/CTS protocol
    ((wxCheckBox*)FindWindow(ID_RTSCTS))->SetValue(prefs.m_s.rtscts);
    // Xon/Xoff protocol
    ((wxCheckBox*)FindWindow(ID_XONXOFF))->SetValue(prefs.m_s.xonxoff);
    return true;
};

class logging : public wxOutbarPanel
{
public:
    logging(wxWindow* parent,
		 wxWindowID id,
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize) :
	   wxOutbarPanel(parent,id,pos,size)
	   {
 		  SetupDataLogging(this);
	   };
    bool Get(prefs_t& prefs);
    bool Set(prefs_t& prefs);
};

bool logging::Get(prefs_t& prefs)
{
    // logfile
    prefs.m_logfile = ((wxTextCtrl*)FindWindow(ID_LOGFILE))->
	   GetValue();
    return true;
};

bool logging::Set(prefs_t& prefs)
{
    // port
    ((wxTextCtrl*)FindWindow(ID_LOGFILE))->
	   SetValue(prefs.m_logfile);
    return true;
};

class colors : public wxOutbarPanel
{
public:
    colors(wxWindow* parent,
		 wxWindowID id,
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize) :
	   wxOutbarPanel(parent,id,pos,size)
	   {
 		  SetupColor(this);
	   };
    bool Get(prefs_t& prefs);
    bool Set(prefs_t& prefs);
};

bool colors::Get(prefs_t& prefs)
{
    wxWindowID id = ID_USE1;
    for(int i=0;i<4;i++) {
	   // in use?
	   prefs.m_c[i].m_use = ((wxCheckBox*)FindWindow(id++))->IsChecked();
	   // range first...
	   prefs.m_c[i].m_min = ((wxSpinCtrl*)FindWindow(id++))->GetValue();
	   // ... last
	   prefs.m_c[i].m_max = ((wxSpinCtrl*)FindWindow(id++))->GetValue();
	   // color index
	   prefs.m_c[i].m_color = ((wxChoice*)FindWindow(id++))->GetSelection();
    }
    // rest
    prefs.m_c[4].m_use = ((wxCheckBox*)FindWindow(id++))->IsChecked();
    prefs.m_c[4].m_color = ((wxChoice*)FindWindow(id++))->GetSelection();
    return true;
};

bool colors::Set(prefs_t& prefs)
{
    wxWindowID id = ID_USE1;
    for(int i=0;i<4;i++) {
	   // in use?
	   ((wxCheckBox*)FindWindow(id++))->SetValue(prefs.m_c[i].m_use);
	   // range first...
	   ((wxSpinCtrl*)FindWindow(id++))->SetValue(prefs.m_c[i].m_min);
	   // ... last
	   ((wxSpinCtrl*)FindWindow(id++))->SetValue(prefs.m_c[i].m_max);
	   // color index
	   ((wxChoice*)FindWindow(id++))->SetSelection(prefs.m_c[i].m_color);
    }
    // rest
    ((wxCheckBox*)FindWindow(id++))->SetValue(prefs.m_c[4].m_use);
    ((wxChoice*)FindWindow(id++))->SetSelection(prefs.m_c[4].m_color);
    return true;
};

#if ( GPIB )
class gpib : public wxOutbarPanel
{
public:
    gpib(wxWindow* parent,
		 wxWindowID id,
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize) :
	   wxOutbarPanel(parent,id,pos,size)
	   {
 		  SetupGPIB(this);
	   };
    bool Get(prefs_t& prefs);
    bool Set(prefs_t& prefs);
};

bool gpib::Get(prefs_t& prefs)
{
    // address
    prefs.m_g.m_address1 = ((wxSpinCtrl*)FindWindow(ID_ADDRESS))->GetValue();
    // timeout
    prefs.m_g.m_timeout = (wxGPIB_Timeout)((wxChoice*)FindWindow(ID_TIMEOUT))
	   ->GetSelection();
    // EOS settings
    // Send EOI at end of write
    prefs.m_g.m_eot = ((wxCheckBox*)FindWindow(ID_IBEOT))->GetValue();
    // the next 3 checkbox settings correspond with the eosMode member
    // of the wxGPIB_DCS struct
    // a.) Terminate read when EOS is detected
    unsigned char eosMode = 0;
    if(((wxCheckBox*)FindWindow(ID_IBEOSA))->GetValue()) eosMode |= 0x04;
    // b.) Set EOI with EOS on write function
    if(((wxCheckBox*)FindWindow(ID_IBEOSB))->GetValue()) eosMode |= 0x08;
    // c.) Compare all 8 bits of EOS byte rather than low 7 bits (rd + wr)
    if(((wxCheckBox*)FindWindow(ID_IBEOSC))->GetValue()) eosMode |= 0x10;
    prefs.m_g.m_eosMode = eosMode;
    // EOS character as dezimal value (for example LineFeed is 10)
    long l;
    ((wxTextCtrl*)FindWindow(ID_IBEOS))->GetValue().ToLong(&l);
    prefs.m_g.m_eosChar = (unsigned char)l;
    return true;
};

bool gpib::Set(prefs_t& prefs)
{
    // address
    ((wxSpinCtrl*)FindWindow(ID_ADDRESS))->SetValue(prefs.m_g.m_address1);
    // timeout
    ((wxChoice*)FindWindow(ID_TIMEOUT))->SetSelection(prefs.m_g.m_timeout);
    // EOS settings
    // Send EOI at end of write
    ((wxCheckBox*)FindWindow(ID_IBEOT))->SetValue(prefs.m_g.m_eot);
    // the next 3 checkbox settings correspond with the eosMode member
    // of the wxGPIB_DCS struct
    // a.) Terminate read when EOS is detected
    if(prefs.m_g.m_eosMode & 0x04) {
	   ((wxCheckBox*)FindWindow(ID_IBEOSA))->SetValue(true);
    }
    // b.) Set EOI with EOS on write function
    if(prefs.m_g.m_eosMode & 0x08) {
	   ((wxCheckBox*)FindWindow(ID_IBEOSB))->SetValue(true);
    }
    // c.) Compare all 8 bits of EOS byte rather than low 7 bits (rd + wr)
    if(prefs.m_g.m_eosMode & 0x10) {
	   ((wxCheckBox*)FindWindow(ID_IBEOSC))->SetValue(true);
    }
    // EOS character as dezimal value (for example LineFeed is 10)
    wxString s;
    s.Printf("%i",prefs.m_g.m_eosChar);
    ((wxTextCtrl*)FindWindow(ID_IBEOS))->SetValue(s);
    return true;
};
#endif

bool SetupPreferences(wxWindow* parent,
				  struct prefs_t& prefs,
				  bool enablePortSettings,
				  bool enableGPIBSettings)
{
    int pageNo = 0;
    int pageRS232 = pageNo++;
    wxOutbarDialog d(parent,-1,_("Settings Dialog"),wxSize(500,380));
    d.AddPage(connect_xpm,
		    _("RS232 Settings"),
		    new rs232(d.GetEmptyPage(),-1,
					 wxDefaultPosition,wxDefaultSize),
		    _("RS232 Connection Settings")
	   );
#if ( GPIB )
    int pageGPIB = pageNo++;
    d.AddPage(ni_xpm,
		    _("GPIB Settings"),
		    new gpib(d.GetEmptyPage(),-1,
					 wxDefaultPosition,wxDefaultSize),
		    _("GPIB Connection settings")
	   );
#endif
    int pageLogging = pageNo++;
    d.AddPage(logging_xpm,
		    _("Logging"),
		    new logging(d.GetEmptyPage(),-1,
					 wxDefaultPosition,wxDefaultSize),
		    _("Data logging settings")
	   );
    int pageColors = pageNo++;
    d.AddPage(colors_xpm,
		    _("Colors"),
		    new colors(d.GetEmptyPage(),-1,
					 wxDefaultPosition,wxDefaultSize),
		    _("Hexchar color settings")
	   );

    d.SetSelection(_("Connection"));

    ((rs232*)d.GetPage(pageRS232))->Set(prefs);
    d.GetPage(pageRS232)->Enable(enablePortSettings);
#if ( GPIB )
    ((gpib*)d.GetPage(pageGPIB))->Set(prefs);
    d.GetPage(pageGPIB)->Enable(enableGPIBSettings & enablePortSettings);
#endif
    ((logging*)d.GetPage(pageLogging))->Set(prefs);
    ((colors*)d.GetPage(pageColors))->Set(prefs);
    
    if(d.ShowModal() == wxID_OK) {
	   ((rs232*)d.GetPage(pageRS232))->Get(prefs);
#if ( GPIB )
	   ((gpib*)d.GetPage(pageGPIB))->Get(prefs);
#endif
	   ((colors*)d.GetPage(pageColors))->Get(prefs);
	   // something was changed
	   return true;
    }
    // no changes
    return false;
};

