#ifndef _WIDGETS_ID_H_
#define _WIDGETS_ID_H_

#include <wx/defs.h>

enum
{
    myID_DUMMY = wxID_HIGHEST + 1,
    
    /* frame - menu bar */

    /* download pane */
    myID_BTN_START_TFTP,
    myID_BTN_STOP_TFTP,

    /* preference dialog */
    myID_PREF_DLG,
    myID_PREF_UI_LANG,
    myID_PREF_UI_LAYOUT_MEMORY,
    myID_PREF_UI_POS_SIZE_MEMORY,
    myID_PREF_TFTP_AUTOSTART,
    myID_PREF_TFTP_INTERFACE,
    myID_PREF_TFTP_ROOTPATH,
    myID_PREF_TFTP_TIMEOUT,
    myID_PREF_TFTP_RETRANSMIT,
    myID_PREF_TFTP_NEGOTIATION,
    myID_PREF_FLASH_DL_OFFSET,
    myID_PREF_FLASH_SPI_UBOOT_OFFSET,
    myID_PREF_FLASH_SPI_UBOOT_IMAGE,
    myID_PREF_FLASH_UBOOT_OFFSET,
    myID_PREF_FLASH_UBOOT_IMAGE,
    myID_PREF_FLASH_KERNEL_OFFSET,
    myID_PREF_FLASH_KERNEL_IMAGE,
    myID_PREF_FLASH_FS_OFFSET,
    myID_PREF_FLASH_FS_IMAGE,
    myID_PREF_FLASH_SPLASH_OFFSET,
    myID_PREF_FLASH_SPLASH_IMAGE,

    /* threads */
    myID_THREAD_TFTPD,
};

#endif /* _WIDGETS_ID_H_ */

    