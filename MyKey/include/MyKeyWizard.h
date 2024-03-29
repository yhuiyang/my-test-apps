/////////////////////////////////////////////////////////////////////////////
// Name:        MyKeyWizard.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     24/05/2010 00:37:08
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYKEYWIZARD_H_
#define _MYKEYWIZARD_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/wizard.h"
#include "wx/gbsizer.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class WizardPageWelcome;
class WizardPagePasswd;
class WizardPageKeyInfo;
class WizardPageDone;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_MYKEYWIZARD 10000
#define ID_WIZARDPAGE_WELCOME 10001
#define ID_WIZARDPAGE_PASSWD 10002
#define ID_TEXTCTRL_LV1PW1 10003
#define ID_TEXTCTRL_LV1PW2 10004
#define ID_TEXTCTRL_LV2PW1 10005
#define ID_TEXTCTRL_LV2PW2 10006
#define ID_BUTTON_DEMO 10007
#define ID_WIZARDPAGE_KEYINFO 10008
#define ID_TEXTCTRL_HWID 10009
#define ID_TEXTCTRL_SWID 10010
#define ID_TEXTCTRL_USER 10011
#define ID_TEXTCTRL_CONTACT 10012
#define ID_RADIOBOX_CODEGEN 10013
#define ID_WIZARDPAGE_DONE 10014
#define SYMBOL_MYKEYWIZARD_IDNAME ID_MYKEYWIZARD
////@end control identifiers


/*!
 * MyKeyWizard class declaration
 */

class MyKeyWizard: public wxWizard
{    
    DECLARE_DYNAMIC_CLASS( MyKeyWizard )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MyKeyWizard();
    MyKeyWizard( wxWindow* parent, wxWindowID id = SYMBOL_MYKEYWIZARD_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MYKEYWIZARD_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Destructor
    ~MyKeyWizard();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin MyKeyWizard event handler declarations

////@end MyKeyWizard event handler declarations

////@begin MyKeyWizard member function declarations

    /// Runs the wizard
    bool Run();

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end MyKeyWizard member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin MyKeyWizard member variables
////@end MyKeyWizard member variables
};

/*!
 * WizardPageWelcome class declaration
 */

class WizardPageWelcome: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPageWelcome )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPageWelcome();

    WizardPageWelcome( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPageWelcome();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPageWelcome event handler declarations

////@end WizardPageWelcome event handler declarations

////@begin WizardPageWelcome member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPageWelcome member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPageWelcome member variables
////@end WizardPageWelcome member variables
};

/*!
 * WizardPagePasswd class declaration
 */

class WizardPagePasswd: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPagePasswd )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPagePasswd();

    WizardPagePasswd( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPagePasswd();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPagePasswd event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGING event handler for ID_WIZARDPAGE_PASSWD
    void OnPasswdPageChanging( wxWizardEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_DEMO
    void OnButtonDemoClick( wxCommandEvent& event );

////@end WizardPagePasswd event handler declarations

////@begin WizardPagePasswd member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPagePasswd member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPagePasswd member variables
////@end WizardPagePasswd member variables
};

/*!
 * WizardPageKeyInfo class declaration
 */

class WizardPageKeyInfo: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPageKeyInfo )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPageKeyInfo();

    WizardPageKeyInfo( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPageKeyInfo();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPageKeyInfo event handler declarations

    /// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_WIZARDPAGE_KEYINFO
    void OnKeyInfoPageChanged( wxWizardEvent& event );

    /// wxEVT_WIZARD_PAGE_CHANGING event handler for ID_WIZARDPAGE_KEYINFO
    void OnKeyInfoPageChanging( wxWizardEvent& event );

////@end WizardPageKeyInfo event handler declarations

////@begin WizardPageKeyInfo member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPageKeyInfo member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPageKeyInfo member variables
////@end WizardPageKeyInfo member variables
};

/*!
 * WizardPageDone class declaration
 */

class WizardPageDone: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( WizardPageDone )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardPageDone();

    WizardPageDone( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~WizardPageDone();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardPageDone event handler declarations

////@end WizardPageDone event handler declarations

////@begin WizardPageDone member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardPageDone member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardPageDone member variables
////@end WizardPageDone member variables
};

#endif
    // _MYKEYWIZARD_H_
