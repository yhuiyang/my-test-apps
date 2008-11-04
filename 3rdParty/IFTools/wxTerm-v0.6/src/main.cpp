/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     
// Author:      Joachim Buermann
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include <errno.h>
#include <wx/awx-0.4/awx.h>
#include <wx/awx-0.4/led.h>

#include "about.h"
#include "config.h"
#include "main.h"
#if ( GPIB )
# include "modgpib.h"
#endif
#include "modserial.h"
#include "toolbar.h"

#include <wx/config.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/filesys.h>
#include <wx/fs_mem.h>
#include <wx/fs_zip.h>
#include <wx/progdlg.h>
#include <wx/splash.h>

#include <wx/ctb-0.13/getopt.h>

#include "connection_wdr.h"

// program icon
#include "wxterm.xpm"


#ifdef __WIN32__
# include <wx/msw/winundef.h>
# pragma warning(disable : 4800)
#endif

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    // Input
    EVT_CONSOLE_CHAR(MyFrame::OnCommandLine)
    EVT_CONSOLE_LINE(MyFrame::OnCommandLine)
    // Toolbar
    EVT_CLOSE(MyFrame::OnClose)
    EVT_MENU(ID_QUIT, MyFrame::OnQuit)
    EVT_MENU(ID_CONNECT, MyFrame::OnConnect)
    EVT_MENU(ID_CONNECT_ON_OFF, MyFrame::OnConnectOnOff)
    EVT_MENU(ID_DISCONNECT, MyFrame::OnDisconnect)
    EVT_MENU(ID_OPEN, MyFrame::OnOpen)
    EVT_MENU(ID_SAVE, MyFrame::OnRecord)
    EVT_MENU(ID_SAVE_AS, MyFrame::OnSave)
    EVT_MENU(ID_BREAK, MyFrame::OnBreak)
    EVT_MENU(ID_EMPTY, MyFrame::OnEmpty)
    EVT_MENU(ID_SETTINGS, MyFrame::OnSettings)
    EVT_MENU(ID_TOGGLE_DTR, MyFrame::OnToggleDTR)
    EVT_MENU(ID_TOGGLE_RTS, MyFrame::OnToggleRTS)
    EVT_BUTTON(ID_TOGGLE_DTR, MyFrame::OnToggleDTR)
    EVT_BUTTON(ID_TOGGLE_RTS, MyFrame::OnToggleRTS)
    EVT_MENU(ID_VIEWHEX, MyFrame::OnToolbar)
    EVT_MENU(ID_VIEWASCII, MyFrame::OnToolbar)
    EVT_MENU(ID_RELOAD, MyFrame::OnToolbar)
    EVT_MENU(ID_TB_TEXT, MyFrame::OnToolbarText)
    EVT_MENU(ID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(ID_HELP, MyFrame::OnHelp)
    // Refresh Ticker
    EVT_UPDATE_UI(ID_TICKER, MyFrame::OnIdle)
    // Input mode
    EVT_CHOICE(ID_IMODE, MyFrame::OnSetInputMode)
    // Quick Baudrate Switch
    EVT_CHOICE(ID_SWITCHBAUD, MyFrame::OnBaudrateChanged)
    // Tabs
    EVT_NOTEBOOK_PAGE_CHANGING(-1,MyFrame::OnPageChanging)
    EVT_NOTEBOOK_PAGE_CHANGED(-1,MyFrame::OnPageChanged)
END_EVENT_TABLE()
    
IMPLEMENT_APP(MyApp)

// -------------------------------------------------------------------
// defines
// -------------------------------------------------------------------
#define EOS_ENTRIES 5

// ----------------- options -------------------------------
const char* options="df:ghx";

const char* helpMessage =
{
    "wxterm - test terminal for serial connections @jb/2003\n"
    "usage wxterm [options] [configfile]\n"
    "Valid options are:\n"
    "-d      : debug facilities\n"
    "-f file : use file for load/save program settings\n"
    "          default is wxterm.db in the working directory\n"
    "          (for example wxterm -f ~/myconfig.db)\n"
    "-g      : enable GPIB support also if no device exists\n"
    "-h      : print this\n"
    "-x      : omit check for GPIB (disables GPIB support)\n"
    "At starting wxterm automatically by double click in the\n"
    "microsoft explorer, the config file can also be committed\n"
    "as the last parameter without -f\n"
};

// -------------------------------------------------------------------
// globals
// -------------------------------------------------------------------

// program setting file (db file)
wxString g_configfile = "wxterm.db";
// helpfile
wxString g_helpfile = wxEmptyString;
// struct for covering the connection parameter
prefs_t g_prefs;
// debugging
bool g_debug = false;
// look for a valid GPIB interface (default)
bool g_gpibcheck = true;
// force gpib support
bool g_gpibforce = false;

// -------------------------------------------------------------------
// MyApp
// -------------------------------------------------------------------

wxString MyApp::GetAppPath(char* argv0)
{
    wxString str;
    wxString cwd = wxGetCwd();
#if defined (__WXMAC__) && !defined(__DARWIN__)
    return cwd;
#endif
    if (wxIsAbsolutePath(argv0)) {
        return wxPathOnly(argv0);
    }
    else {
        wxString currentDir(cwd);
        if (currentDir.Last() != wxFILE_SEP_PATH)
        {
            currentDir += wxFILE_SEP_PATH;
        }

        str = currentDir + argv0;
        if (wxFileExists(str))
        {
            return wxPathOnly(str);
        }
    }
    return wxEmptyString;
}

bool MyApp::OnInit()
{
    int lang = 0;
    int val;

    SetVendorName(_T("IFTOOLS GbR"));
    SetAppName(_T("wxTerm"));

    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxInitAllImageHandlers();
    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem fsys;

    wxConfig::Get();

    wxMemoryFSHandler::AddFile(wxT("about.jpg"),
						 about,
						 about_size);

    wxFSFile* f = fsys.OpenFile(wxT("memory:about.jpg"));

    while ((val=getopt(argc,argv,(char*)options))!=EOF) {
	   switch (val) {
	   case 'd' : g_debug = true; break;
	   case 'f' : g_configfile = optarg; break;
	   case 'g' : g_gpibforce = true; break;
	   case 'h' : printf(helpMessage); exit(0);
	   case 'x' : g_gpibcheck = false; break;
	   }
    }
    // check system language
    switch(lang) {
    case 1: m_locale.Init(wxLANGUAGE_ENGLISH); break;
    case 2: m_locale.Init(wxLANGUAGE_GERMAN); break;
    default: m_locale.Init(m_locale.GetSystemLanguage());
    }
    wxString catalog = GetAppPath(argv[0]) + wxFILE_SEP_PATH;
    m_locale.AddCatalogLookupPathPrefix(catalog);
    m_locale.AddCatalog(wxT("wxterm"));

    if(m_locale.GetName() == "de_DE") {
	   g_helpfile = GetAppPath(argv[0]) + wxFILE_SEP_PATH + "wxterm_de.zip";
    }
    else {
	   g_helpfile = GetAppPath(argv[0]) + wxFILE_SEP_PATH + "wxterm_en.zip";
    }
    printf("%s\n",g_helpfile.c_str());
    wxString dbfile(argv[optind]);
    if(!dbfile.IsEmpty()) {
	   g_configfile = dbfile;
    }

    wxSplashScreen* splash = new wxSplashScreen(
	   wxImage(*f->GetStream(),wxBITMAP_TYPE_JPEG),
	   wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
	   6000, NULL, -1, wxDefaultPosition, wxDefaultSize,
	   wxSIMPLE_BORDER | wxSTAY_ON_TOP);
    wxYield();
    MyFrame *frame = new MyFrame("wxTerminal",
						   wxDefaultPosition,
						   wxDefaultSize);

    // define the minimal size of the application
    frame->SetSizeHints(600,300);
    frame->Show(true);
    SetTopWindow(frame);

    return true;
};


// -------------------------------------------------------------------
// MyFrame
// -------------------------------------------------------------------

MyFrame::MyFrame(const wxString& title,
			  const wxPoint& pos, 
			  const wxSize& size) :
    wxFrame((wxFrame*)NULL,-1,title,pos,size,
		  wxDEFAULT_FRAME_STYLE)
{
    int i = 0;
    m_module = NULL;
    SetIcon(wxICON(wxterm));

    if(g_debug) {
	   wxLog* log = new wxLogWindow(this,"Debug Log");
	   wxLog::SetActiveTarget(log);
    }

    if(g_gpibcheck) {
	   m_gpib_exist = CheckForGPIB();
    }
    else {
	   m_gpib_exist = false;
    }
    m_logfd = NULL;
    m_logcounter = 0;
    m_rcounter = 0;
    
    if(!ReadConfig(g_prefs)) {
	   // something is going wrong! Perhaps the registry or init file
	   // is corrupt or doesn't exist. So we use the default settings
	   prefs_t p;
	   g_prefs = p;
    }

    SetBackgroundColour(wxSystemSettings::GetColour(
					   wxSYS_COLOUR_BTNFACE));

    wxBoxSizer* top = new wxBoxSizer(wxVERTICAL);

    // Add help file
    m_help = new wxHtmlHelpController;
    m_help->AddBook(g_helpfile);

    // create Menubar and attach this menu bar to the frame
    SetMenuBar(CreateMenuBar());

    // create a toolbar
    m_tb = new awxToolbar(this,-1);
    CreateToolbar(m_tb,g_prefs.m_toolbarText);
    top->Add(m_tb,0,wxEXPAND|wxALL,0);
    // create a statusbar
    m_sb = new MyStatusBar(this);
    SetStatusBar(m_sb);
    SetStatusBarPane(-1);
    GetStatusBar()->Show();
    PositionStatusBar();
    
    m_split = new wxSplitterWindow(this,-1,
							wxDefaultPosition,
							wxDefaultSize,
							wxSP_NOBORDER | wxWANTS_CHARS);
#ifdef __WXMSW__
   m_split->SetMinimumPaneSize(10);
#endif
    // a special widget showing the'output' of the connected device
    // as hex or normal ascii
    m_dump = new wxDView(m_split,-1,
					wxDefaultPosition,
					wxDefaultSize,
					g_prefs.m_c,
					0);
    // the bottom textfield is the input field, all lines,
    // conclude with a newline (RETURN) will send throughout
    // the adjusted comport
    m_input = new Console(m_split,
					 ID_COMMANDLINE,
					 "",
					 wxDefaultPosition,
					 //wxSize(400,120),
					 wxDefaultSize,
					 wxTE_MULTILINE | 
					 wxNO_FULL_REPAINT_ON_RESIZE |
					 wxHSCROLL
	   );
    m_input->SetFont(wxFont(12,wxMODERN,wxNORMAL,wxBOLD));

    top->Add(m_split,1,wxEXPAND|wxALL,0);
    m_split->SplitHorizontally(m_dump,m_input,0);
    
    // on the bottom a notebook widget select the current active device,
    // for example a serial RS232 or GPIB device module
    wxNotebook* tabs = new wxNotebook(this,ID_MODULES,
							   wxDefaultPosition,
							   wxSize(-1,100),
							   wxNB_FIXEDWIDTH);
    memset(m_modules,0,sizeof(m_modules));
    m_modules[0] = new wxModuleRS232(tabs);
    tabs->AddPage(m_modules[0],"Serial",true);
#if ( GPIB )
    if(m_gpib_exist) {
	   m_modules[1] = new wxModuleGPIB(tabs);
	   tabs->AddPage(m_modules[1],"GPIB",false);
    }
#endif
    top->Add(tabs,0,wxEXPAND|wxALL,0);
    // default Page is serial page
    m_module = m_modules[0];
    // tell dialog to use sizer
    SetAutoLayout(true);
    // actually set the sizer   
    SetSizer(top);
    top->Fit(this);

    if(g_prefs.m_dim.width < 600) {
	   g_prefs.m_dim.width = 600;
    }
    SetSize(g_prefs.m_dim);
    if(g_prefs.m_sashPos > 0) {
	   m_split->SetSashPosition(g_prefs.m_sashPos);
    }

    m_modules[0]->SetEOSSelection(g_prefs.m_eos);
    m_modules[0]->SetInputMode(g_prefs.m_inputMode);

    Disconnect();

    m_dump->SetMode(wxDUMP_AS_ASCII);
    m_dump->SetAutoscroll(g_prefs.m_autoScroll);
    // show display settings
    SetStatusText("ASCII",0);    
    // show connection settings
    SetStatusText(g_prefs.GetDeviceSettings(),1);
    SetStatusText(_("Disconnected"),2);

    m_dump->Clear();

    m_ticker = new Ticker(this);
    m_ticker->Start(50);
   
    if(g_prefs.m_autoConnect) {
	   if(!Connect(g_prefs)) {
		  // oops, an error occurs
		  wxString s("");
		  s.Printf("Cannot open %s\n"
				 "Please check the parameter\n"
				 "for the connection!",
				 g_prefs.m_port.c_str());
		  wxMessageDialog *d = new wxMessageDialog(this,s);
		  d->ShowModal();
	   }
	   else {
		  PressButton(m_tb,ID_CONNECT_ON_OFF,true);
		  ShowLinestate(true);
	   }
    }
    Update();
};

MyFrame::~MyFrame()
{
    delete m_tb;
    delete m_ticker;
};

bool MyFrame::CheckForGPIB()
{
#if ( GPIB )
    wxGPIB gpib;
    int result = gpib.Open("gpib1");
    gpib.Close();
    return ( result >= 0 ) || g_gpibforce;
#else
    return 0;
#endif
};

int MyFrame::ClearCachedFiles(wxString dir,wxString pattern)
{
    wxDir tmpdir;
    wxArrayString tmpfiles;
    size_t n = tmpdir.GetAllFiles(dir,&tmpfiles,"wxterm*",wxDIR_FILES);
    if(n > 0) {
	   for(int m=0;m < n;m++) {
		  wxRemoveFile(tmpfiles[m].c_str());
	   }
	   return n;
    }
    return 0;
};

int MyFrame::Connect(struct prefs_t& prefs)
{
    if(!m_module->IsOpen()) {
	   if(m_module == m_modules[0]) {
		  // serial device control struct
		  wxSerialPort_DCS dcs = prefs.m_s;
		  if(m_module->OpenDevice((char*)prefs.m_port.c_str(),&dcs) < 0) {
			 return 0;
		  }
	   }
#if ( GPIB )
	   else {
		  wxGPIB_DCS dcs = prefs.m_g;
		  // gpib device control struct
		  int result = m_module->OpenDevice(wxGPIB1,&dcs);
		  if(result < 0) {
			 wxLogDebug("Open GPIB board 1 = %i",result);
			 return 0;
		  }
	   }
#endif
	   ShowLinestate(true);
	   SetStatusText(_("Connected"),2);
	   m_module->Refresh(true);
    }
    return 1;
};

int MyFrame::Disconnect()
{
    if(m_module->IsOpen()) {
	   m_module->CloseDevice();
	   SetStatusText(_("Disconnected"),2);
	   m_sb->SetBlink(false);
    }
    return 1;
};

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxFileSystem fsys;
    wxFSFile* f = fsys.OpenFile(wxT("memory:about.jpg"));
    wxSplashScreen* splash = new wxSplashScreen(
	   wxImage(*f->GetStream(),wxBITMAP_TYPE_JPEG),
	   wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
	   6000, NULL, -1, wxDefaultPosition, wxDefaultSize,
	   wxSIMPLE_BORDER | wxSTAY_ON_TOP);
    wxYield();
};

