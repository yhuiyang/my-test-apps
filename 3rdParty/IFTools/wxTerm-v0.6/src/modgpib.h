/////////////////////////////////////////////////////////////////////////////
// Name:        modgpib.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: modgpib.h,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __TBGPIB_H
#define __TBGPIB_H

#include <wx/wx.h>
#include <wx/ctb-0.13/gpib.h>
#include "modbase.h"

class wxModuleGPIB : public wxModuleBase
{
protected:
    wxGPIB m_dev;
    wxStaticText* m_gpibState[14];
    wxStaticText* m_devState[6];
    wxStaticText* m_lineState[8];
    wxStaticText* m_errno;
    wxStaticText* m_errid;
    wxCheckBox* m_readbg;
    wxButton* m_readButton;
    int m_readOne;
    void InitBase();    
public:
    wxModuleGPIB(wxWindow* parent,
			  wxWindowID id = -1,
			  const wxPoint& pos = wxDefaultPosition,
			  const wxSize& size = wxDefaultSize) :
	   wxModuleBase(parent,id,pos,size)
	   {InitBase();}
    wxModuleGPIB() {
	   InitBase();
    };
    ~wxModuleGPIB();
    int CloseDevice() {
	   EnableChildren(false);
	   return m_dev.Close();
    };
    void EnableChildren(bool enable);
    int GetEOSSelection() {return 0;};
    wxString GetEOSString() {return wxString("\n");};
    int GetInputMode() {return 0;};
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
    void SetEOSSelection(int selection) {};
    void SetEOSString(const wxString& eos) {};
    void SetInputMode(int mode) {};
    void ShowState(bool force);
    int WriteDevice(char* buf,size_t len);
    // special device specific functions
protected:
    void OnReadCheckbox(wxCommandEvent& event);
    void OnReadButton(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
};

#endif
