/////////////////////////////////////////////////////////////////////////////
// Name:        dataview.cpp
// Purpose:     
// Author:      Joachim Buermann
// Id:          $Id: dataview.cpp,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "dataview.h"
#include "dbgprint.h"

wxDView::wxDView(wxWindow* parent,
			  wxWindowID id,
			  const wxPoint& pos,
			  const wxSize& size,
			  colors_t* colors,
			  long style) :
    wxPanel(parent,id,pos,size)
{
    BaseInit();
    wxBoxSizer* top = new wxBoxSizer(wxHORIZONTAL);
    m_view = new wxDataWin(this,-1,
					  wxDefaultPosition,wxDefaultSize,
					  m_array,
					  colors,
					  style);
    top->Add(m_view,1,wxEXPAND|wxALL,0);
    m_sb = new wxScrollBar(this,-1,
								wxDefaultPosition,wxDefaultSize,
								wxVERTICAL);
    m_sb->SetScrollbar(0,1,1,1);
    top->Add(m_sb,0,wxEXPAND|wxTOP|wxBOTTOM,0);
    // actually set the sizer   
    SetAutoLayout(true);
    // I don't know, what happens, but fact is:
    // win32 doesn't open the program window (it's only appears
    // in the taskmanager and eats some memory), if SetSizerAndFit
    // is used.
    // On the other hand, linux shows a CRITICAL **: file pango-layout.c
    // error without the SetSizerAndFit call. If you ignore this, 
    // the program starts normally ?!?
    // Maybe someone can fix this!
#ifdef __WIN32__
    SetSizer(top);
#else
    SetSizerAndFit(top);
#endif
};

BEGIN_EVENT_TABLE(wxDView, wxPanel)
    EVT_SCROLL(wxDView::OnScrollEvent)
    EVT_SCROLL_TOP(wxDView::OnScrollEvent)
    EVT_SCROLL_BOTTOM(wxDView::OnScrollEvent)
    EVT_SCROLL_LINEUP(wxDView::OnScrollEvent)
    EVT_SCROLL_LINEDOWN(wxDView::OnScrollEvent)
    EVT_SCROLL_PAGEUP(wxDView::OnScrollEvent)
    EVT_SCROLL_PAGEDOWN(wxDView::OnScrollEvent)
    EVT_SCROLL_THUMBTRACK(wxDView::OnScrollEvent)
    EVT_SCROLL_THUMBRELEASE(wxDView::OnScrollEvent)
    EVT_SCROLL_ENDSCROLL(wxDView::OnScrollEvent)
END_EVENT_TABLE()

wxDView::~wxDView()
{
    delete m_view;
    delete m_array;
};

// recalculate the scrollbar settings (position, thumb size,
// thumb position
void wxDView::CalculateScrollBar()
{
    int position = 0;
    int range = 1;
    int thumb = 1;
    // In ASCII mode, the scrollbar range is set from 0 to the
    // count of received lines (identified by CR or LF)
    // The scrollbar thumb size is the count of displayed lines
    if(m_view->GetMode() == wxDUMP_AS_ASCII) {
	   range = m_lines;
	   thumb = m_view->GetVerticalPageOffset();
    }
    // in HEX view the scrollbar range is the completely recieved
    // bytes divided by the hex columns in each line
    else {
	   if(m_view->GetVerticalLineOffset() > 0) {
		  range = m_array->GetCount() / m_view->GetVerticalLineOffset();
		  thumb = m_view->GetVerticalPageOffset()  / 
			 m_view->GetVerticalLineOffset();
	   }
    }
    if(thumb <= 0) thumb = 1;
    if(m_view->IsAutoScrolled()) {
	   position = range - thumb;
    }
    else {
	   position = m_sb->GetThumbPosition();
    }
    if(position < 0) position = 0;
    m_sb->SetScrollbar(position,thumb,range+1,thumb-1);
    // dbgprintf("SET <-  %i[##%i##]  %i->\n",position,thumb,range+1);
};

void wxDView::OnScrollEvent(wxScrollEvent& event)
{
//     dbgprintf("SCR <-  %i[##%i##]  %i->\n",
// 		    m_sb->GetThumbPosition(),
// 		    m_sb->GetThumbSize(),
// 		    m_sb->GetRange());
    m_view->SetPosition(event.GetPosition() * m_view->GetVerticalLineOffset());
};

void wxDView::Redraw()
{
    m_view->Redraw();
    CalculateScrollBar();
};

void wxDView::SetMode(wxDataWinMode mode)
{
    CalculateScrollBar();
    m_view->SetMode(mode);
};