void MyFrame::OnBaudrateChanged(wxCommandEvent& event)
{
    if(m_module->IsOpen() && (m_module == m_modules[0])) {
	   wxBaud baudrate = (wxBaud)strtol(event.GetString().c_str(),NULL,10);
	   ((wxModuleRS232*)m_module)->SetBaudrate(baudrate);
	   g_prefs.m_s.baud = baudrate;
	   SetStatusText(g_prefs.GetDeviceSettings(),1);
    }
};

void MyFrame::OnClose(wxCloseEvent& event)
{
    m_ticker->Stop();
    wxRect dim(GetPosition(),GetSize());
    g_prefs.m_dim = dim;
    g_prefs.m_autoConnect = m_module->IsOpen();
    g_prefs.m_eos = m_modules[0]->GetEOSSelection();
    g_prefs.m_inputMode = m_modules[0]->GetInputMode();
    g_prefs.m_sashPos = m_split->GetSashPosition();
    WriteConfig(g_prefs);
    event.Skip();
};

void MyFrame::OnCommandLine(wxConsoleEvent& event)
{
    if(!m_module->IsOpen()) return;
    if(event.GetEventType() == wxEVT_CONSOLE_CHAR) {
	   char ch = event.GetChar();
	   m_module->WriteDevice(&ch,1);
    }
    else {
	   wxString line = event.GetLine();
	   wxString eos = m_module->GetEOSString();
	   // if hexinput enabled, parse line for '$xx'
	   // sequencies
	   if(m_module->GetInputMode() == 1) {
		  char c;
		  int quota = 0;
		  unsigned char hex = 0;
		  wxString out;
		  size_t len = line.Length();
		  for(int i=0;i<len;i++) {
			 c = line.GetChar(i);
			 if(quota == 1) {
				if(c == '$') {
				    // $$ means: send a single '$'
				    out.Append(c);
				    quota = 0;
				    continue;
				}
				if(isxdigit(c)) {
				    if(isdigit(c)) hex = (c - '0');
				    else hex = ((c & 0x4F) - '7');
				    quota = 2;
				}
				else {
				    quota = 0;
				}
				continue;
			 }
			 else if(quota == 2) {
				if(isxdigit(c)) {
				    hex <<= 4;
				    if(isdigit(c)) hex |= (c - '0');
				    else hex |= ((c & 0x4F) - '7');
				}
				out.Append(hex);
				quota = 0;
				continue;
			 }
			 if(c == '$') quota = 1;
			 else out.Append(c);
		  }
		  out += eos;
		  m_module->WriteDevice((char*)out.c_str(),out.length());
	   }
	   else {
		  line += eos;
		  m_module->WriteDevice((char*)line.c_str(),line.length());
	   }
    }
};

