#include <wx/config.h>
#include "config.h"

bool ReadConfig(prefs_t& prefs)
{
    bool result = true;
    int ok = 0;
    long l;
    wxConfigBase* config = wxConfig::Get();

    while(ok++ < 1) {
	   // connected device
	   if(!config->Read(wxT("Connection/ComPort"),&prefs.m_port)) break;
	   // baudrate (RS232)
	   if(!config->Read(wxT("Connection/Baudrate"),&l)) break;
	   prefs.m_s.baud = wxBaud(l);
	   // wordlen (RS232)
	   if(!config->Read(wxT("Connection/Wordlen"),&l)) break;
	   prefs.m_s.wordlen = l;
	   // stopbits (RS232)
	   if(!config->Read(wxT("Connection/Stopbits"),&l)) break;
	   prefs.m_s.stopbits = l;
	   // parity (RS232)
	   if(!config->Read(wxT("Connection/Parity"),&l)) break;
	   prefs.m_s.parity = wxParity(l);
	   // protocol rts/cts (RS232)
	   if(!config->Read(wxT("Connection/RtsCts"),&l)) break;
	   prefs.m_s.rtscts = l;
	   // protocol xon/xoff (RS232)
	   if(!config->Read(wxT("Connection/XonXoff"),&l)) break;
	   prefs.m_s.xonxoff = l;
#if ( GPIB )
	   // address1 (GPIB)
	   if(!config->Read(wxT("Connection/Address1"),&l)); break;
	   prefs.m_g.m_address1 = l;
	   // address2 (GPIB)
	   if(!config->Read(wxT("Connection/Address2"),&l)) break;
	   prefs.m_g.m_address2 = l;
	   // timeout (GPIB)
	   if(!config->Read(wxT("Connection/Timeout"),&l)) break;
	   prefs.m_g.m_timeout = wxGPIB_Timeout(l);
	   // eot (GPIB)
	   if(!config->Read(wxT("Connection/eot"),&l)) break;
	   prefs.m_g.m_eot = l;
	   // eos char (GPIB)
	   if(!config->Read(wxT("Connection/eosChar"),&l)) break;
	   prefs.m_g.m_eosChar = (unsigned char)l;
	   // eos mode (GPIB)
	   if(!config->Read(wxT("Connection/eosMode"),&l)) break;
	   prefs.m_g.m_eosMode = (unsigned char)l;
#endif
	   // auto connect
	   if(!config->Read(wxT("Settings/AutoConnect"),&prefs.m_autoConnect)) {
		  break;
	   }
	   // appearance
	   if(!config->Read(wxT("Settings/ViewMode"),&l)) break;
	   prefs.m_viewMode = l;
	   // autoscroll
	   if(!config->Read(wxT("Settings/AutoScroll"),&prefs.m_autoScroll)) {
		  break;
	   }
	   // eos terminal settings
	   if(!config->Read(wxT("Settings/TermEos"),&l)) break;
	   prefs.m_eos = l;
	   // input mode
	   if(!config->Read(wxT("Settings/InputMode"),&l)) break;
	   prefs.m_inputMode = l;
	   // toolbar text on/off
	   if(!config->Read(wxT("Settings/ToolbarText"),&prefs.m_toolbarText)) {
		  break;
	   }
	   // sash position
	   if(!config->Read(wxT("Settings/SashPosition"),&l)) break;
	   prefs.m_sashPos = l;
	   // color rules
	   for(int i=0;i<5;i++) {
		  wxString s;
		  s.Printf("Settings/ColorRule%i",i);
		  if(!config->Read(s,&l)) break;
		  prefs.m_c[i].Set(l);
	   }
	   // window position and size
	   int x,y,w,h;
	   if(!config->Read(wxT("Settings/Xpos"),&x)) break;
	   if(!config->Read(wxT("Settings/Ypos"),&y)) break;
	   if(!config->Read(wxT("Settings/Height"),&w)) break;
	   if(!config->Read(wxT("Settings/Width"),&h)) break;
	   prefs.m_dim = wxRect(x,y,w,h);
    }
    return ok > 0;
};

bool WriteConfig(prefs_t& prefs)
{
    wxConfigBase* config = wxConfig::Get();
    // connected device
    config->Write(wxT("Connection/ComPort"),prefs.m_port);
    // baudrate (RS232)
    config->Write(wxT("Connection/Baudrate"),prefs.m_s.baud);
    // wordlen (RS232)
    config->Write(wxT("Connection/Wordlen"),prefs.m_s.wordlen);  
    // stopbits (RS232)
    config->Write(wxT("Connection/Stopbits"),prefs.m_s.stopbits);  
    // parity (RS232)
    config->Write(wxT("Connection/Parity"),prefs.m_s.parity);  
    // protocol rts/cts (RS232)
    config->Write(wxT("Connection/RtsCts"),prefs.m_s.rtscts);  
    // protocol xon/xoff (RS232)
    config->Write(wxT("Connection/XonXoff"),prefs.m_s.xonxoff);  
#if ( GPIB )
    // address1 (GPIB)
    config->Write(wxT("Connection/Address1"),prefs.m_g.m_address1);
    // address2 (GPIB)
    config->Write(wxT("Connection/Address2"),prefs.m_g.m_address2);
    // timeout (GPIB)
    config->Write(wxT("Connection/Timeout"),prefs.m_g.m_timeout);
    // eot (GPIB)
    config->Write(wxT("Connection/eot"),prefs.m_g.m_eot);
    // eos char (GPIB)
    config->Write(wxT("Connection/eosChar"),prefs.m_g.m_eosChar);
    // eos mode (GPIB)
    config->Write(wxT("Connection/eosMode"),prefs.m_g.m_eosMode);
#endif
    // auto connect
    config->Write(wxT("Settings/AutoConnect"),prefs.m_autoConnect);
    // appearance
    config->Write(wxT("Settings/ViewMode"),prefs.m_viewMode);
    // autoscroll
    config->Write(wxT("Settings/AutoScroll"),prefs.m_autoScroll);
    // eos terminal settings
    config->Write(wxT("Settings/TermEos"),prefs.m_eos);
    // input mode
    config->Write(wxT("Settings/InputMode"),prefs.m_inputMode);
    // toolbar text on/off
    config->Write(wxT("Settings/ToolbarText"),prefs.m_toolbarText);
    // sash position
    config->Write(wxT("Settings/SashPosition"),prefs.m_sashPos);
    // color rules (5)
    long* lp = (long*)prefs.m_c;
    for(int i=0;i<5;i++) {
	   wxString s;
	   s.Printf("Settings/ColorRule%i",i);
	   config->Write(s,lp[i]);
    }
    // window position and size
    config->Write(wxT("Settings/Xpos"),prefs.m_dim.GetX());
    config->Write(wxT("Settings/Ypos"),prefs.m_dim.GetY());
    config->Write(wxT("Settings/Height"),prefs.m_dim.GetWidth());
    config->Write(wxT("Settings/Width"),prefs.m_dim.GetHeight());
    return true;
};
