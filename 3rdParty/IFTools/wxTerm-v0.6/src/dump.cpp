/////////////////////////////////////////////////////////////////////////////
// Name:        dump.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: dump.cpp,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "dump.h"

IMPLEMENT_DYNAMIC_CLASS( wxDataWinEvent, wxEvent )
DEFINE_EVENT_TYPE( wxEVT_DUMP )
DEFINE_EVENT_TYPE( wxEVT_DUMP_END )

wxDataWinEvent::wxDataWinEvent(wxWindow* win)
{
    SetEventType(wxEVT_DUMP);
    SetEventObject(win);
};

wxDataWin::wxDataWin(wxWindow* parent,
				 wxWindowID id,
				 const wxPoint& pos,
				 const wxSize& size,
				 Array* array,
 				 colors_t* colors,
				 long style) :
    wxWindow(parent,id,pos,size,style)
{
    m_colors = colors;
    m_bitmap = new wxBitmap(10,12);
    m_font = new wxFont(12,wxMODERN,wxNORMAL,wxBOLD);

    int w,h;
    wxBitmap tmpbmp(100,100);
    wxMemoryDC dc;
    dc.SelectObject(tmpbmp);
    dc.SetFont(*m_font);
    dc.GetTextExtent("Xg",&w,&h);
    dc.SelectObject(wxNullBitmap);
    m_char.Set((w>>1),h);

    m_eostbl.Set('\n');
    m_eostbl.Set('\r');

    m_mode = wxDUMP_AS_ASCII;
    m_array = array;
    m_offset = 0;
    m_autoscroll = true;

    memset(m_asciitbl,'.',sizeof(m_asciitbl));
    for(char c=' ';c<'~';c++) { m_asciitbl[c] = c; }

    m_cols = 80;
    m_rows = 25;
    m_buf = new char[(m_cols+1) * m_rows];
};

BEGIN_EVENT_TABLE(wxDataWin, wxWindow)
    EVT_PAINT(wxDataWin::OnPaint)
    EVT_SIZE(wxDataWin::OnSizeEvent)
END_EVENT_TABLE()

wxDataWin::~wxDataWin()
{
    delete[] m_buf;
    delete m_bitmap;
    delete m_font;
};

size_t wxDataWin::GetOrigin()
{
    char ch;
    int n = m_array->GetCount();
    int m = 1;
    size_t ls = 0;
    // linebreaks counter
    size_t lb = m_rows;
    if(n == 0) return 0;
    // array index starts with 0, so n-1 indexed the last character
    // in the array
    n--;
    while((n >= 0) && lb) {
	   m_array->Get(ch,n);
	   if(m_eostbl.Get(ch)) {
		  // linebreak character cr or lf
		  lb--;
		  ls = n+1;
		  m = 0;
	   }
	   else {
		  m++;
	   }
	   // if the count of characters between two linebreaks 
	   // is greater as the allowed width (m_cols), a additional
	   // linebreak must added
	   if(m >= m_cols) {
		  lb--;
		  m = 0;
	   }
	   n--;
    }
    if(lb) {
	   return 0;
    }
    return ls;
};

void wxDataWin::DrawAsAscii(wxDC& dc)
{
    char buf[256];
    char* cp;
    char ch;
    int y = 0;
 
    if(m_autoscroll) {
	   m_offset = GetOrigin();
    }
    size_t offset = m_offset;
    memset(buf,0,sizeof(buf));
    for(int row = 0;row < m_rows;row++) {
	   cp = buf;
	   for(int col = 0;col < m_cols;col++) {
		  if(!m_array->Get(ch,offset++)) {
			 break;
		  }
		  if(m_eostbl.Get(ch)) {
			 break;
		  }
		  else if(ch < ' ') {
			 *cp++ = ' ';
		  }
		  else {
			 *cp++ = ch;
		  }
	   }
	   *cp = 0;
	   dc.DrawText(buf,4,y);
	   y += m_char.GetY();
    }
};

