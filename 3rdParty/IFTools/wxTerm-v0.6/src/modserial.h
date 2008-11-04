/////////////////////////////////////////////////////////////////////////////
// Name:        modserial.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: modserial.h,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __TBSERIAL_H
#define __TBSERIAL_H

#include <wx/wx.h>
#include <wx/awx-0.4/led.h>
#include <wx/ctb-0.13/serport.h>
#include "modbase.h"

class wxModuleRS232 : public wxModuleBase
{
protected:
    awxLed* m_led[6];
    enum {
	   LED_RTS = 0,
	   LED_DTR,
	   LED_CTS,
	   LED_DSR,
	   LED_DCD,
	   LED_RI
    };
    bool m_dtr;
    bool m_rts;
    wxSerialPort m_dev;
    void InitBase();
public:
    wxModuleRS232(wxWindow* parent,
			   wxWindowID id = -1,
			   const wxPoint& pos = wxDefaultPosition,
			   const wxSize& size = wxDefaultSize) :
	   wxModuleBase(parent,id,pos,size)
	   {InitBase();}
    wxModuleRS232() {
	   InitBase();
    };
    ~wxModuleRS232();
    int CloseDevice() {
	   EnableChildren(false);
	   return m_dev.Close();
    };
    void EnableChildren(bool enable);
    int GetEOSSelection();
    wxString GetEOSString();
    int GetInputMode();
    int IsOpen() {
	   return m_dev.IsOpen();
    };
    int OpenDevice(char* dev,void* dcs) {
	   int result = m_dev.Open(dev,dcs);
	   EnableChildren(result >= 0);
	   return result;
    };
    int ReadDevice(char* buf,size_t len);
    void ResetDevice();
    bool SetBaudrate(wxBaud baudrate);
    void SetEOSSelection(int selection);
    void SetEOSString(const wxString& string);
    void SetInputMode(int mode);
    void ShowState(bool force);
    int WriteDevice(char* buf,size_t len);
    // special device specific functions
    void SetDTR(bool state);
    void SetRTS(bool state);
};

#endif
