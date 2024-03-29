#ifndef _NET_ADAPTER_H_
#define _NET_ADAPTER_H_

#include <wx/wx.h>

class NetAdapter
{
public:
    NetAdapter(wxString name, wxString ip, wxString netmask,
        wxString broadcast)
    {
        _name = name;
        _ip = ip;
        _netmask = netmask;
        _broadcast = broadcast;
    }
    wxString& GetName(void) { return _name; }
    wxString& GetIp(void) { return _ip; }
    wxString& GetNetmask(void) { return _netmask; }
    wxString& GetBroadcast(void) { return _broadcast; }

private:
    wxString _name;
    wxString _ip;
    wxString _netmask;
    wxString _broadcast;
};

#endif /* _NET_ADAPTER_H_ */