void MyFrame::OnConnect(wxCommandEvent& event)
{
    if(!m_module->IsOpen()) {
	   if(Connect(g_prefs)) {
		  // no error, com is open
		  PressButton(m_tb,ID_CONNECT_ON_OFF,true);
		  ShowLinestate(m_module->IsOpen());
	   }
	   else {
		  PressButton(m_tb,ID_CONNECT_ON_OFF,false);
	   }
    }
};

void MyFrame::OnConnectOnOff(wxCommandEvent& event)
{
    int ok = 0;
    if(m_module->IsOpen()) {
	   ok = Disconnect();
    }
    else {
	   ok = Connect(g_prefs);
	   if(!ok) {
		  PressButton(m_tb,ID_CONNECT_ON_OFF,false);
	   }
    }
    if(ok) {
	   ShowLinestate(m_module->IsOpen());
    }
};

void MyFrame::OnDisconnect(wxCommandEvent& event)
{
    if(m_module->IsOpen()) {
	   if(Disconnect()) {
		  // no error, com port was closed
		  PressButton(m_tb,ID_CONNECT_ON_OFF,false);
		  ShowLinestate(m_module->IsOpen());
	   }
    }
};

void MyFrame::OnHelp(wxCommandEvent& event)
{
    m_help->DisplayContents();
};

