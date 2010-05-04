#ifndef _WIDGET_ID_H_
#define _WIDGET_ID_H_

#include <wx/defs.h>

enum
{
    myID_DUMMY = wxID_HIGHEST + 1,

    /* app */
    myID_CXTMENU_ADAPTER_FIRST,
    myID_CXTMENU_ADAPTER_UDP = myID_CXTMENU_ADAPTER_FIRST,
    myID_CXTMENU_ADAPTER_TCP,
    myID_CXTMENU_ADAPTER_LAST,
    myID_CXTMENU_ADAPTER_DUMMY = myID_CXTMENU_ADAPTER_FIRST + (myID_CXTMENU_ADAPTER_LAST - myID_CXTMENU_ADAPTER_FIRST) * 10,

    /* frame - menu bar */
    myID_VIEW_PANE_START,
    myID_VIEW_RADIO_PANE_START,
    myID_VIEW_MAC_ADDR_USAGE_PANE,
    myID_VIEW_DOWNLOAD_PANE,
    myID_VIEW_RADIO_PANE_END,
    myID_VIEW_CHECK_PANE_START,
    myID_VIEW_LOG_PANE,
    myID_VIEW_OPTION_PANE,
    myID_VIEW_CHECK_PANE_END,
    myID_VIEW_PANE_END,
    myID_VIEW_RESET_LAYOUT,
    myID_HELP_DOC,

    /* frame - status bar */
    myID_SBAR,

    /* download */
    myID_TARGET_CHECK_ALL,
    myID_TARGET_UNCHECK_ALL,
    myID_DOWNLOAD_TARGET_LIST_SELECT_NONE,
    myID_DOWNLOAD_SEARCH_BTN,
    myID_DOWNLOAD_TARGET_LIST,
    myID_DOWNLOAD_SPECIFIC_RB,
    myID_DOWNLOAD_GLOBAL_RB,
    myID_DOWNLOAD_GLOBAL_FILE,
    myID_DOWNLOAD_SELECTED_BTN,
    myID_MODIFY_MAC_BTN,

    /* log */

    /* preference */
    myID_REPORT_ENTRIES_DESC,
    myID_REPORT_ENTRIES_NUM,
    myID_MAC_VENDOR_CODE,
    myID_MAC_PRODUCT_CODE_1ST,
    myID_MAC_PRODUCT_CODE_LAST,
    myID_UPDATE_VENDOR_PRODUCT_CODE_BTN,
    myID_MAC_INVALID,
    myID_UPDATE_INVALID_MAC_BTN,

    /* mac update */
    myID_MAC_UPDATE_NETADDR_TEXTCTRL,

    /* threads */
    myID_SEARCH_THREAD,
    myID_UPDATE_THREAD,
};

#endif /* _WIDGET_ID_H_ */
