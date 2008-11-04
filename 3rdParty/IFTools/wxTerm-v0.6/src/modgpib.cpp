/////////////////////////////////////////////////////////////////////////////
// Name:        modgpib.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: modgpib.cpp,v 1.1.1.1 2005/05/11 07:40:07 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "dbgprint.h"
#include "menu.h"
#include "modgpib.h"
#include <wx/ctb-0.13/timer.h>
#ifdef __WIN32__
# include <wx/msw/winundef.h>
#endif

enum {
    ID_BGREAD = wxID_HIGHEST + 1,
    ID_READ
};

struct gpibState_t {
    unsigned int m_errbit;
    const char* m_name;
    const char* m_desc;
};

static gpibState_t gpibStates[16] = {
    {0x0001,"DCAS","Device Clear State"},
    {0x0002,"DTAS","Device Trigger State"},
    {0x0004,"LACS","Listener"},
    {0x0008,"TACS","Talker"},
    {0x0010,"ATN","Attention is asserted"},
    {0x0020,"CIC","Controller-In-Charge"},
    {0x0040,"REM","Remote State"},
    {0x0080,"LOK","Lockout State"},
    {0x0100,"CMPL","I/O completed"},
    {0x0800,"RQS","Device requesting service"},
    {0x1000,"SRQI","SRQ interrupt received"},
    {0x2000,"END","END or EOS detected"},
    {0x4000,"TIMO","time limit exceeded"},
    {0x8000,"ERR","GPIB error"}
};

static gpibState_t gpibDevStates[6] = {
    {0x0004,"QYE","Query error bit"},
    {0x0008,"QUES","Question state register request"},
    {0x0010,"MAV","Message available"},
    {0x0020,"ESB","Event status bit"},
    {0x0040,"MSS","Master summary bit"},
    {0x0080,"OPER","Operation status bit"},
};

static gpibState_t gpibLineStates[8] = {
    {0x0001,"DAV","Data Valid"},
    {0x0002,"NDAC","Not Data Accepted"},
    {0x0004,"NRFD","Not Ready For Data"},
    {0x0008,"IFC","Interface Clear"},
    {0x0010,"REN","Remote Enable"},
    {0x0020,"SQR","Service Request"},
    {0x0040,"ATN","Attention"},
    {0x0080,"EOI","End or Identify"}
};

BEGIN_EVENT_TABLE(wxModuleGPIB, wxPanel)
    EVT_BUTTON(ID_READ,wxModuleGPIB::OnReadButton)
    EVT_CHECKBOX(ID_BGREAD,wxModuleGPIB::OnReadCheckbox)
END_EVENT_TABLE()

wxModuleGPIB::~wxModuleGPIB()
{
};

void wxModuleGPIB::EnableChildren(bool enable)
{
    wxNode *node = GetChildren().First();
    while(node) {
	   ((wxWindow*)node->GetData())->Enable(enable);
	   node = node->Next();
    }
};