void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
    static wxString lastfile("");
    lastfile = wxFileSelector("Choose a file to send",
						"", // default Path
						lastfile, // default file
						"", // default extension
						"*.*"	// wildcards
	   );
    if(!lastfile.empty()) {
	   // work with the file
	   m_ticker->Stop();
	   SendFile(lastfile);
	   m_ticker->Start();
    }
};

void MyFrame::OnPageChanged(wxNotebookEvent& event)
{
    wxNotebook* tab = (wxNotebook*)FindWindow(ID_MODULES);
    if(tab) {
	   g_prefs.m_device = tab->GetSelection();
	   m_module = m_modules[tab->GetSelection()];
	   SetStatusText(g_prefs.GetDeviceSettings(),1);
	   // gpib doesn't have a raw mode
	   m_input->SetRawMode(m_module->GetInputMode() == 2);
    }
};

void MyFrame::OnPageChanging(wxNotebookEvent& event)
{
    if(m_module && m_module->IsOpen()) {
	   event.Veto();
    }
};

void MyFrame::OnRecord(wxCommandEvent& event)
{
    if(event.GetInt()) {
	   // start logging (log button is pressed)
	   if(StartLogging()) {
		  // release log button
		  PressButton(m_tb,ID_SAVE,false);
	   }
	   else {
		  m_sb->SetBlink(true);
	   }
    }
    else {
	   // stop logging (log button is released)
	   StopLogging();
	   m_sb->SetBlink(false);
    }
};

