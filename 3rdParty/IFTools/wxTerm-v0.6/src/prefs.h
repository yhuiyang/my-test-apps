/////////////////////////////////////////////////////////////////////////////
// Name:        prefs.h
// Purpose:     struct to save all program settings
// Author:      Joachim Buermann
// Id:          $Id: prefs.h,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __PREFS_H
#define __PREFS_H

#include <wx/wx.h>
#if ( GPIB )
# include <wx/ctb-0.13/gpib.h>
#endif
#include <wx/ctb-0.13/serport.h>
#ifdef __WIN32__
# include <wx/msw/winundef.h>
#endif

struct colors_t
{
    unsigned int m_min : 8;
    unsigned int m_max : 8;
    unsigned int m_color: 8;			  // color index
    unsigned int m_use: 1;
    unsigned int nc : 7;			  // not used
    void Set(unsigned int u) {
	   m_min = u & 0xff;
	   m_max = (u >> 8) & 0xff;
	   m_color = (u >> 16) & 0xff;
	   m_use = (u >> 24) & 0xff;
    };
    unsigned int Get() {
	   unsigned int u = m_min | 
		  (m_max << 8) | 
		  (m_color << 16) |
		  (m_use << 24);
	   return u;
    };
};

bool IsColorRule(colors_t rule,unsigned char c,int& cindex);

struct prefs_t
{
    // RS232 connection settings
    wxString m_port;
    wxSerialPort_DCS m_s;
    // end of string sequence for serial communication (lf, cr, crlf...)
    int m_eos;
    // input mode for serial communication (cooked, cooked hex or raw)
    int m_inputMode;
#if ( GPIB )
    // GPIB connection settings
    wxGPIB_DCS m_g;
#endif
    // selected device (0: rs232, 1: gpib)
    int m_device;
    // additional settings
    wxString m_logfile;
    // if true, connect the last selected device on start
    bool m_autoConnect;
    // if true, scroll the output window to show the last
    bool m_autoScroll;
    // view mode (ascii=0, hex=1)
    int m_viewMode;
    int m_sashPos;
    // position and size
    wxRect m_dim;
    // toolbar settings
    bool m_toolbarText;
    // color settings
    colors_t m_c[5];
    prefs_t() {
	   m_port = wxCOM1;
	   m_eos = 0;			  // lf
	   m_inputMode = 0;		  // cooked
	   m_device = 0;		  // default is rs232
	   m_logfile = "log.dat";
	   m_autoConnect = false;
	   m_autoScroll = true;
	   m_sashPos = -1;
	   m_viewMode = 0;
	   m_toolbarText = true;
	   memset(m_c,0,sizeof(m_c));
	   m_c[4].m_min = 0;
	   m_c[4].m_max = 255;
#if ( GPIB )
	   // GPIB default timeout is 3s, reduce it to 100ms 
	   m_g.m_timeout = wxGPIB_TO_100ms;
#endif
	   m_dim = wxRect(20,20,600,400);
    };
    wxString GetDeviceSettings();
};

//
// return values:
// false : no changes
// true  : setup was changed
//
bool SetupPreferences(wxWindow* parent,
				  struct prefs_t& prefs,
				  bool enablePortSettings,
				  bool enableGPIBSettings);

#endif