void wxModuleGPIB::InitBase()
{
    int i = 0;
    m_readOne = 0;
    int vgap = 8;
    int hgap = 2;

    wxFont *font = new wxFont(8,wxSWISS,wxNORMAL,wxNORMAL);
    wxStaticText* widget;
    m_sizer = new wxBoxSizer(wxHORIZONTAL);

    // box1 (GPIB Status)
    wxStaticBoxSizer* box1 = new wxStaticBoxSizer(
	   new wxStaticBox(this,-1,_("GPIB Status")),
	   wxHORIZONTAL);
    wxGridSizer* b1 = new wxGridSizer(7,vgap,hgap);

    for(i=0;i<14;i++) {
	   widget = new wxStaticText(this,-1,gpibStates[i].m_name);
	   widget->SetToolTip(gpibStates[i].m_desc);
	   widget->SetFont(*font);
	   widget->Enable(false);
	   b1->Add(widget,0,wxALIGN_CENTER,0);
	   m_gpibState[i] = widget;
    }
    m_gpibState[13]->SetForegroundColour(wxColour(255,0,0));

    box1->Add(b1,0,wxCENTER,0);
    m_sizer->Add(box1,0,wxLEFT,4);

    // box2 (Device Status)
    wxStaticBoxSizer* box2 = new wxStaticBoxSizer(
	   new wxStaticBox(this,-1,_("Device Status")),
	   wxHORIZONTAL);
    wxGridSizer* b2 = new wxGridSizer(3,vgap,hgap);
    for(i=0;i<6;i++) {
	   widget = new wxStaticText(this,-1,gpibDevStates[i].m_name);
	   widget->SetToolTip(gpibDevStates[i].m_desc);
	   widget->SetFont(*font);
	   widget->Enable(false);
	   b2->Add(widget,0,wxALIGN_CENTER,0);
	   m_devState[i] = widget;
    }
    box2->Add(b2,0,wxCENTER,0);
    m_sizer->Add(box2,0,wxLEFT,4);

    // box3 (Line Status
    wxStaticBoxSizer* box3 = new wxStaticBoxSizer(
 	   new wxStaticBox(this,-1,_("Line Status")),
 	   wxHORIZONTAL);
    wxGridSizer* b3 = new wxGridSizer(4,vgap,hgap);
    for(i=0;i<8;i++) {
	   widget = new wxStaticText(this,-1,gpibLineStates[i].m_name);
	   widget->SetToolTip(gpibLineStates[i].m_desc);
	   widget->SetFont(*font);
	   widget->Enable(false);
	   b3->Add(widget,0,wxALIGN_CENTER,0);
	   m_lineState[i] = widget;
    }
    box3->Add(b3,0,wxCENTER,0);
    m_sizer->Add(box3,0,wxLEFT,4);

    // box4 (GPIB Errors)
    wxStaticBoxSizer* box4 = new wxStaticBoxSizer(
 	   new wxStaticBox(this,-1,_("Error")),
 	   wxHORIZONTAL);
    wxFlexGridSizer* b4 = new wxFlexGridSizer(2,vgap,hgap);
    widget = new wxStaticText(this,-1,_("No."));
    widget->Enable(false);
    b4->Add(widget,0,wxALIGN_LEFT,0);
    widget = new wxStaticText(this,-1,_(""),
						wxDefaultPosition,wxSize(40,-1));
    widget->Enable(false);
    m_errno = widget;
    b4->Add(widget,0,wxALIGN_LEFT,0);
    widget = new wxStaticText(this,-1,_("ID."));
    widget->Enable(false);
    b4->Add(widget,0,wxALIGN_LEFT,0);
    widget = new wxStaticText(this,-1,_(""),
						wxDefaultPosition,wxSize(40,-1));
    widget->Enable(false);
    m_errid = widget;
    b4->Add(widget,0,wxALIGN_LEFT,0);
    box4->Add(b4,0,wxLEFT,0);
    m_sizer->Add(box4,0,wxLEFT,4);

    // box5 (some controls)
    wxStaticBoxSizer* box5 = new wxStaticBoxSizer(
 	   new wxStaticBox(this,-1,_("Query")),
 	   wxHORIZONTAL);
    wxGridSizer* b5 = new wxGridSizer(1,1,1);
    m_readButton = new wxButton(this,ID_READ,_("Read"));
    // Read Button is disabled and ...
    m_readButton->Enable(false);
    b5->Add(m_readButton,0,wxADJUST_MINSIZE,0);
    m_readbg = new wxCheckBox(this,ID_BGREAD,_("Auto read"));
    // ... background reading is on
    m_readbg->SetValue(true);
    b5->Add(m_readbg,0,wxADJUST_MINSIZE,0);
    box5->Add(b5,0,wxADJUST_MINSIZE,0);

    // expand the not needed space
    m_sizer->Add(10,0,wxEXPAND|wxALL,1);
    m_sizer->Add(box5,0,wxRIGHT,0);
    
    SetSizerAndFit(m_sizer);
};

void wxModuleGPIB::OnReadButton(wxCommandEvent& event)
{
    // request for only one message
    m_readOne = 1;
    // disable both, checkbox and button during the request
    // (until read is successfull or an error occurs)
    m_readButton->Enable(false);
    m_readbg->Enable(false);
};