void MyFrame::OnToolbar(wxCommandEvent& event)
{
    switch(event.GetId()) {
    case ID_VIEWASCII:
	   g_prefs.m_viewMode = VIEW_AS_ASCII;
	   break;
    case ID_VIEWHEX:
	   g_prefs.m_viewMode = VIEW_AS_HEX;
	   break;
    case ID_RELOAD:
	   g_prefs.m_autoScroll = event.GetInt();
	   m_dump->SetAutoscroll(g_prefs.m_autoScroll);
	   break;
    default:
	   return;
    }
    Update();
};

void MyFrame::OnToolbarText(wxCommandEvent& event)
{
    delete m_tb;
    m_tb = new awxToolbar(this,-1);
    g_prefs.m_toolbarText = (bool)event.GetInt();
    CreateToolbar(m_tb,event.GetInt());
    GetSizer()->Prepend(m_tb,0,wxEXPAND|wxALL,0);
    Layout();
    m_tb->Redraw();
    Update();
};

int MyFrame::StartLogging()
{
    static wxString lastfile("log.dat");
    lastfile = wxFileSelector("Log to file",
						"", // default path is cwd
						lastfile,	// default file name
						"dat", // default extension
						"*.*", // wild cards
						wxSAVE
	   );
    if(lastfile.IsEmpty()) {
	   return 1;
    }
    if((m_logfd = fopen(lastfile.c_str(),"w+b")) == NULL) {
	   return 2;
    }
    m_logcounter = 0;
    SetStatusText(wxFileName(lastfile).GetFullName().c_str(),3);
    SetStatusText("0",4);
    return 0;
};

