/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _LOG_PANE_H_
#define _LOG_PANE_H_

#include <wx/wx.h>
#include <wx/log.h>

class LogPane : public wxPanel
{
public:
    // ctor
    LogPane();
    LogPane(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    // dtor
    ~LogPane();

    bool Create(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    bool IsVerbose();
    void SetVerbose(bool verbose = true);

private:
    // helper functions
    void Init();
    void CreateControls();

    // event handlers
    void OnSave(wxCommandEvent &event);
    void OnErase(wxCommandEvent &event);

    // private data members
    wxTextCtrl *_logTextCtrl;
    wxLog *_oldLogTarget;
};

#endif /* _LOG_PANE_H_ */

