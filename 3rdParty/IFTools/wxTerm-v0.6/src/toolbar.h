/////////////////////////////////////////////////////////////////////////////
// Name:        toolbar.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: toolbar.h,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2003 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __TOOLBAR_H
#define __TOOLBAR_H

#include <wx/awx-0.4/awx.h>
#include "menu.h"

int CreateToolbar(awxToolbar* tb,bool showText = true);
void EnableButton(awxToolbar* tb,long id,bool enable);
void PressButton(awxToolbar* tb,long id,bool press);

#endif
