/////////////////////////////////////////////////////////////////////////////
// Name:        menu.cpp
// Purpose:     
// Author:      Joachim Buermann
// Id:          $Id: menu.cpp,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "menu.h"

// create a menu bar
wxMenuBar* CreateMenuBar()
{
    wxMenu *menuFile = new wxMenu("");
    menuFile->Append(ID_CONNECT, _("&Open COM"));
    menuFile->Append(ID_DISCONNECT, _("&Close COM"));
    menuFile->Append(ID_QUIT, _("E&xit\tCtrl-X"));

    wxMenu *menuView = new wxMenu;
    menuView->Append(ID_EMPTY,_("&Clear Output\tCtrl-C"));
    menuView->AppendCheckItem(ID_RELOAD,_("&Scroll Output\tCtrl-Shift-S"));
    menuView->AppendSeparator();
    menuView->AppendRadioItem(ID_VIEWASCII,_("&Ascii Output\tCtrl-T"));
    menuView->AppendRadioItem(ID_VIEWHEX,_("&Hex Output\tCtrl-H"));

    wxMenu *menuOptions = new wxMenu;
    menuOptions->Append(ID_SETTINGS,_("&Preference"));
    
    wxMenu *menuExtras = new wxMenu;
    menuExtras->Append(ID_BREAK,_("Send &Break\tCtrl-B"));
    menuExtras->Append(ID_TOGGLE_DTR,_("Toggle &DTR\tCtrl-D"));
    menuExtras->Append(ID_TOGGLE_RTS,_("Toggle &RTS\tCtrl-R"));
    menuExtras->AppendCheckItem(ID_TB_TEXT,_("Show toolbar text"));
    menuExtras->Check(ID_TB_TEXT,true);

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(ID_HELP,_("wxTerm &Help\tF1"));
    helpMenu->Append(ID_ABOUT,_("&About...\tCtrl-A"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuView, _("&View"));
    menuBar->Append(menuOptions, _("&Options"));
    menuBar->Append(menuExtras, _("&Extras"));
    menuBar->Append(helpMenu, _("&Help"));

    return menuBar;
};
