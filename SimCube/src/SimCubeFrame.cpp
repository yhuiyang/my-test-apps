// wx headers
#include <wx/wx.h>
#include <wx/iconbndl.h>
#include <wx/wxsqlite3.h>
// our headers
#include "ConfigPane.h"
#include "DebugPane.h"
#include "HistoryPane.h"
#include "PeerPane.h"
#include "PropertyPane.h"
#include "SimCubeFrame.h"
#include "TrapPane.h"
#include "SimCubeApp.h"
// resource headers
#include "img/SimCube-32.xpm"
#include "img/SimCube-48.xpm"
#include "img/SimCube-64.xpm"

enum
{
    ID_VIEW_PROPERTY_PANE = wxID_HIGHEST + 1,
    ID_VIEW_TRAP_PANE,
    ID_VIEW_HISTORY_PANE,
    ID_VIEW_PEER_PANE,
    ID_VIEW_CONFIG_PANE,
    ID_VIEW_DOWNLOAD_PANE,
    ID_VIEW_LOG_PANE,
    ID_VIEW_RESET_LAYOUT,
    ID_HELP_DOC,
};

BEGIN_EVENT_TABLE(SimCubeFrame, wxFrame)
    EVT_ERASE_BACKGROUND(SimCubeFrame::OnEraseBackground)
    EVT_SIZE(SimCubeFrame::OnSize)
    EVT_CLOSE(SimCubeFrame::OnClose)
END_EVENT_TABLE()

SimCubeFrame::SimCubeFrame()
{
    Init();
}

SimCubeFrame::SimCubeFrame(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos,
    const wxSize &size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool SimCubeFrame::Create(wxWindow *parent, wxWindowID id,
    const wxString &caption, const wxPoint &pos,
    const wxSize &size, long style)
{
    int x, y, w, h;
    wxFrame::Create(parent, id, caption, pos, size, style);
    CreateControls();

    /* update frame size and position */
    RetrieveFrameSizeAndPosition(&x, &y, &w, &h);
    SetSize(x, y, w, h);
    return true;
}

SimCubeFrame::~SimCubeFrame()
{
    _auiManager.UnInit();
}

void SimCubeFrame::Init()
{

}

void SimCubeFrame::CreateControls()
{
    /* tell wxAuiManager to manage this frame */
    _auiManager.SetManagedWindow(this);

    /* setup icon for window title bar, taskbar and task switching bar */
    wxIconBundle icons;
    icons.AddIcon(wxIcon(SimCube_32_xpm));
    icons.AddIcon(wxIcon(SimCube_48_xpm));
    icons.AddIcon(wxIcon(SimCube_64_xpm));
    SetIcons(icons);

    /* menu bar */
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT);
    wxMenu *view_menu = new wxMenu;
    view_menu->AppendCheckItem(ID_VIEW_PROPERTY_PANE, _("Property Pane"),
        _("Show or hide the property pane."));
    view_menu->AppendCheckItem(ID_VIEW_TRAP_PANE, _("Trap Pane"),
        _("Show or hid the trap pane."));
    view_menu->AppendCheckItem(ID_VIEW_HISTORY_PANE, _("History Pane"),
        _("Show or hide the history pane."));
    view_menu->AppendCheckItem(ID_VIEW_PEER_PANE, _("Remote Pane"),
        _("Show or hide the remote peer pane."));
    view_menu->AppendCheckItem(ID_VIEW_CONFIG_PANE, _("Configuration Pane"),
        _("Show or hide the configuration pane."));
    view_menu->AppendCheckItem(ID_VIEW_DOWNLOAD_PANE, _("Download Pane"),
        _("Show or hide the download pane."));
    view_menu->AppendCheckItem(ID_VIEW_LOG_PANE, _("Log Widnow"),
        _("Show or hide the logging window."));
    view_menu->AppendSeparator();
    view_menu->Append(ID_VIEW_RESET_LAYOUT, _("Reset all panes"),
        _("Reset all panes to its default position and size."));
    wxMenu *help_menu = new wxMenu;
    help_menu->Append(ID_HELP_DOC, _("Help file"),
        _("Show help file."));
    help_menu->Append(wxID_ABOUT);

    wxMenuBar *mb = new wxMenuBar;
    mb->Append(file_menu, _("File"));
    mb->Append(view_menu, _("View"));
    mb->Append(help_menu, _("Help"));
    SetMenuBar(mb);

    /* tool bar */

    /* panes */
    DebugPane *dbgWin = new DebugPane(this);
    wxLog *logger = new wxLogTextCtrl(dbgWin->GetLogTextCtrl());
    delete wxLog::SetActiveTarget(logger);
    wxLog::SetTimestamp(wxT("[%Y/%m/%d %H:%M:%S]"));
    _auiManager.AddPane(dbgWin, wxAuiPaneInfo().Name(wxT("DebugPane")).
        Caption(_("Log Output")).Bottom().CloseButton(true).
        DestroyOnClose(false).MaximizeButton(true).MinSize(-1, 150));

    _auiManager.AddPane(new PropertyPane(this), wxAuiPaneInfo().
        Name(wxT("PropertyPane")).Caption(_("Supported Property")).Left().
        CloseButton(true).DestroyOnClose(false).MaximizeButton(true).
        MinSize(300, -1));

    _auiManager.AddPane(new HistoryPane(this), wxAuiPaneInfo().
        Name(wxT("HistoryPane")).Caption(_("Transaction History")).Center().
        CloseButton(true).DestroyOnClose(false).MaximizeButton(true).
        MinSize(500, -1));

    _auiManager.AddPane(new ConfigPane(this), wxAuiPaneInfo().
        Name(wxT("ConfigPane")).Caption(_("SimCube Configuration")).Bottom().
        CloseButton(true).DestroyOnClose(false).MaximizeButton(true).
        MinSize(150, 50).Hide());

    _auiManager.AddPane(new TrapPane(this), wxAuiPaneInfo().
        Name(wxT("TrapPane")).Caption(_("Send Trap Message")).Right().
        CloseButton(true).DestroyOnClose(false).MinSize(100, 100).
        Hide());

    _auiManager.AddPane(new PeerPane(this), wxAuiPaneInfo().
        Name(wxT("PeerPane")).Caption(_("Remote Status")).Bottom().
        CloseButton(true).DestroyOnClose(false).MinSize(200, 100));

    /* status bar */
    SetStatusBar(CreateStatusBar(3));

    /* commit all change */
    _auiManager.Update();
}

