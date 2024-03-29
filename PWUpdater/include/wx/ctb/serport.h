/////////////////////////////////////////////////////////////////////////////
// Name:        serport.h
// Purpose:     simple wrapper file
// Author:      Joachim Buermann
// Id:          $Id: serport.h,v 1.1.1.1 2004/11/24 10:30:11 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SERIALPORT_BASE_H
#define _WX_SERIALPORT_BASE_H

#if defined (__WXMSW__)
#include "serportx.h"
#include <windows.h>

#define wxCOM1 "com1"
#define wxCOM2 "com2"
#define wxCOM3 "com3"
#define wxCOM4 "com4"
#define wxCOM5 "com5"
#define wxCOM6 "com6"
#define wxCOM7 "com7"
#define wxCOM8 "com8"
#define wxCOM9 "com9"
#define wxCOM10 "\\\\.\\com10"
#define wxCOM11 "\\\\.\\com11"
#define wxCOM12 "\\\\.\\com12"
#define wxCOM13 "\\\\.\\com13"
#define wxCOM14 "\\\\.\\com14"
#define wxCOM15 "\\\\.\\com15"
#define wxCOM16 "\\\\.\\com16"

/*!
   \class wxSerialPort
   
   \brief the win32 version
*/
class wxSerialPort : public wxSerialPort_x
{
protected:
    /*!
      \brief the win32 api equivalent for the Linux
      file descriptor
    */
    HANDLE fd;
    /*!
      \brief a very special struct in the win32 api for controling
      an asynchronous serial communication through the com ports.
    */
    OVERLAPPED m_ov;

    /*!
      \brief The win32 API only allows to test for an existing
      break, framing, overrun or parity, not for the occured numbers.
      So every error event decrement this internal error struct and
      can request by an Ioctl call.
    */
    wxSerialPort_EINFO einfo;
    
    /*!
     \brief The win32 API doesn't have any function to detect the
     current state of the output lines RST and DTR (if someone knows
     some function, please contact me).
     So we save the state always after changing one or both lines
     (for example, on Open, SetLineState and ChangeLineState)
    */
    int m_rtsdtr_state;

    int CloseDevice();
    int OpenDevice(const char *devname, void *dcs);
public:
    wxSerialPort();
    ~wxSerialPort();

    int ChangeLineState(wxSerialLineState flags);
    int ClrLineState(wxSerialLineState flags);
    int GetLineState();
    int Ioctl(int cmd, void *args);
    int IsOpen();
    int Read(char *buf, size_t len);
    int SendBreak(int duration);
    int SetBaudRate(wxBaud baudrate);
    int SetLineState(wxSerialLineState flags);
    int Write(char *buf, size_t len);
};
#elif defined (__WXGTK__)

#include "wx/ctb/serportx.h"
#include <linux/serial.h>
#include <termios.h>

#define wxCOM1 "/dev/ttyS0"
#define wxCOM2 "/dev/ttyS1"
#define wxCOM3 "/dev/ttyS2"
#define wxCOM4 "/dev/ttyS3"
#define wxCOM5 "/dev/ttyS4"
#define wxCOM6 "/dev/ttyS5"
#define wxCOM7 "/dev/ttyS6"
#define wxCOM8 "/dev/ttyS7"
#define wxCOM9 "/dev/ttyS8"

/*!
   \class wxSerialPort
   
   \brief the linux version
*/
class wxSerialPort : public wxSerialPort_x
{
protected:
    /*!
      \brief under Linux, the serial ports are normal file descriptor
    */
    int fd;
    /*!
      \brief Linux defines this struct termios for controling asynchronous
      communication. t covered the active settings, save_t the original 
      settings.
    */
    struct termios t, save_t;

    /*!
      \brief The Linux serial driver summing all breaks, framings, overruns
      and parity errors for each port during system runtime. Because we only
      need the errors during a active connection, we must save the actual
      error numbers in this separate structurs.
    */
    struct serial_icounter_struct save_info, last_info;

    /*!
      \brief adaptor member function, to convert the plattform independent
      type wxBaud into a linux conform value.
      \param baud the baudrate as wxBaud type
      \return speed_t linux specific data type, defined in termios.h
    */
    speed_t AdaptBaudrate(wxBaud baud);
    
    int CloseDevice();
    int OpenDevice(const char *devname, void *dcs);
public:
    wxSerialPort();
    ~wxSerialPort();

    int ChangeLineState(wxSerialLineState flags);
    int ClrLineState(wxSerialLineState flags);
    int GetLineState();
    int Ioctl(int cmd, void *args);
    int IsOpen();
    int Read(char *buf, size_t len);
    int SendBreak(int duration);
    int SetBaudRate(wxBaud baudrate);
    int SetLineState(wxSerialLineState flags);
    int Write(char *buf, size_t len);
};

#endif

#endif
// __SERPORT_BASE_H

