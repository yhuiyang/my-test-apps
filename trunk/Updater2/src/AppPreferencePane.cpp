// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>
#include "UpdaterApp.h"
#include "AppPreferencePane.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
AppPreferencePane::AppPreferencePane()
{
    Init();
}

AppPreferencePane::AppPreferencePane(wxWindow *parent, wxWindowID id,
                                     const wxPoint &pos, const wxSize &size,
                                     long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

AppPreferencePane::~AppPreferencePane()
{

}

void AppPreferencePane::Init()
{

}

bool AppPreferencePane::Create(wxWindow *parent, wxWindowID id,
                               const wxPoint &pos, const wxSize &size,
                               long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void AppPreferencePane::CreateControls()
{
    wxNotebook *prefNB = new wxNotebook(this, wxID_ANY);

    //
    // network page
    //
    wxPanel *networkPage = new wxPanel(prefNB, wxID_ANY);
    wxStaticBoxSizer *actIfSizer = new wxStaticBoxSizer(wxVERTICAL, networkPage, _("Actived Interface"));
    actIfSizer->Add(new wxStaticText(networkPage, wxID_STATIC, _("Please choose an active interface from system valid interfaces:")), 0, wxALL, 5);
    wxVector<NetAdapter> &adapters = wxGetApp().m_Adapters;
    wxArrayString ifArrayStr;
    wxString ifStr, activedStr = wxGetApp().m_pAppOptions->GetOption(wxT("ActivedInterface"));
    wxVector<NetAdapter>::iterator it;
    for (it = adapters.begin(); it != adapters.end(); ++it)
    {
        ifStr.clear();
        ifStr << wxT("IP: [") << it->GetIp() << wxT("] Name: [") << it->GetName() << wxT("]");
        ifArrayStr.push_back(ifStr);

        if (ifStr.Contains(activedStr))
            activedStr = ifStr;
    }
    wxChoice *ifChoice = new wxChoice(networkPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, ifArrayStr);
    ifChoice->SetStringSelection(activedStr);
    ifChoice->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &AppPreferencePane::OnUpdateActivedInterface, this);
    actIfSizer->Add(ifChoice, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 5);
    wxCheckBox *skipVMif = new wxCheckBox(networkPage, wxID_ANY, _("Skip interfaces generated by VMware. (Need restart application)"));
    if (!wxGetApp().m_pAppOptions->GetOption(wxT("SkipVMInterface")).Cmp(wxT("Yes")))
        skipVMif->SetValue(true);
    skipVMif->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &AppPreferencePane::OnUpdateSkipVMInterface, this);
    actIfSizer->Add(skipVMif, 0, wxALL | wxEXPAND, 5);

    wxStaticBoxSizer *searchSizer = new wxStaticBoxSizer(wxVERTICAL, networkPage, _("Search Method"));
    searchSizer->Add(new wxStaticText(networkPage, wxID_STATIC, _("Broadcast address:")), 0, wxLEFT | wxRIGHT | wxTOP, 5);
    wxRadioButton *searchMethod1 = new wxRadioButton(networkPage, wxID_ANY, _("Use subnet broadcast address"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    wxRadioButton *searchMethod2 = new wxRadioButton(networkPage, wxID_ANY, _("Use 255.255.255.255"));
    bool useMethod1 = true;
    long lMethod;
    if (wxGetApp().m_pAppOptions->GetOption(wxT("SearchMethod")).ToLong(&lMethod))
    {
        if (lMethod == 0)
            useMethod1 = true;
        else
            useMethod1 = false;
    }
    searchMethod1->SetValue(useMethod1 ? true : false);
    searchMethod2->SetValue(useMethod1 ? false : true);
    searchMethod1->Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &AppPreferencePane::OnUseSearchMethod1, this);
    searchMethod2->Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &AppPreferencePane::OnUseSearchMethod2, this);
    searchSizer->Add(searchMethod1, 0, wxLEFT | wxRIGHT | wxTOP, 5);
    searchSizer->Add(searchMethod2, 0, wxALL, 5);
    wxBoxSizer *numSearchSizer = new wxBoxSizer(wxHORIZONTAL);
    numSearchSizer->Add(new wxStaticText(networkPage, wxID_STATIC, _("Number of sending broadcast packet:")), 0, wxALL | wxALIGN_CENTER, 0);
    long lNumSearch = 3;
    wxGetApp().m_pAppOptions->GetOption(wxT("SearchCount")).ToLong(&lNumSearch);
    wxSpinCtrl *numSearch = new wxSpinCtrl(networkPage, wxID_ANY, wxString::Format(wxT("%d"), lNumSearch), wxDefaultPosition, wxSize(50, -1));
    numSearch->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &AppPreferencePane::OnUpdateNumberOfSearch, this);
    numSearch->SetRange(1, 10);
    numSearchSizer->Add(numSearch, 0, wxLEFT, 5);
    searchSizer->Add(numSearchSizer, 0, wxLEFT, 5);

    wxBoxSizer *netSizer = new wxBoxSizer(wxVERTICAL);
    netSizer->Add(actIfSizer, 0, wxALL | wxEXPAND, 5);
    netSizer->Add(searchSizer, 0, wxALL | wxEXPAND, 5);
    networkPage->SetSizer(netSizer);

    //
    // MAC address page
    //
    wxPanel *macPage = new wxPanel(prefNB, wxID_ANY);
    wxStaticBoxSizer *macPoolSizer = new wxStaticBoxSizer(wxVERTICAL, macPage, _("Pool Range"));
    wxBoxSizer *vendorMACSizer = new wxBoxSizer(wxHORIZONTAL);
    vendorMACSizer->Add(new wxStaticText(macPage, wxID_STATIC, _("Default MAC Vendor code:")), 0, wxRIGHT | wxALIGN_CENTER, 5);
    wxTextCtrl *vendorMAC = new wxTextCtrl(macPage, wxID_ANY); // TODO: replace me
    vendorMACSizer->Add(vendorMAC, 0, wxLEFT, 5);
    macPoolSizer->Add(vendorMACSizer, 0, wxALL, 5);
    wxBoxSizer *startProductMACSizer = new wxBoxSizer(wxHORIZONTAL);
    startProductMACSizer->Add(new wxStaticText(macPage, wxID_STATIC, _("MAC Product code begin at:")), 0, wxRIGHT | wxALIGN_CENTER, 5);
    wxTextCtrl *startProductMAC = new wxTextCtrl(macPage, wxID_ANY); // TODO: replace me
    startProductMACSizer->Add(startProductMAC, 0, wxLEFT, 5);
    macPoolSizer->Add(startProductMACSizer, 0, wxALL, 5);
    wxBoxSizer *endProductMACSizer = new wxBoxSizer(wxHORIZONTAL);
    endProductMACSizer->Add(new wxStaticText(macPage, wxID_STATIC, _("MAC Product code end at:")), 0, wxRIGHT | wxALIGN_CENTER, 5);
    wxTextCtrl *endProductMAC = new wxTextCtrl(macPage, wxID_ANY); // TODO: replace me
    endProductMACSizer->Add(endProductMAC, 0, wxLEFT, 5);
    macPoolSizer->Add(endProductMACSizer, 0, wxALL, 5);

    wxStaticBoxSizer *invalidMacSizer = new wxStaticBoxSizer(wxVERTICAL, macPage, _("Invalid MAC Address"));
    invalidMacSizer->Add(new wxStaticText(macPage, wxID_STATIC, _("Request user doing update when target board with this MAC address:")), 0, wxALL, 5);
    wxTextCtrl *invalidMac = new wxTextCtrl(macPage, wxID_ANY); // TODO: replace me
    invalidMacSizer->Add(invalidMac, 0, wxLEFT | wxRIGHT, 5);

    wxStaticBoxSizer *reportFileSizer = new wxStaticBoxSizer(wxVERTICAL, macPage, _("Report File"));
    wxBoxSizer *reportDirSizer = new wxBoxSizer(wxHORIZONTAL);
    reportDirSizer->Add(new wxStaticText(macPage, wxID_STATIC, _("Report folder:")), 0, wxRIGHT | wxALIGN_CENTER, 5);
    reportDirSizer->Add(new wxDirPickerCtrl(macPage, wxID_ANY), 1, wxLEFT, 5);
    reportFileSizer->Add(reportDirSizer, 0, wxALL | wxEXPAND, 5);
    wxBoxSizer *autoRotateSizer = new wxBoxSizer(wxHORIZONTAL);
    autoRotateSizer->Add(new wxStaticText(macPage, wxID_STATIC, _("Automatically rotate report file?")), 0, wxRIGHT | wxALIGN_CENTER, 5);
    wxArrayString rotateString;
    rotateString.push_back(_("Never"));
    rotateString.push_back(_("Every year"));
    rotateString.push_back(_("Every month"));
    rotateString.push_back(_("Every week"));
    rotateString.push_back(_("Limit entries"));
    wxChoice *rotateChoice = new wxChoice(macPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, rotateString);
    autoRotateSizer->Add(rotateChoice, 0, wxLEFT, 5);
    reportFileSizer->Add(autoRotateSizer, 0, wxALL, 5);
    wxBoxSizer *limitEntriesSizer = new wxBoxSizer(wxHORIZONTAL);
    limitEntriesSizer->Add(new wxStaticText(macPage, wxID_STATIC, _("Maximun entries in a sigle report file:")), 0, wxRIGHT | wxALIGN_CENTER, 5);
    limitEntriesSizer->Add(new wxTextCtrl(macPage, wxID_ANY), 0, wxLEFT, 5);
    reportFileSizer->Add(limitEntriesSizer, 0, wxALL, 5);

    wxBoxSizer *macSizer = new wxBoxSizer(wxVERTICAL);
    macSizer->Add(macPoolSizer, 0, wxALL | wxEXPAND, 5);
    macSizer->Add(invalidMacSizer, 0, wxALL | wxEXPAND, 5);
    macSizer->Add(reportFileSizer, 0, wxALL | wxEXPAND, 5);
    macPage->SetSizer(macSizer);

    prefNB->AddPage(networkPage, _("Network"), true);
    prefNB->AddPage(macPage, _("MAC Address"), false);

    wxBoxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);
    paneSizer->Add(prefNB, 1, wxALL | wxEXPAND, 5);
    SetSizer(paneSizer);
}

