/////////////////////////////////////////////////////////////////////////////
// Name:        modbase.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: modbase.cpp,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "modbase.h"

BEGIN_EVENT_TABLE(wxModuleBase, wxPanel)
#if wxCHECK_VERSION(2, 6, 0)
    // do nothing
#else
    EVT_SIZE(wxModuleBase::OnSizeEvent)
    EVT_ERASE_BACKGROUND(wxModuleBase::OnEraseBackground)
#endif
END_EVENT_TABLE()

wxModuleBase::wxModuleBase(wxWindow* parent,
					  wxWindowID id,
					  const wxPoint& pos,
					  const wxSize& size) :
    wxPanel(parent,id,pos,size)
{
    InitBase();
    SetBackgroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE) );
};

wxModuleBase::~wxModuleBase()
{
};
