/////////////////////////////////////////////////////////////////////////////
// Name:        toolbar.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: toolbar.cpp,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2003 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "toolbar.h"

#ifndef ICONS
# define ICONS "../icons/32x32/icons.h"
#endif

#include ICONS

#define USE_SAVE_AS 0

int CreateToolbar(awxToolbar* tb,bool showText)
{
    awxButton* mb;
    tb->EnableTheme(true);
    if(!showText) tb->SetExtraSpace(2);
    // Quit button
    mb = new awxButton(tb,ID_QUIT,
				   wxDefaultPosition,wxDefaultSize,
				   quit_xpm,
				   NULL,
				   NULL,
				   quit_xpm);
    mb->SetToolTip(_("Quit"));
    if(showText) mb->SetText(_("Quit"));
    tb->Add(mb);
    // ---------------------------------
    tb->Add(new awxSeparator(tb));
    // Connect
    mb = new awxCheckButton(tb,ID_CONNECT_ON_OFF,
					   wxDefaultPosition,wxDefaultSize,
					   disconnect_xpm,
					   NULL,
					   connect_xpm,
					   disconnect_xpm);
    mb->SetToolTip(_("(Dis)Connect"));
    tb->Add(mb);
    // ---------------------------------
    tb->Add(new awxSeparator(tb));
    // New (clear) button
    mb = new awxButton(tb,ID_EMPTY,
				   wxDefaultPosition,wxDefaultSize,
				   filenew_xpm,
				   NULL,
				   NULL,
				   filenew_xpm);
    mb->SetToolTip(_("Clear output window (Ctrl-c)"));
    if(showText) mb->SetText(_("Clear"));
    tb->Add(mb);
    // Open button
    mb = new awxButton(tb,ID_OPEN,
				   wxDefaultPosition,wxDefaultSize,
				   fileopen_xpm,
				   NULL,
				   NULL,
				   fileopen_dis_xpm);
    mb->SetToolTip(_("Open file to send (Ctrl-o)"));
    if(showText) mb->SetText(_("Open"));
    tb->Add(mb);
    // Save button
    mb = new awxButton(tb,ID_SAVE_AS,
				   wxDefaultPosition,wxDefaultSize,
				   filesave_xpm,
				   NULL,
				   NULL,
				   filesave_dis_xpm);
    mb->SetToolTip(_("Save received data as..."));
    if(showText) mb->SetText(_("Save as"));
    tb->Add(mb);
    // ---------------------------------
    tb->Add(new awxSeparator(tb));
    // Switch (radio) buttons
    awxRadioButtonBox* m_box = new awxRadioButtonBox(tb,ID_VIEW);
    // Hex
    mb = new awxRadioButton(tb,ID_VIEWHEX,
					   wxDefaultPosition,wxSize(40,32),
					   view_hex_xpm,
					   NULL,
					   NULL,
					   view_hex_xpm,
					   m_box
	   );
    mb->SetToolTip(_("Show data as hex (Ctrl-h)"));
    if(showText) mb->SetText(_("Hex"));
    tb->Add(mb);
    // Ascii
    mb = new awxRadioButton(tb,ID_VIEWASCII,
					   wxDefaultPosition,wxSize(40,32),
					   view_ascii_xpm,
					   NULL,
					   NULL,
					   view_ascii_xpm,
					   m_box
	   );
    mb->SetToolTip(_("Show data as normal text (Ctrl-t)"));
    if(showText) mb->SetText(_("Ascii"));
    tb->Add(mb);
    // Autoscroll
    mb = new awxCheckButton(tb,ID_RELOAD,
					   wxDefaultPosition,wxDefaultSize,
					   refresh_xpm,
					   NULL,
					   NULL,
					   refresh_xpm);
    mb->SetToolTip(_("Switch on/off autoscrolling"));
    if(showText) mb->SetText(_("Scroll"));
    tb->Add(mb);
    // ---------------------------------
    tb->Add(new awxSeparator(tb));
    // Break
    mb = new awxButton(tb,ID_BREAK,
				   wxDefaultPosition,wxDefaultSize,
				   break_xpm,
				   NULL,
				   NULL,
				   break_dis_xpm);
    mb->SetToolTip(_("Reset device"));
    if(showText) mb->SetText(_("Reset"));
    tb->Add(mb);
    // Settings
    mb = new awxButton(tb,ID_SETTINGS,
				   wxDefaultPosition,wxDefaultSize,
				   configure_xpm,
				   NULL,
				   NULL,
				   configure_xpm);
    mb->SetToolTip(_("Preferences"));
    if(showText) mb->SetText(_("Settings"));
    tb->Add(mb);

    m_box->SetValue(ID_VIEWASCII);
    tb->Redraw();
    return 0;
};

void EnableButton(awxToolbar* tb,long id,bool enable)
{
    awxButton* mb;
    mb = (awxButton*)tb->FindWindow(id);
    if(mb) {
	   if(enable) mb->Enable();
	   else mb->Disable();
    }
};

void PressButton(awxToolbar* tb,long id,bool press)
{
    awxButton* mb;
    mb = (awxButton*)tb->FindWindow(id);
    if(mb) {
	   if(press) mb->Press();
	   else mb->Release();
    }
};