void wxModuleGPIB::OnReadCheckbox(wxCommandEvent& event)
{
    m_readButton->Enable(!event.IsChecked());
};

int wxModuleGPIB::ReadDevice(char* buf,size_t len)
{
    if(!m_dev.IsOpen()) {
	   return 0;
    }
    if(m_readOne) {
	   int state = 0;
	   int rd = m_dev.Read(buf,len);
	   m_dev.Ioctl(CTB_GPIB_GETSTA,&state);
	   if((state & 0x8000) || ((state & 0x2100) == 0x2100)) {
		  m_readOne = 0;
		  m_readButton->Enable(true);
		  m_readbg->Enable(true);
	   }
	   return rd;
	   
    }
    else if(m_readbg->IsChecked()) {
	   return m_dev.Read(buf,len);
    }
    return 0;
};

void wxModuleGPIB::ResetDevice()
{
    if(m_dev.IsOpen()) {
	   m_dev.Ioctl(CTB_RESET,NULL);
	   m_readbg->Enable(true);
	   m_readButton->Enable(false);
    }
};

void wxModuleGPIB::ShowState(bool force)
{
    if(m_dev.IsOpen()) {
	   static int lastError = 0;
	   static int lastLines = 0;
	   wxColour level[2];
	   level[0].Set(0x80,0x80,0xff);
	   level[1].Set(0xff,0x80,0x40);

	   int i = 0;
	   int m = 4;
	   // request serial poll register
	   int spr = 0;
	   m_dev.Ioctl(CTB_GPIB_GETRSP,&spr);
	   // request gpib status
	   int state = 0;
	   m_dev.Ioctl(CTB_GPIB_GETSTA,&state);
	   // request error
	   int error = 0;
	   m_dev.Ioctl(CTB_GPIB_GETERR,&error);
	   // request line status
	   int lines = 0;
	   m_dev.Ioctl(CTB_GPIB_GETLINES,&lines);

	   // show state of serial poll register
	   for(m=0x04;m<=0x80;m<<=1) {
		  m_devState[i++]->Enable(spr & m);
	   }
	   // show state of gpib
	   i = 0;
	   for(m=0x01;m<=0x100;m<<=1) {
		  m_gpibState[i++]->Enable(state & m);
	   }
	   for(m=0x800;m<=0x8000;m<<=1) {
		  m_gpibState[i++]->Enable(state & m);
	   }
	   // show line state
	   i = 0;
	   if(lines != lastLines) {
		  int lm = lines & 0xFF;
		  int ls = (lines >> 8) & 0xFF;
		  int i=0;
		  for(m=0x01;m<=0x80;m<<=1) {
			 m_lineState[i]->SetForegroundColour(level[(ls & m) != 0]);
			 m_lineState[i++]->Enable(lm & m);
		  }
		  lastLines = lines;
	   }
	   // show error
	   if(error != lastError) {
		  m_errno->Enable(state & 0x8000);
		  m_errid->Enable(state & 0x8000);
		  if(state & 0x8000) {
			 char buf[8];
			 wxString s;
			 s.Printf("%i",error);
			 // an error occured
			 m_errno->SetLabel(s);
			 m_errid->SetLabel( m_dev.GetErrorNotation( error ) );
		  }
		  else {
			 m_errno->SetLabel("");
			 m_errid->SetLabel("");
		  }
		  lastError = error;
	   }
	   // disable checkbox for background reading if device not opened
	   if(!m_dev.IsOpen() & m_readbg->IsEnabled()) {
		  m_readbg->Enable(false);
	   }
	   else if(m_dev.IsOpen() & !m_readbg->IsEnabled()) {
		  m_readbg->Enable(true);
	   }
    }
};

int wxModuleGPIB::WriteDevice(char* buf,size_t len)
{
    if(m_dev.IsOpen()) {
	   int to = 0;
	   timer t(100,&to,NULL);
	   t.start();
	   int wr = m_dev.Writev(buf,len,&to);
	   return wr;
    }
    return 0;
};