void wxDataWin::DrawAsHex(wxDC& dc)
{
    // data displayed like a hex editor
    // xx xx xx xx xx xx xx xx|aaaaaaaa
    // means 4 characters for each sign (2 hex + space + ascii).
    int n = 0;
    int cols = (m_cols-1) >> 2;
    char hexline[64];
    char ascline[64];

    int border = m_hx * cols;

    if(!cols) return;

    if(m_autoscroll) {
	   n = m_array->GetCount();
	   n += n % cols;
	   n -= (m_rows * cols);
	   if(n < 0) n = 0;
	   m_offset = n;
    }

    char *cphex;
    char* cpasc;
    char ch;
    int y = 0;
    int col = 0;
    int row = 0;
    size_t offset = m_offset;

    memset(hexline,0,sizeof(hexline));

    for(row = 0; row < m_rows; row++) {
	   cphex = hexline;
	   cpasc = ascline;
	   memset(ascline,0,sizeof(ascline));
	   for(col = 0; col < cols; col++) {
		  if(!m_array->Get(ch,offset++)) {
			 DrawTextAsHex(dc,hexline,col,4,y);
			 dc.SetTextForeground(*wxBLUE);
			 dc.DrawText(ascline,border+12,y);
			 return;
		  }
		  *cphex++ = ch;
		  *cpasc++ = m_asciitbl[ch];
	   }
	   *cphex = 0;
	   *cpasc = 0;
	   DrawTextAsHex(dc,hexline,cols,4,y);
	   dc.SetTextForeground(*wxBLUE);
	   dc.DrawText(ascline,border+12,y);
	   y += m_char.GetY();
    }
};

void wxDataWin::DrawOnBitmap()
{
    wxSize s = GetClientSize();
    if((m_bitmap->GetWidth() != s.GetWidth()) || 
	  (m_bitmap->GetHeight() != s.GetHeight())) {
	   m_bitmap->Create(s.x,s.y);
    }
    wxMemoryDC dc;
    dc.SelectObject(*m_bitmap);
    dc.SetFont(*m_font);
    dc.GetTextExtent("XXX",&m_hx,&m_hy);
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.Clear();

    if(m_mode == wxDUMP_AS_HEX) {
	   DrawAsHex(dc);
    }
    else {
	   DrawAsAscii(dc);
    }
    dc.SelectObject(wxNullBitmap);
};

static wxColour colors[8] = {
    wxColour(0xde,0x13,0x00),	  // dark red
    wxColour(0xe7,0x68,0x1e),	  // dark orange
    wxColour(0xe7,0xd5,0x0a),	  // dark yellow
    wxColour(0x3a,0x94,0x1e),	  // dark green
    wxColour(0x35,0x01,0x9f),	  // dark blue
    wxColour(0x85,0x48,0x85),	  // dark violet
    wxColour(0x7e,0x7e,0x7e),	  // grey
    wxColour(0,0,0)			  // black
};

void wxDataWin::DrawTextAsHex(wxDC& dc,
						char* text,
						size_t len,
						wxCoord x,wxCoord y)
{
    static char hextbl[16] = {
	   '0','1','2','3','4','5','6','7',
	   '8','9','A','B','C','D','E','F'
    };
    char hexchar[4];
    char* cp = hexchar;
    int cindex = 0;
    hexchar[3] = 0;
    while(len--) {
	   cp = hexchar;
	   *cp++ = hextbl[(*text >> 4) & 0x0f];
	   *cp++ = hextbl[*text & 0x0f];
	   *cp++ = ' ';
	   // check for color rules
	   dc.SetTextForeground(*wxBLACK);
	   for(int i=0;i<=4;i++) {
		  if(IsColorRule(m_colors[i],*text,cindex)) {
			 dc.SetTextForeground(colors[cindex & 7]);
			 break;
		  }
	   };
	   dc.DrawText(hexchar,x,y);
	   x += m_hx;
	   text++;
    }
};

void wxDataWin::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*m_bitmap,0,0,false);
};

/*
  OnSizeEvent will be called on win32 systems only if the dimension
  off the windows has REALLY changes. It wouldn't be called if the
  window was iconified/maximized or at program start!!!
  On Gtk2 OnSizeEvent was fired at program start, but not at
  iconification/maximizing!!!
*/
void wxDataWin::OnSizeEvent(wxSizeEvent& event)
{
    // ask the event for the current size...
    m_size = event.GetSize();
    // and calculate the rows and columns
    delete m_buf;
    m_rows = m_size.GetHeight() / m_char.GetY();
    m_cols = m_size.GetWidth() / m_char.GetX();
    m_bufsize = (m_cols+1) * m_rows;
    m_buf = new char[m_bufsize];
    DrawOnBitmap();
};

void wxDataWin::Redraw()
{
    wxClientDC dc(this);
    DrawOnBitmap();
    dc.DrawBitmap(*m_bitmap,0,0,false);
};

void wxDataWin::SetPosition(size_t position)
{
    if(GetMode() == wxDUMP_AS_HEX) {
	   // in HEX mode position is a byte number
	   m_offset = position;
    }
    else {
	   // in ASCII mode position is a line number, so we
	   // look for the line number(position)
	   char ch;
	   size_t line = 0;
	   for(size_t n=0;n <= m_array->GetCount();n++) {
		  m_array->Get(ch,n);
		  if((ch == '\r') || (ch == '\n')) {
			 line++;
			 if(line == position) {
				m_offset = n+1;
			 }
		  }
	   }
    }
    Redraw();
};