void AppPreferencePane::OnUpdateSearchMethod(const int method)
{
    if ((method >= 0) && (method <= 1))
        wxGetApp().m_pAppOptions->SetOption(wxT("SearchMethod"), wxString::Format(wxT("%d"), method));
}

// event handlers
void AppPreferencePane::OnUpdateActivedInterface(wxCommandEvent &event)
{
    wxString ifChoiceString = event.GetString();
    size_t ifNamePosStart = ifChoiceString.Find(wxT("Name: ["));
    if (ifNamePosStart != wxNOT_FOUND)
    {
        /* length calculate is based on when we formated the string in the wxChoice */
        size_t ifNameLen = (ifChoiceString.length() - 1) - (ifNamePosStart + 7);
        wxString ifNameString = ifChoiceString.Mid(ifNamePosStart + 7, ifNameLen);
        wxGetApp().m_pAppOptions->SetOption(wxT("ActivedInterface"), ifNameString);
    }
}

void AppPreferencePane::OnUpdateSkipVMInterface(wxCommandEvent &event)
{
    wxGetApp().m_pAppOptions->SetOption(wxT("SkipVMInterface"), event.IsChecked() ? wxT("Yes") : wxT("No"));
}

void AppPreferencePane::OnUseSearchMethod1(wxCommandEvent &WXUNUSED(event))
{
    OnUpdateSearchMethod(0);
}

void AppPreferencePane::OnUseSearchMethod2(wxCommandEvent &WXUNUSED(event))
{
    OnUpdateSearchMethod(1);
}

void AppPreferencePane::OnUpdateNumberOfSearch(wxCommandEvent &event)
{
    wxGetApp().m_pAppOptions->SetOption(wxT("SearchCount"), wxString::Format(wxT("%d"), event.GetInt()));
}
