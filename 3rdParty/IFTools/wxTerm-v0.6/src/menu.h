/////////////////////////////////////////////////////////////////////////////
// Name:        menu.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: menu.h,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2003 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __MENU_H
#define __MENU_H

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

enum {
    ID_QUIT = wxID_HIGHEST + 1,
    ID_MODULES,
    ID_CONNECT,
    ID_DISCONNECT,
    ID_CONNECT_ON_OFF,
    ID_HELP,
    ID_ABOUT,
    ID_NEW,
    ID_OPEN,
    ID_SAVE,
    ID_SAVE_AS,
    ID_VIEW,
    ID_VIEWHEX,
    ID_VIEWASCII,
    ID_REFRESH,
    ID_SETTINGS,
    ID_COMMANDLINE,
    ID_OUTPUT,
    ID_EMPTY,
    ID_EOS,
    ID_IMODE,
    ID_HEXIN,
    ID_BREAK,
    ID_FORMAT,
    ID_RELOAD,
    ID_SCROLL,
    ID_TB_TEXT,
    ID_TICKER,
    ID_TOGGLE_RTS,
    ID_TOGGLE_DTR,
    ID_LED_REC,
    ID_LED_RTS,
    ID_LED_DTR,
    ID_LED_CTS,
    ID_LED_DSR,
    ID_LED_DCD,
    ID_LED_RI,
    ID_SWITCHBAUD
};

wxMenuBar* CreateMenuBar();

#endif