void SimCubeFrame::RetrieveFrameSizeAndPosition(int *x, int *y, int *w, int *h)
{
    int _x = -1, _y = -1, _w = -1, _h = -1;
    wxSQLite3Database *db = wxGetApp().GetMainDatabase();
    wxSQLite3ResultSet set;

    wxASSERT_MSG(db, wxT("db is null pointer"));
    wxASSERT_MSG(x, wxT("x is null pointer"));
    wxASSERT_MSG(y, wxT("y is null pointer"));
    wxASSERT_MSG(w, wxT("w is null pointer"));
    wxASSERT_MSG(h, wxT("h is null pointer"));

    if (db->IsOpen())
    {
        set = db->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'FrameX'"));
        if (set.NextRow())
            _x = set.GetInt(0);
        set.Finalize();
        set = db->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'FrameY'"));
        if (set.NextRow())
            _y = set.GetInt(0);
        set.Finalize();
        set = db->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'FrameW'"));
        if (set.NextRow())
            _w = set.GetInt(0);
        set.Finalize();
        set = db->ExecuteQuery(wxT("SELECT ConfigValue FROM CfgTbl WHERE ConfigName = 'FrameH'"));
        if (set.NextRow())
            _h = set.GetInt(0);
        set.Finalize();
    }

    if ((_x == -1) || (_y == -1) || (_w == -1) || (_h == -1))
    {
        wxSize screen = wxGetDisplaySize();
        float hRatio = 0.85f, vRatio = 0.85f;

        if (screen.x <= 1024)
            hRatio = 0.95f;
        if (screen.y <= 768)
            vRatio = 0.90f;

        _w = (int)(screen.x * hRatio);
        _h = (int)(screen.y * vRatio);
        _x = (int)((screen.x - _w) * 0.5);
        _y = (int)((screen.y - _h) * 0.5);
    }

    *x = _x;
    *y = _y;
    *w = _w;
    *h = _h;
}

void SimCubeFrame::OnEraseBackground(wxEraseEvent &event)
{
    event.Skip();
}

void SimCubeFrame::OnSize(wxSizeEvent &event)
{
    event.Skip();
}

void SimCubeFrame::OnClose(wxCloseEvent &WXUNUSED(event))
{
    Destroy();
}

