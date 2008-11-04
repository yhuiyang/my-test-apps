/////////////////////////////////////////////////////////////////////////////
// Name:        modbase.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: modbase.h,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __MODBASE_H
#define __MODBASE_H

#include <wx/wx.h>

class wxModuleBase : public wxPanel
{
protected:
    wxBoxSizer* m_sizer;
    virtual void InitBase() {}; 
public:
    wxModuleBase(wxWindow* parent,
			  wxWindowID id = -1,
			  const wxPoint& pos = wxDefaultPosition,
			  const wxSize& size = wxDefaultSize);
    wxModuleBase() {
	   InitBase();
    };
    ~wxModuleBase();
    virtual int CloseDevice() = 0;
    virtual void EnableChildren(bool enable) {};
    virtual int GetEOSSelection() = 0;
    virtual wxString GetEOSString() = 0;
    virtual int GetInputMode() = 0;
    virtual int IsOpen() = 0;
    void OnEraseBackground(wxEraseEvent& event) {
	   m_sizer->Layout();
	   event.Skip();
    };
    void OnSizeEvent(wxSizeEvent& event) {
	   wxSize size = event.GetSize();
	   m_sizer->SetDimension(0,0,size.GetWidth(),size.GetHeight());
	   m_sizer->Layout();
    };
    virtual int OpenDevice(char* name,void* dcs) = 0;
    virtual int ReadDevice(char* buf,size_t len) = 0;
    virtual void ResetDevice() = 0;
    virtual void SetEOSSelection(int selection) = 0;
    virtual void SetEOSString(const wxString& string) = 0;
    virtual void SetInputMode(int mode) = 0;
    virtual void ShowState(bool force) = 0;
    virtual int WriteDevice(char* buf,size_t len) = 0;
    DECLARE_EVENT_TABLE()
};

#endif
