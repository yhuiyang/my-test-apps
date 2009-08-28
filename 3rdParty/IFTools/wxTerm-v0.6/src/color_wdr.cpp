//------------------------------------------------------------------------------
// Source code generated by wxDesigner from file: color.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "color_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "color_wdr.h"


// Implement window functions

wxSizer *SetupColor( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer *item1 = new wxFlexGridSizer( 4, 4, 8 );

    wxStaticText *item2 = new wxStaticText( parent, ID_TEXT, wxT("Rule"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    item2->SetFont( wxFont( 13, wxSWISS, wxNORMAL, wxBOLD ) );
#if defined(__WXMSW__) && !(wxCHECK_VERSION(2,3,0))
    item2->SetSize( item2->GetBestSize() );
#endif
    item1->Add( item2, 0, wxALIGN_CENTER, 0 );

    wxStaticText *item3 = new wxStaticText( parent, ID_TEXT, wxT("From first"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->SetFont( wxFont( 13, wxSWISS, wxNORMAL, wxBOLD ) );
#if defined(__WXMSW__) && !(wxCHECK_VERSION(2,3,0))
    item3->SetSize( item3->GetBestSize() );
#endif
    item1->Add( item3, 0, wxALIGN_CENTER, 5 );

    wxStaticText *item4 = new wxStaticText( parent, ID_TEXT, wxT("To last"), wxDefaultPosition, wxDefaultSize, 0 );
    item4->SetFont( wxFont( 13, wxSWISS, wxNORMAL, wxBOLD ) );
#if defined(__WXMSW__) && !(wxCHECK_VERSION(2,3,0))
    item4->SetSize( item4->GetBestSize() );
#endif
    item1->Add( item4, 0, wxALIGN_CENTER, 5 );

    wxStaticText *item5 = new wxStaticText( parent, ID_TEXT, wxT("with color"), wxDefaultPosition, wxDefaultSize, 0 );
    item5->SetFont( wxFont( 13, wxSWISS, wxNORMAL, wxBOLD ) );
#if defined(__WXMSW__) && !(wxCHECK_VERSION(2,3,0))
    item5->SetSize( item5->GetBestSize() );
#endif
    item1->Add( item5, 0, wxALIGN_CENTER, 5 );

    wxCheckBox *item6 = new wxCheckBox( parent, ID_USE1, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 0 );

    wxSpinCtrl *item7 = new wxSpinCtrl( parent, ID_FIRST1, wxT("0"), wxDefaultPosition, wxSize(50,-1), 0, 0, 255, 0 );
    item1->Add( item7, 0, wxALIGN_CENTER, 5 );

    wxSpinCtrl *item8 = new wxSpinCtrl( parent, ID_LAST1, wxT("0"), wxDefaultPosition, wxSize(50,-1), 0, 0, 255, 0 );
    item1->Add( item8, 0, wxALIGN_CENTER, 5 );

    wxString strs9[] = 
    {
        wxT("Red"), 
        wxT("Orange"), 
        wxT("Yellow"), 
        wxT("Green"), 
        wxT("Blue"), 
        wxT("Violet"), 
        wxT("Grey"), 
        wxT("Black")
    };
    wxChoice *item9 = new wxChoice( parent, ID_COLOR1, wxDefaultPosition, wxSize(100,-1), 8, strs9, 0 );
    item1->Add( item9, 0, wxALIGN_CENTER, 5 );

    wxCheckBox *item10 = new wxCheckBox( parent, ID_USE2, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item10, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5 );

    wxSpinCtrl *item11 = new wxSpinCtrl( parent, ID_FIRST2, wxT("0"), wxDefaultPosition, wxSize(50,-1), 0, 0, 255, 0 );
    item1->Add( item11, 0, wxALIGN_CENTER, 5 );

    wxSpinCtrl *item12 = new wxSpinCtrl( parent, ID_LAST2, wxT("0"), wxDefaultPosition, wxSize(50,-1), 0, 0, 255, 0 );
    item1->Add( item12, 0, wxALIGN_CENTER, 5 );

    wxString strs13[] = 
    {
        wxT("Red"), 
        wxT("Orange"), 
        wxT("Yellow"), 
        wxT("Green"), 
        wxT("Blue"), 
        wxT("Violet"), 
        wxT("Grey"), 
        wxT("Black")
    };
    wxChoice *item13 = new wxChoice( parent, ID_COLOR2, wxDefaultPosition, wxSize(100,-1), 8, strs13, 0 );
    item1->Add( item13, 0, wxALIGN_CENTER, 5 );

    wxCheckBox *item14 = new wxCheckBox( parent, ID_USE3, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item14, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5 );

    wxSpinCtrl *item15 = new wxSpinCtrl( parent, ID_FIRST3, wxT("0"), wxDefaultPosition, wxSize(50,-1), 0, 0, 255, 0 );
    item1->Add( item15, 0, wxALIGN_CENTER, 5 );

    wxSpinCtrl *item16 = new wxSpinCtrl( parent, ID_LAST3, wxT("0"), wxDefaultPosition, wxSize(50,-1), 0, 0, 255, 0 );
    item1->Add( item16, 0, wxALIGN_CENTER, 5 );

    wxString strs17[] = 
    {
        wxT("Red"), 
        wxT("Orange"), 
        wxT("Yellow"), 
        wxT("Green"), 
        wxT("Blue"), 
        wxT("Violet"), 
        wxT("Grey"), 
        wxT("Black")
    };
    wxChoice *item17 = new wxChoice( parent, ID_COLOR3, wxDefaultPosition, wxSize(100,-1), 8, strs17, 0 );
    item1->Add( item17, 0, wxALIGN_CENTER, 5 );

    wxCheckBox *item18 = new wxCheckBox( parent, ID_USE4, wxT("4"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item18, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5 );

    wxSpinCtrl *item19 = new wxSpinCtrl( parent, ID_FIRST4, wxT("0"), wxDefaultPosition, wxSize(50,-1), 0, 0, 255, 0 );
    item1->Add( item19, 0, wxALIGN_CENTER, 5 );

    wxSpinCtrl *item20 = new wxSpinCtrl( parent, ID_LAST4, wxT("0"), wxDefaultPosition, wxSize(50,-1), 0, 0, 255, 0 );
    item1->Add( item20, 0, wxALIGN_CENTER, 5 );

    wxString strs21[] = 
    {
        wxT("Red"), 
        wxT("Orange"), 
        wxT("Yellow"), 
        wxT("Green"), 
        wxT("Blue"), 
        wxT("Violet"), 
        wxT("Grey"), 
        wxT("Black")
    };
    wxChoice *item21 = new wxChoice( parent, ID_COLOR4, wxDefaultPosition, wxSize(100,-1), 8, strs21, 0 );
    item1->Add( item21, 0, wxALIGN_CENTER, 5 );

    wxCheckBox *item22 = new wxCheckBox( parent, ID_USE5, wxT("Rest"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item22, 0, wxALIGN_CENTER, 5 );

    item1->Add( 20, 20, 0, wxALIGN_CENTER, 5 );

    item1->Add( 20, 20, 0, wxALIGN_CENTER, 5 );

    wxString strs23[] = 
    {
        wxT("Red"), 
        wxT("Orange"), 
        wxT("Yellow"), 
        wxT("Green"), 
        wxT("Blue"), 
        wxT("Violet"), 
        wxT("Grey"), 
        wxT("Black")
    };
    wxChoice *item23 = new wxChoice( parent, ID_COLOR5, wxDefaultPosition, wxSize(100,-1), 8, strs23, 0 );
    item1->Add( item23, 0, wxALIGN_CENTER, 5 );

    item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

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