int MyFrame::StopLogging()
{
    int err = 1;
    if(m_logfd) {
	   err = fclose(m_logfd);
	   m_logfd = NULL;
 	   SetStatusText("",3);
 	   SetStatusText("",4);
    }
    return err;
};

int MyFrame::SendFile(wxString filename)
{
    char buf[256];
    wxFile file;
    wxString msg;
    if(file.Open(filename.c_str()) == false) {
	   return -1;
    }
    int filesize = file.Length();
    msg.Printf("send %i from %i Bytes",0,filesize);
    wxProgressDialog d("Send file",
				   msg,
				   filesize,
				   NULL,
				   wxPD_AUTO_HIDE | wxPD_CAN_ABORT);
    d.Show();
    if(m_module->IsOpen()) {
	   off_t rd;
	   off_t wr;
	   off_t total = 0;
	   while((rd = file.Read(buf,sizeof(buf))) > 0) {
		  char* cp = buf;

		  while(rd > 0) {
			 wr = m_module->WriteDevice(cp,rd);
			 if(wr < 0) {
				// EAGAIN error
				sleepms(1);
			 }
			 cp += wr;
			 rd -= wr;
			 total += wr;
		  }
		  msg.Printf("Sended Bytes: %i from %i",filesize,total);
		  if(d.Update(total-1,msg) == false) break;
	   }
    }
    return 0;
};

void MyFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
    // save temporary cache file
    wxString saveas = wxFileSelector("Save to file",
							  "", // default path is cwd
							  "log.dat",	// default file name
							  "dat", // default extension
							  "*.*", // wild cards
							  wxSAVE
	   );
    SaveFile(saveas);
};

