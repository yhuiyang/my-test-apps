/////////////////////////////////////////////////////////////////////////////
// Name:        dataview.h
// Purpose:     
// Author:      Joachim Buermann
// Id:          $Id: dataview.h,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __DATAVIEW_H
#define __DATAVIEW_H

#include <wx/awx-0.4/awx.h>
#include "array.h"
#include "dump.h"
#include "prefs.h"

class wxDView : public wxPanel
{
protected:
    Array* m_array;
    wxDataWin* m_view;
    wxScrollBar* m_sb;
    size_t m_lines;
public:
    wxDView(wxWindow* parent,
		  wxWindowID id,
		  const wxPoint& pos,
		  const wxSize& size,
		  colors_t* colors,
		  long style);
    ~wxDView();
    void AppendData(char* buf,size_t bufsize) {
	   // count the line breaks to calculate the correect dimension
	   // of the scrollbar in the ascii mode
	   for(size_t i=0;i<bufsize;i++) {
		  if((buf[i] == '\n') || (buf[i] == '\r')) m_lines++;
	   }
	   m_array->Add(buf,bufsize);
	   Redraw();
    };
    void BaseInit() {
	   // initialisation of all non wxWindow elements
	   m_array = new Array(64*1024);
	   m_lines = 0;
    };
    void CalculateScrollBar();
    void Clear() {
	   m_lines = 0;
	   m_array->Clear();
	   m_view->Clear();
	   Redraw();
    };
    size_t GetCount() {
	   return m_array->GetCount();
    };
    size_t Read(char* buf,size_t bufsize,size_t& pos) {
	   return m_array->Read(buf,bufsize,pos);
    };
    void Redraw();
    void SetAutoscroll(bool enable) {
	   m_sb->Enable(enable == false);
	   m_view->SetAutoscroll(enable);
    };
    void SetEOS(char eos) {m_view->SetEOS(eos);};
    void SetMode(wxDataWinMode mode);
protected:
    void OnScrollEvent(wxScrollEvent& event);
    DECLARE_EVENT_TABLE()
};

#endif
