/////////////////////////////////////////////////////////////////////////////
// Name:        portsel.h
// Purpose:     wxPortSelector class
// Author:      Joachim Buermann
// Id:          $Id: portsel.h,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>

class wxPortSelector : public wxChoice
{
protected:
    wxArrayString ScanPorts();
public:
    wxPortSelector(wxWindow* parent,
			    wxWindowID id,
			    const wxPoint& pos,
			    const wxSize& size);
};
