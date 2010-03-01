// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include <iphlpapi.h>
#include "WidgetId.h"
#include "TargetsPane.h"
#include "SearchThread.h"
#include "UpdaterApp.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
BEGIN_EVENT_TABLE(TargetsPane, wxPanel)
    EVT_BUTTON(myID_TARGET_SEARCH_BTN, TargetsPane::OnSearchButtonClicked)
    EVT_THREAD(myID_SEARCH_THREAD, TargetsPane::OnSearchThread)
END_EVENT_TABLE()

TargetsPane::TargetsPane()
{
    Init();
}

TargetsPane::TargetsPane(wxWindow *parent, wxWindowID id, 
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

TargetsPane::~TargetsPane()
{

}

void TargetsPane::Init()
{

}

bool TargetsPane::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void TargetsPane::CreateControls()
{
    wxBoxSizer *bgSizer = new wxBoxSizer(wxVERTICAL);

    bgSizer->Add(new wxButton(this, myID_TARGET_SEARCH_BTN, wxT("Search")), 0, wxALL, 5);
    bgSizer->Add(new wxButton(this, wxID_ANY, wxT("Cancel")), 0, wxALL, 5);

    SetSizer(bgSizer);
}

//
// event handlers
//
void TargetsPane::OnSearchButtonClicked(wxCommandEvent &event)
{
    SearchThread *thread = new SearchThread(this);
    if (thread
        && (thread->Create() == wxTHREAD_NO_ERROR)
        && (thread->Run() == wxTHREAD_NO_ERROR))
    {
        wxButton *btn = wxDynamicCast(FindWindow(event.GetId()), wxButton);
        btn->Enable(false);
    }
}

void TargetsPane::OnSearchThread(wxThreadEvent &event)
{
    SearchThreadMessage msg = event.GetPayload<SearchThreadMessage>();
    wxButton *btn;

    switch (msg.type)
    {
    case SEARCH_THREAD_COMPLETED:
        wxLogMessage(wxT("Target search thread is completed!"));
        btn = wxDynamicCast(FindWindow(myID_TARGET_SEARCH_BTN), wxButton);
        if (btn) btn->Enable(true);
        break;
    case SEARCH_THREAD_TARGET_FOUND:
        wxLogMessage(wxT("Target found: name(%s), ip(%s), mac(%s)"),
            msg.name, msg.ip, msg.mac);
        break;
    default:
        break;
    }
}
