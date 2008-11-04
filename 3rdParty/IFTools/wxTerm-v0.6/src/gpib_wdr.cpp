//------------------------------------------------------------------------------
// Source code generated by wxDesigner from file: gpib.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "gpib_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "gpib_wdr.h"

#include <wx/intl.h>

// Implement window functions

wxSizer *SetupGPIB( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxGridSizer *item1 = new wxGridSizer( 2, 0, 0 );

    wxStaticText *item2 = new wxStaticText( parent, ID_TEXT, _("Address"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxSpinCtrl *item3 = new wxSpinCtrl( parent, ID_ADDRESS, wxT("1"), wxDefaultPosition, wxSize(100,-1), 0, 1, 31, 1 );
    item1->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item4 = new wxStaticText( parent, ID_TEXT, _("Timeout"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxString strs5[] = 
    {
        _("None"), 
        _("10us"), 
        _("30us"), 
        _("100us"), 
        _("300us"), 
        _("1ms"), 
        _("3ms"), 
        _("10ms"), 
        _("30ms"), 
        _("100ms"), 
        _("300ms"), 
        _("1s"), 
        _("3s"), 
        _("10s"), 
        _("30s"), 
        _("100s"), 
        _("300s"), 
        _("1000s")
    };
    wxChoice *item5 = new wxChoice( parent, ID_TIMEOUT, wxDefaultPosition, wxSize(100,-1), 18, strs5, 0 );
    item1->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item1, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5 );

    wxStaticBox *item7 = new wxStaticBox( parent, -1, _("EOS Settings") );
    wxStaticBoxSizer *item6 = new wxStaticBoxSizer( item7, wxVERTICAL );

    wxFlexGridSizer *item8 = new wxFlexGridSizer( 2, 0, 8 );

    wxStaticText *item9 = new wxStaticText( parent, ID_TEXT, _("Send EOI at end of Write"), wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxCheckBox *item10 = new wxCheckBox( parent, ID_IBEOT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item10, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    wxStaticText *item11 = new wxStaticText( parent, ID_TEXT, _("Terminate Read on EOS"), wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxCheckBox *item12 = new wxCheckBox( parent, ID_IBEOSA, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item12, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    wxStaticText *item13 = new wxStaticText( parent, ID_TEXT, _("Set EOI with EOS on Write"), wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxCheckBox *item14 = new wxCheckBox( parent, ID_IBEOSB, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item14, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0 );

    wxStaticText *item15 = new wxStaticText( parent, ID_TEXT, _("8-bit EOS Compare"), wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxCheckBox *item16 = new wxCheckBox( parent, ID_IBEOSC, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item16, 0, wxGROW|wxALIGN_CENTER_HORIZONTAL|wxALL, 0 );

    wxStaticText *item17 = new wxStaticText( parent, ID_TEXT, _("EOS Byte decimal)"), wxDefaultPosition, wxDefaultSize, 0 );
    item8->Add( item17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item18 = new wxTextCtrl( parent, ID_IBEOS, wxT(""), wxDefaultPosition, wxSize(40,-1), 0 );
    item8->Add( item18, 0, wxALIGN_CENTER|wxALL, 0 );

    item6->Add( item8, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item0->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetAutoLayout( TRUE );
        parent->SetSizer( item0 );
        if (call_fit)
        {
            item0->Fit( parent );
            item0->SetSizeHints( parent );
        }
    }
    
    return item0;
}

// Implement menubar functions

// Implement toolbar functions

// Implement bitmap functions


// End of generated file
