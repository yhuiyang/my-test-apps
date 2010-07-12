/*
 *  AppOptions.cpp - Configuration back-end db (Use wxSQLite3/SQLite3).
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "AppOptions.h"

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
AppOptions::AppOptions()
{
    wxStandardPathsBase &stdPaths = wxStandardPaths::Get();
    wxString dbDir = wxFileName(stdPaths.GetExecutablePath()).GetPathWithSep();
    wxString dbName = wxApp::GetInstance()->GetAppName();

    if (dbName.empty())
        dbName = wxT("appopt");

    dbName << wxString::Format(wxT("-%s"), wxGetUserId());

    if (NULL != (_db = new wxSQLite3Database))
    {
        _db->Open(dbDir + dbName + wxT(".db"));
        if (_db->IsOpen())
            DB_Init();
    }
}

AppOptions::~AppOptions()
{
    if (_db)
    {
        if (_db->IsOpen())
            _db->Close();
        delete _db;
    }
}

void AppOptions::DB_Init()
{
    wxString sql;

    sql << wxT("BEGIN TRANSACTION;")
        << wxT("CREATE TABLE IF NOT EXISTS AppOptions (OptionName TEXT UNIQUE, OptionValue TEXT);")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"Language\", \"\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"ActivePerspective\", \"\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"DefaultPerspective\", \"\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"ActivedInterface\", \"\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"SkipVMInterface\", \"false\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"TftpdAutoStart\", \"true\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"TftpdRoot\", \"\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"RubyDownloadMemory\", \"0x1000000\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"UBootOffset\", \"0x0\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"KernelOffset\", \"0x0\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"FileSystemOffset\", \"0x0\");")
        << wxT("INSERT OR IGNORE INTO AppOptions VALUES (\"SplashOffset\", \"0x0\");")
        << wxT("COMMIT TRANSACTION;");

    _db->ExecuteUpdate(sql);
}

bool AppOptions::SetOption(const wxString &option, const wxString &value)
{
    bool result = true;
    wxString sqlUpdate;

    sqlUpdate << wxT("UPDATE AppOptions SET OptionValue = '") << value
        << wxT("' WHERE OptionName = '") << option << wxT("'");
    if (1 != _db->ExecuteUpdate(sqlUpdate))
        result = false;

    return result;
}

bool AppOptions::SetOption(const wxString &option, const long value)
{
    return SetOption(option, wxString::Format(wxT("%ld"), value));
}

bool AppOptions::SetOption(const wxString &option, const bool value)
{
    wxString val = value ? wxT("true") : wxT("false");
    return SetOption(option, val);
}

bool AppOptions::GetOption(const wxString &option, wxString &value)
{
    wxSQLite3ResultSet set;
    wxString sqlQuery;
    bool result = false;

    sqlQuery 
        << wxT("SELECT OptionValue FROM AppOptions WHERE OptionName = '")
        << option 
        << wxT("'");
    set = _db->ExecuteQuery(sqlQuery);
    if (set.NextRow())
    {
        value = set.GetAsString(0);
        result = true;
    }
    set.Finalize();

    return result;
}

wxString AppOptions::GetOption(const wxString &option)
{
    wxString result;

    if (GetOption(option, result))
        return result;
    else
        return wxEmptyString;
}

long AppOptions::GetOption(const wxString &option, long *value)
{
    long result = 0;
    
    GetOption(option).ToLong(&result);
    if (value)
        *value = result;

    return result;
}

bool AppOptions::GetOption(const wxString &option, bool *value)
{
    bool result;

    if (!GetOption(option).Cmp(wxT("true")))
        result = true;
    else
        result = false;

    if (value)
        *value = result;

    return result;
}

