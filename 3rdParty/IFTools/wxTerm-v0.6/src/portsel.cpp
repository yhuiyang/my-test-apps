/////////////////////////////////////////////////////////////////////////////
// Name:        portsel.cpp
// Purpose:     scans for available comports and put it in a wxChoice
// Author:      Joachim Buermann
// Id:          $Id: portsel.cpp,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "portsel.h"
#include <wx/progdlg.h>
#include <wx/ctb-0.13/serport.h>

wxPortSelector::wxPortSelector(wxWindow* parent,
						 wxWindowID id,
						 const wxPoint& pos,
						 const wxSize& size):
    wxChoice(parent,id,pos,wxSize(120,-1))
{
    wxArrayString ports = ScanPorts();
    for(int i=0;i<ports.Count();i++) {
	   Append(ports.Item(i));
    }
    SetSelection(0);
};

wxArrayString wxPortSelector::ScanPorts()
{
    const char* ports[] = {
	   wxCOM1,wxCOM2,wxCOM3,wxCOM4,wxCOM5,wxCOM6,wxCOM7,wxCOM8,
#ifdef __GNUG__
	   "/dev/ttyUSB0","/dev/ttyUSB1"
#else
	   wxCOM9,wxCOM10,wxCOM11,wxCOM12
#endif
    };

    wxProgressDialog progress( _("Please wait"),
						 _("Search for new serial ports..."),
						 sizeof( ports ) >> 2 );

    wxArrayString result;

    wxSerialPort com;

    for(int i=0; i < ( sizeof( ports ) >> 2 ); i++, progress.Update( i ) ) {
#ifdef __GNUG__
	   if(com.Open(ports[i]) < 0) {
		  continue;
	   }
	   result.Add(wxString(ports[i]));
	   com.Close();
#else
	   COMMCONFIG cc;
	   DWORD dwSize = sizeof(cc);
	   if (::GetDefaultCommConfig(ports[i], &cc, &dwSize)) {
		  if(cc.dwProviderSubType == PST_RS232) {
			 if(com.Open(ports[i]) >= 0) {
				result.Add(wxString(ports[i]));
				com.Close();
			 }
		  }
	   }
#endif
    }
    return result;
};
