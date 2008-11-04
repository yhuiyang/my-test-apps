/////////////////////////////////////////////////////////////////////////////
// Name:        dump.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: dump.h,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __WXDUMP_H
#define __WXDUMP_H

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "bits_t.h"
#include "array.h"
#include "prefs.h"

class wxDataWinEvent : public wxEvent
{
    DECLARE_DYNAMIC_CLASS( wxDataWinEvent ) 
public:
    wxDataWinEvent(wxWindow* win = (wxWindow*)NULL);
    wxEvent* Clone(void) const {return new wxDataWinEvent(*this); };
private:
};

typedef void (wxEvtHandler::*wxDataWinEventFunction)(wxDataWinEvent&);

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_DATAWIN, 1)
    DECLARE_EVENT_TYPE(wxEVT_DATAWIN_END, 2)
END_DECLARE_EVENT_TYPES()

#define EVT_DATAWIN(func) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_DATAWIN, \
                              -1, \
                              -1, \
                              (wxObjectEventFunction) \
                              (wxDumpEventFunction) & func, \
                              (wxObject *) NULL ),

#define EVT_DATAWIN_END(func) \
    DECLARE_EVENT_TABLE_ENTRY(wxEVT_DATAWIN_END, \
                              -1, \
                              -1, \
                              (wxObjectEventFunction) \
                              (wxDumpEventFunction) & func, \
                              (wxObject *) NULL ),

enum wxDataWinMode {
    wxDUMP_AS_ASCII = 0,
    wxDUMP_AS_HEX
};

class wxDataWin : public wxWindow
{
protected:
    wxBitmap* m_bitmap;
    wxFont* m_font;
    wxSize m_size;
    wxSize m_char;
    unsigned int m_cols;
    unsigned int m_rows;
    char* m_buf;
    size_t m_bufsize;
    size_t m_offset;
    char m_asciitbl[256];
    int m_hx,m_hy;

    // pointer to color table
    colors_t* m_colors;

    // bitmap for eos
    bits_t(256);
    bits_t_256 m_eostbl;

    Array *m_array;

    wxDataWinMode m_mode;
    bool m_autoscroll;

    void DrawAsAscii(wxDC& dc);
    void DrawAsHex(wxDC& dc);
    void DrawData(wxDC& dc);
    void DrawOnBitmap();
    void DrawTextAsHex(wxDC& dc,
				   char* text,
				   size_t len,
				   wxCoord x, wxCoord y);
public:
    wxDataWin(wxWindow* parent,
		    wxWindowID id,
		    const wxPoint& pos,
		    const wxSize& size,
		    Array* array,
		    colors_t* colors,
		    long style = 0);
    ~wxDataWin();
    void Clear() {m_offset = 0;};
    wxDataWinMode GetMode() {return m_mode;};
    // gives the internal offset (position)
    size_t GetOffset() {return m_offset;};
    size_t GetOrigin();
    // gives the vertical scroll offset for one line.
    // In ASCII view it's one line, in HEX view the count of bytes 
    // displayed in one line
    size_t GetVerticalLineOffset() {
	   if(m_mode == wxDUMP_AS_ASCII) return 1;
	   else return (m_cols-1) >> 2;
    };
    // gives the vertical scroll offset for one page.
    // In ASCII view it's the count of displayed lines, in HEX view
    // the count of displayed hex data
    size_t GetVerticalPageOffset() {
	   return GetVerticalLineOffset() * m_rows;
    };
    bool IsAutoScrolled() {return m_autoscroll;};
    void OnPaint(wxPaintEvent& event);
    void OnSizeEvent(wxSizeEvent& event);
    void Redraw();
    void SetAutoscroll(bool enable) {
	   m_autoscroll = enable;
    };
    void SetEOS(char eos) {
	   m_eostbl.Clrall();
	   m_eostbl.Set(eos);
    };
    void SetMode(wxDataWinMode mode) {
	   m_mode = mode;
    };
    void SetPosition(size_t offset);
    DECLARE_EVENT_TABLE()
};

#endif