int MyFrame::SaveFile(wxString saveas)
{
    char buf[1024];
    int ch = 0;
    size_t n = 0;
    size_t len = m_dump->GetCount();
    wxFile file;
    wxString msg;
    if(saveas.empty()) return -1;

    if(!file.Open(saveas,wxFile::write)) return -2;
    

    msg.Printf("save %i from %i Bytes",0,len);
    wxProgressDialog d("Save output data",msg,len,NULL,
 				   wxPD_AUTO_HIDE | wxPD_CAN_ABORT);
    d.Show();

    // pos is used as read pointer, so initiate it with 0
    size_t pos = 0;
    while(n = m_dump->Read(buf,sizeof(buf),pos)) {
 	   if(!file.Write(buf,n)) return -3;
 	   msg.Printf("Saved Bytes: %i from %i",n,len);
 	   d.Update(n,msg);
    }
    return 0;
};

void MyFrame::OnSettings(wxCommandEvent& WXUNUSED(event))
{
    if(SetupPreferences(this,
				    g_prefs,
				    m_module->IsOpen() == false,
				    m_gpib_exist)) {
	   SetStatusText(g_prefs.GetDeviceSettings(),1);
	   m_dump->Redraw();
    }
};

void MyFrame::OnToggleDTR(wxCommandEvent& event)
{
    static int dtr = 0;
    if(m_module->IsOpen() && (m_module == m_modules[0])) {
	   dtr ^= 1;
	   ((wxModuleRS232*)m_module)->SetDTR(dtr);
    }
};

void MyFrame::OnToggleRTS(wxCommandEvent& event)
{
    static int rts = 0;
    if(m_module->IsOpen() && (m_module == m_modules[0])) {
	   rts ^= 1;
	   ((wxModuleRS232*)m_module)->SetRTS(rts);
    }
};

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
};

// this methode will called every 100ms
void MyFrame::OnIdle(wxUpdateUIEvent& WXUNUSED(event))
{
    static int focused = 0;
    static int ledint = 0;
    wxString s;
    if(m_module->IsOpen()) {
	   char b[512];
	   memset(b,0,sizeof(b));
	   // read maximum 80 Bytes, Read never blocks, so we can
	   // do it whenever we wanted (you can decrease the timer
	   // interval or growup the buffer size, if you have many
	   // datas and a high speed
	   int n = m_module->ReadDevice(b,sizeof(b));
	   if(n > 0) {
		  m_rcounter += n;
		  s.Printf("%li",m_rcounter);
		  SetStatusText(s,4);
		  // whenever there were received some data, we put it
		  // in the output window
		  wxString line(b);
		  m_dump->AppendData(b,n);
		  // if record active, save received data into a file
		  if(m_logfd) {
			 fwrite(b,1,n,m_logfd);
			 m_logcounter += n;
			 s.Printf("%li",m_logcounter);
			 SetStatusText(s,4);
		  }
	   }
	   ShowLinestate();
    }
    if(!focused) {
	   focused = 1;
	   m_input->SetFocusFromKbd();
    }
};

void MyFrame::Update()
{
    if(g_prefs.m_viewMode == VIEW_AS_ASCII) {
	   m_dump->SetMode(wxDUMP_AS_ASCII);
	   SetStatusText("ASCII",0);
	   GetMenuBar()->FindItem(ID_VIEWASCII)->Check(true);
    }
    else {
	   m_dump->SetMode(wxDUMP_AS_HEX);
	   SetStatusText("HEX",0);
	   GetMenuBar()->FindItem(ID_VIEWHEX)->Check(true);
    }
    // connect button
    PressButton(m_tb,ID_CONNECT_ON_OFF,m_module->IsOpen());
    // ascii and hex buttons
    PressButton(m_tb,ID_VIEWASCII,g_prefs.m_viewMode == VIEW_AS_ASCII);
    PressButton(m_tb,ID_VIEWHEX,g_prefs.m_viewMode == VIEW_AS_HEX);
    // autoscroll (fresh) button
    PressButton(m_tb,ID_RELOAD,g_prefs.m_autoScroll);
    GetMenuBar()->FindItem(ID_RELOAD)->Check(g_prefs.m_autoScroll);
    // output window
    m_dump->Redraw();
    // Input mode
    m_input->SetRawMode(m_module->GetInputMode() == 2);
};
