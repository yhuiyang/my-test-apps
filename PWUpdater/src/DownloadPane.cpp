// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/thread.h>
#include "PWUpdater.h"
#include "DownloadPane.h"
#include "WidgetsId.h"
#include "TftpdThread.h"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
BEGIN_EVENT_TABLE(DownloadPane, wxPanel)
    EVT_BUTTON(myID_BTN_START_TFTP, DownloadPane::OnButtonStartTftp)
    EVT_BUTTON(myID_BTN_STOP_TFTP, DownloadPane::OnButtonStopTftp)
    EVT_THREAD(myID_THREAD_TFTPD, DownloadPane::OnThreadTftpd)
END_EVENT_TABLE()

DownloadPane::DownloadPane()
{
    Init();
}

DownloadPane::DownloadPane(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    Init();
    Create(parent, id, pos, size, style);
}

DownloadPane::~DownloadPane()
{
}

void DownloadPane::Init()
{
}

bool DownloadPane::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint &pos, const wxSize &size,
                         long style)
{
    wxPanel::Create(parent, id, pos, size, style);
    CreateControls();
    Center();
    return true;
}

void DownloadPane::CreateControls()
{
    wxBoxSizer *paneSizer = new wxBoxSizer(wxVERTICAL);

    paneSizer->Add(new wxButton(this, myID_BTN_START_TFTP, wxT("Start Server")), 0, wxALL, 5);
    paneSizer->Add(new wxButton(this, myID_BTN_STOP_TFTP, wxT("Stop Server")), 0, wxALL, 5);

    SetSizerAndFit(paneSizer);
}

void DownloadPane::DoStartTftpServerThread()
{
    wxIPV4address local;
    wxString root;
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpdServerThread *&pServer = wxGetApp().m_pTftpdServerThread;
    
    /* assign preferred interface */
    local.AnyAddress();

    /* assign root path. Use wxEmptyString for current working path. */
    root = wxEmptyString;

    cs.Enter();

    /* check if another tftp server thread is running... */
    if (pServer)
    {
        cs.Leave();
        wxLogWarning(wxT("There is another tftp server thread running!"));
        return;
    }

    pServer = new TftpdServerThread(this, myID_THREAD_TFTPD, local, root);

    if (pServer->Create() != wxTHREAD_NO_ERROR)
    {
        wxLogError(wxT("Can't create tftp server thread!"));
        wxDELETE(pServer);
    }
    else
    {
        if (pServer->Run() != wxTHREAD_NO_ERROR)
        {
            wxLogError(wxT("Can't run the tftp server thread!"));
            wxDELETE(pServer);
        }
    }

    cs.Leave();
}

void DownloadPane::DoStopTftpServerThread()
{
    wxCriticalSection &cs = wxGetApp().m_serverCS;
    TftpdServerThread *&pServer = wxGetApp().m_pTftpdServerThread;

    cs.Enter();

    /* check if tftp server thread is already running... */
    if (!pServer)
    {
        cs.Leave();
        wxLogWarning(wxT("There is no tftp server thread running."));
        return;
    }

    if (pServer->Delete() != wxTHREAD_NO_ERROR)
        wxLogError(wxT("Can't delete tftp server thread!"));

    cs.Leave();
}

//
// event handlers
//
void DownloadPane::OnButtonStartTftp(wxCommandEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Start tftp server thread..."));
    DoStartTftpServerThread();
}

void DownloadPane::OnButtonStopTftp(wxCommandEvent &WXUNUSED(event))
{
    wxLogMessage(wxT("Stop tftp server thread..."));
    DoStopTftpServerThread();
}

void DownloadPane::OnThreadTftpd(wxThreadEvent &event)
{
    TftpdMessage msg = event.GetPayload<TftpdMessage>();
    int transferMode, errorCode, currentBlock, totalBlock;
    wxString fileName, errorMessage;

    switch (msg.GetEvent())
    {
    case TFTPD_EVENT_READ_REQUEST:
        transferMode = msg.GetNum1();
        fileName = msg.GetString();
        wxLogMessage(wxT("Read request: file %s, mode = %d"),
            fileName, transferMode);
        break;
    case TFTPD_EVENT_READ_TRANSFER_UPDATE:
    case TFTPD_EVENT_READ_TRANSFER_DONE:
        currentBlock = msg.GetNum1();
        totalBlock = msg.GetNum2();
        fileName = msg.GetString();
        wxLogMessage(wxT("Read update: file %s, current = %d, total = %d"),
            fileName, currentBlock, totalBlock);
        break;
    case TFTPD_EVENT_WRITE_REQUEST:
        transferMode = msg.GetNum1();
        fileName = msg.GetString();
        wxLogMessage(wxT("Write request: file %s, mode = %d"),
            fileName, transferMode);
        break;
    case TFTPD_EVENT_WRITE_TRANSFER_UPDATE:
    case TFTPD_EVENT_WRITE_TRANSFER_DONE:
        currentBlock = msg.GetNum1();
        totalBlock = msg.GetNum2();
        fileName = msg.GetString();
        wxLogMessage(wxT("Write update: file %s, current = %d, total = %d"),
            fileName, currentBlock, totalBlock);
        break;
    case TFTPD_EVENT_ERROR:
        errorCode = msg.GetNum1();
        errorMessage = msg.GetString();
        wxLogMessage(wxT("Tftp error: error code = %d, error message = %s"),
            errorCode, errorMessage);
        break;
    default:
        wxLogError(wxT("Unknown TFTPD messge received! (%d"), msg.GetEvent());
        return;
    }
}
