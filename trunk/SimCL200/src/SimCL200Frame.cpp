// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "SimCL200Frame.h"

#include "port_32_16.xpm"
#include "baud_32_16.xpm"
#include "csize_32_16.xpm"
#include "parity_32_16.xpm"
#include "stopb_32_16.xpm"

enum
{
    myID_PORT = wxID_HIGHEST + 1,
    myID_BAUDRATE,
    myID_CHARSIZE,
    myID_STOPBITS,
    myID_PARITY,
};

IMPLEMENT_CLASS(SimCL200Frame, wxFrame)

BEGIN_EVENT_TABLE(SimCL200Frame, wxFrame)
END_EVENT_TABLE()

SimCL200Frame::SimCL200Frame()
{
    Init();
}

SimCL200Frame::SimCL200Frame(wxWindow *parent, wxWindowID id,
                             const wxString &caption, const wxPoint &pos,
                             const wxSize &size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool SimCL200Frame::Create(wxWindow *parent, wxWindowID id,
                           const wxString &caption, const wxPoint &pos,
                           const wxSize &size, long style)
{
    wxFrame::Create(parent, id, caption, pos, size, style);
    CreateControls();
    Centre();
    return true;
}

SimCL200Frame::~SimCL200Frame()
{
}

void SimCL200Frame::Init()
{
}

void SimCL200Frame::CreateControls()
{
    m_ribbon = new wxRibbonBar(this);

    /* serial port page */
    wxRibbonPage *portPage = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("SerialPort"));
    wxRibbonPanel *configPanel = new wxRibbonPanel(portPage, wxID_ANY, wxT("Configuration"), wxNullBitmap,
        wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
    wxRibbonToolBar *configToolBar = new wxRibbonToolBar(configPanel);
    configToolBar->AddTool(wxID_ANY, port_32_16_xpm);
    configToolBar->AddTool(wxID_ANY, baud_32_16_xpm);
    configToolBar->AddTool(wxID_ANY, csize_32_16_xpm);
    configToolBar->AddTool(wxID_ANY, parity_32_16_xpm);
    configToolBar->AddTool(wxID_ANY, stopb_32_16_xpm);
    configToolBar->SetRows(1, 2);

    new wxRibbonPanel(portPage, wxID_ANY, wxT("Connection"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_EXT_BUTTON);

    /* dummy page */
    new wxRibbonPage(m_ribbon, wxID_ANY, wxT("CL-200"));

    m_ribbon->Realize();

    wxTextCtrl *log = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
        wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_LEFT | wxTE_BESTWRAP | wxBORDER_NONE);

    wxSizer *frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(m_ribbon, 0, wxEXPAND);
    frameSizer->Add(log, 1, wxEXPAND);

    SetSizerAndFit(frameSizer);
}

