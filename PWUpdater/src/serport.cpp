/////////////////////////////////////////////////////////////////////////////
// Name:        serport.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: serport.cpp,v 1.1.1.1 2004/11/24 10:30:11 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined (__WXMSW__)

#include <string.h>
#include "wx/ctb/serport.h"

#define wxSERIALPORT_BUFSIZE 4096

wxSerialPort::wxSerialPort()
{
    memset(&m_ov, 0, sizeof(OVERLAPPED));
    fd = INVALID_HANDLE_VALUE;
    m_rtsdtr_state = wxSERIAL_LINESTATE_NULL;
};

wxSerialPort::~wxSerialPort()
{
    Close();
};

int wxSerialPort::CloseDevice()
{
    if (fd != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_ov.hEvent);
        CloseHandle(fd);
        fd = INVALID_HANDLE_VALUE;
    }
    return 0;
};

int wxSerialPort::ChangeLineState(wxSerialLineState flags)
{
#if (1)
    BOOL ok = false;
    if (flags & wxSERIAL_LINESTATE_DTR)
    {
        if (m_rtsdtr_state & wxSERIAL_LINESTATE_DTR)
        {
            ok = EscapeCommFunction(fd, CLRDTR);
        }
        else
        {
            ok = EscapeCommFunction(fd, SETDTR);
        }
        m_rtsdtr_state ^= wxSERIAL_LINESTATE_DTR;
    }
    if (flags & wxSERIAL_LINESTATE_RTS)
    {
        if (m_rtsdtr_state & wxSERIAL_LINESTATE_RTS)
        {
            ok = EscapeCommFunction(fd, CLRRTS);
        }
        else
        {
            ok = EscapeCommFunction(fd, SETRTS);
        }
        m_rtsdtr_state ^= wxSERIAL_LINESTATE_RTS;
    }
    if (!ok) return -1;
    return 0;
#else
    BOOL ok = false;
    if (flags & wxSERIAL_LINESTATE_DTR)
        ok = EscapeCommFunction(fd, SETDTR);
    else
        ok = EscapeCommFunction(fd, CLRDTR);
    if (flags & wxSERIAL_LINESTATE_RTS)
        ok = EscapeCommFunction(fd, SETRTS);
    else
        ok = EscapeCommFunction(fd, CLRRTS);
    if (!ok) return -1;
    return 0;
#endif
};

int wxSerialPort::ClrLineState(wxSerialLineState flags)
{
    BOOL ok = false;
    if (flags & wxSERIAL_LINESTATE_DTR)
    {
        ok = EscapeCommFunction(fd, CLRDTR);
        m_rtsdtr_state &= ~wxSERIAL_LINESTATE_DTR;
    }
    if (flags & wxSERIAL_LINESTATE_RTS)
    {
        ok = EscapeCommFunction(fd, CLRRTS);
        m_rtsdtr_state &= ~wxSERIAL_LINESTATE_RTS;
    }
    if (!ok) return -1;
    return 0;
};

int wxSerialPort::GetLineState()
{
    BOOL ok = false;
    DWORD stat;
    int flags = 0;
    if(GetCommModemStatus(fd, &stat))
    {
        if (stat & MS_CTS_ON)
            flags |= wxSERIAL_LINESTATE_CTS;
        if (stat & MS_DSR_ON)
            flags |= wxSERIAL_LINESTATE_DSR;
        if (stat & MS_RING_ON)
            flags |= wxSERIAL_LINESTATE_RING;
        if (stat & MS_RLSD_ON)
            flags |= wxSERIAL_LINESTATE_DCD;
        ok = true;
    }
    if (!ok) return -1;
    return flags;
};

int wxSerialPort::Ioctl(int cmd, void *args)
{
    COMSTAT comstat;
    DWORD errors;
    int result = 0;

    switch (cmd)
    {
    case CTB_RESET:
        return SendBreak(0);
    case CTB_SER_GETEINFO:
        if (ClearCommError(fd, &errors, &comstat))
        {
            // actualize the last events
            if (errors & CE_BREAK) einfo.brk++;
            if (errors & CE_FRAME) einfo.frame++;
            if (errors & CE_OVERRUN) einfo.overrun++;
            if (errors & CE_RXPARITY) einfo.parity++;
            *(wxSerialPort_EINFO *)args = einfo;
            return 0;
        }
    case CTB_SER_GETBRK:
        if (ClearCommError(fd, &errors, &comstat))
        {
            if (errors & CE_BREAK) result = 1;
            einfo.brk += result;
            *(int *)args = result;
            return 0;
        }
        break;
    case CTB_SER_GETFRM:
        if (ClearCommError(fd, &errors, &comstat))
        {
            if (errors & CE_FRAME) result = 1;
            einfo.frame += result;
            *(int *)args = result;
            return 0;
        }
    case CTB_SER_GETOVR:
        if (ClearCommError(fd, &errors, &comstat))
        {
            if (errors & CE_OVERRUN) result = 1;
            einfo.overrun += result;
            *(int *)args = result;
            return 0;
        }
        break;
    case CTB_SER_GETPAR:
        if (ClearCommError(fd, &errors, &comstat))
        {
            if (errors & CE_RXPARITY) result = 1;
            einfo.parity += result;
            *(int *)args = result;
            return 0;
        }
        break;
    case CTB_SER_GETINQUE:
        if (ClearCommError(fd, &errors, &comstat))
        {
            *(int *)args = (int)comstat.cbInQue;
            return 0;
        }
        break;
    }
    // error or unknown command
    return -1;
};

int wxSerialPort::IsOpen()
{
    return (fd != INVALID_HANDLE_VALUE);
};

int wxSerialPort::OpenDevice(const char *devname, void *dcs)
{
    // if dcs isn't NULL, type cast
    if (dcs) m_dcs = *(wxSerialPort_DCS *)dcs;

    fd = CreateFile(devname,            // device name
        GENERIC_READ | GENERIC_WRITE,   // O_RDWR
        0,                              // not shared
        NULL,                           // default value for object security ?!?
        OPEN_EXISTING,                  // file (device) exists
        FILE_FLAG_OVERLAPPED,           // asynchron handling
        NULL);                          // no more handle flags

    if (fd == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    // save the device name
    strncpy_s(m_devname, (char *)devname, sizeof(m_devname));
    // we write an eos to avoid a buffer overflow
    m_devname[sizeof(m_devname) - 1] = '\0';

    // device control block
    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    dcb.DCBlength = sizeof(dcb);
    dcb.BaudRate = m_dcs.baud;
    dcb.fBinary = 1;

    m_rtsdtr_state = wxSERIAL_LINESTATE_NULL;

    // Specifies whether the CTS (clear-to-send) signal is monitored
    // for output flow control. If this member is TRUE and CTS is turned
    // off, output is suspended until CTS is sent again.
    dcb.fOutxCtsFlow = m_dcs.rtscts;

    // Specifies the DTR (data-terminal-ready) flow control.
    // This member can be one of the following values:
    // DTR_CONTROL_DISABLE   Disables the DTR line when the device is
    //                       opened and leaves it disabled.
    // DTR_CONTROL_ENABLE    Enables the DTR line when the device is
    //                       opened and leaves it on.
    // DTR_CONTROL_HANDSHAKE Enables DTR handshaking. If handshaking is
    //                       enabled, it is an error for the application
    //                       to adjust the line by using the
    //                       EscapeCommFunction function.
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    m_rtsdtr_state |= wxSERIAL_LINESTATE_DTR;
    // Specifies the RTS flow control. If this value is zero, the
    // default is RTS_CONTROL_HANDSHAKE. This member can be one of
    // the following values:
    // RTS_CONTROL_DISABLE   Disables the RTS line when device is
    //                       opened and leaves it disabled.
    // RTS_CONTROL_ENABLED   Enables the RTS line when device is
    //                       opened and leaves it on.
    // RTS_CONTROL_HANDSHAKE Enables RTS handshaking. The driver
    //                       raises the RTS line when the
    //                       "type-ahead" (input)buffer is less than
    //                       one-half full and lowers the RTS line
    //                       when the buffer is more than three-quarters
    //                       full. If handshaking is enabled, it is an
    //                       error for the application to adjust the
    //                       line by using the EscapeCommFunction function.
    // RTS_CONTROL_TOGGLE    Specifies that the RTS line will be high if
    //                       bytes are available for transmission. After
    //                       all buffered bytes have been send, the RTS
    //                       line will be low.
    if (m_dcs.rtscts) dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    else
    {
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
        m_rtsdtr_state |= wxSERIAL_LINESTATE_RTS;
    }
    // Specifies the XON/XOFF flow control.
    // If fOutX is true (the default is false), transmission stops when the
    // XOFF character is received and starts again, when the XON character
    // is received.
    dcb.fOutX = m_dcs.xonxoff;
    // If fInX is true (default is false), the XOFF character is sent when
    // the input buffer comes within XoffLim bytes of being full, and the
    // XON character is sent, when the input buffer comes within XonLim
    // bytes of being empty.
    dcb.fInX = m_dcs.xonxoff;
    // default character for XOFF is 0x13 (hex 13)
    dcb.XoffChar = 0x13;
    // default character for XON is 0x11 (hex 11)
    dcb.XonChar = 0x11;
    // set the minimum number of bytes allowed in the input buffer before
    // the XON character is sent (3/4 of full size)
    dcb.XonLim = (wxSERIALPORT_BUFSIZE >> 2) * 3;
    // set the maximum number of free bytes in the input buffer, before the
    // XOFF character is sent (3/4 of full size)
    dcb.XoffLim = (wxSERIALPORT_BUFSIZE >> 2) * 3;

    // parity
    if (m_dcs.parity == wxPARITY_ODD) dcb.Parity = ODDPARITY;
    else if (m_dcs.parity == wxPARITY_EVEN) dcb.Parity = EVENPARITY;
    else dcb.Parity = NOPARITY;
    // stopbits
    if (m_dcs.stopbits == 2) dcb.StopBits = TWOSTOPBITS;
    else dcb.StopBits = ONESTOPBIT;
    // wordlen, valid values are 5,6,7,8
    dcb.ByteSize = m_dcs.wordlen;

    if (!SetCommState(fd, &dcb))
        return -2;

    // create event for overlapped I/O
    // we need a event object, which inform us about the
    // end of an operation (here reading device)
    m_ov.hEvent = CreateEvent(NULL, // LPSECURITY_ATTRIBUTES lpsa
        TRUE,                       // BOOL fManualReset
        TRUE,                       // BOOL fInitialState
        NULL);                      // LPTSTR lpszEventName
    if (m_ov.hEvent == INVALID_HANDLE_VALUE)
    {
        return -3;
    }

    /* THIS IS OBSOLETE!!!
    // event should be triggered, if there are some received data
    if (!SetCommMask(fd, EV_RXCHAR))
        return -4;
    */

    COMMTIMEOUTS cto = { MAXDWORD, 0, 0, 0, 0 };
    if (!SetCommTimeouts(fd, &cto))
        return -5;

    // for a better performance with win95/98 I increased the internal
    // buffer to wxSERIALPORT_BUFSIZE (normal size is 1024, but this can
    // be a little bit to small, if you use a higher baudrate like 115200)
    if (!SetupComm(fd, wxSERIALPORT_BUFSIZE, wxSERIALPORT_BUFSIZE))
        return -6;

    // clear the internal error struct
    memset(&einfo, 0, sizeof(einfo));
    return 0;
};

int wxSerialPort::Read(char *buf, size_t len)
{
    DWORD read;
    int m = m_fifo->items();
    while (len)
    {
        if (m_fifo->get(buf) == 1)
        {
            len--;
            buf++;
        }
        else
        {
            break;
        }
    }
    if (!ReadFile(fd, buf, len, &read, &m_ov))
    {
        // if we use a asynchrone reading, ReadFile gives always
        // FALSE
        // ERROR_IO_PENDING means ok, other values show an error
        if (GetLastError() != ERROR_IO_PENDING)
        {
            // oops..., error in communication
            return -1;
        }
    }
    else
    {
        // ok, we have read all wanted bytes
        return (int)read + m;
    }
    return 0;
};

int wxSerialPort::SendBreak(int duration)
{
    if (duration <= 0) duration = 1;
    if (!SetCommBreak(fd)) return -1;
    // win32 Sleep parameter is ms
    Sleep(duration * 250);
    if (!ClearCommBreak(fd)) return -1;
    // no error
    return 0;
};

int wxSerialPort::SetBaudRate(wxBaud baudrate)
{
    DCB dcb;
    int baud;
    switch (baudrate)
    {
    case wxBAUD_1200:
        baud = CBR_1200;
        break;
    case wxBAUD_2400:
        baud = CBR_2400;
        break;
    case wxBAUD_4800:
        baud = CBR_4800;
        break;
    case wxBAUD_9600:
        baud = CBR_9600;
        break;
    case wxBAUD_19200:
        baud = CBR_19200;
        break;
    case wxBAUD_38400:
        baud = CBR_38400;
        break;
    case wxBAUD_57600:
        baud = CBR_57600;
        break;
    case wxBAUD_115200:
        baud = CBR_115200;
        break;
    case wxBAUD_230400:
        baud = 230400;
        break;
    case wxBAUD_460800:
        baud = 460800;
        break;
    case wxBAUD_921600:
        baud = 921600;
        break;
    default:
        return -1;
    }
    // get the current dcb...
    if (!GetCommState(fd, &dcb))
    {
        return -1;
    }
    dcb.BaudRate = baud;
    // and write it back
    if (!SetCommState(fd, &dcb))
    {
        return -1;
    }
    m_dcs.baud = baudrate;
    return 0;
};

int wxSerialPort::SetLineState(wxSerialLineState flags)
{
    BOOL ok = false;
    if (flags & wxSERIAL_LINESTATE_DTR)
    {
        ok = EscapeCommFunction(fd, SETDTR);
        // save the new state
        m_rtsdtr_state |= wxSERIAL_LINESTATE_DTR;
    }
    if (flags & wxSERIAL_LINESTATE_RTS)
    {
        ok = EscapeCommFunction(fd, SETRTS);
        // save the new state
        m_rtsdtr_state |= wxSERIAL_LINESTATE_RTS;
    }
    if (!ok) return -1;
    return 0;
};

int wxSerialPort::Write(char *buf, size_t len)
{
    DWORD write;
    if (!WriteFile(fd, buf, len, &write, &m_ov))
    {
        if (GetLastError() != ERROR_IO_PENDING)
        {
            return -1;
        }
        else
        {
            // VERY IMPORTANT to flush the data out of the internal
            // buffer
            FlushFileBuffers(fd);
            // first you must call GetOverlappedResult, then you
            // get the REALLY transmitted count of bytes
            if (!GetOverlappedResult(fd, &m_ov, &write, TRUE))
            {
                // ooops... something is going wrong
                return (int)write;
            }
        }
    }
    return write;
};

#elif defined (__WXGTK__)

#include "wx/ctb/serport.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

wxSerialPort::wxSerialPort() :
    wxSerialPort_x()
{
    fd = -1;
};

wxSerialPort::~wxSerialPort()
{
    Close();
};

speed_t wxSerialPort::AdaptBaudrate(wxBaud baud)
{
    switch (baud)
    {
    case wxBAUD_150: return B150;
    case wxBAUD_300: return B300;
    case wxBAUD_600: return B600;
    case wxBAUD_1200: return B1200;
    case wxBAUD_2400: return B2400;
    case wxBAUD_4800: return B4800;
    case wxBAUD_9600: return B9600;
    case wxBAUD_38400: return B38400;
    case wxBAUD_57600: return B57600;
    case wxBAUD_115200: return B115200;
    case wxBAUD_230400: return B230400;
    case wxBAUD_460800: return B460800;
    case wxBAUD_921600: return B921600;
    default: return B19200;
    }
};

int wxSerialPort::CloseDevice()
{
    int err = 0;
    // only close an open file handle
    if (fd < 0) return EBADF;
    // With some systems, it is recommended to flush the serial port's
    // Output before closing it, in order to avoid a possible hang of
    // the process...
    // Thanks to Germain (I couldn't answer you, because your email
    // address was invalid)
    tcflush(fd, TCOFLUSH);
    // recovery original settings
    tcsetattr(fd,TCSANOW, &save_t);
    // and close device
    err = close(fd);
    fd = -1;
    return err;
};

int wxSerialPort::ChangeLineState(wxSerialLineState flags)
{
    int state;
    ioctl(fd, TIOCMGET, &state);
    state ^= flags;
    return ioctl(fd, TIOCMSET, &state);
};

int wxSerialPort::ClrLineState(wxSerialLineState flags)
{
    return ioctl(fd, TIOCMBIC, &flags);
};

int wxSerialPort::GetLineState()
{
    wxSerialLineState flags = wxSERIAL_LINESTATE_NULL;
    if (ioctl(fd, TIOCMGET, &flags) < 0)
    {
        return -1;
    }
    return (int)(flags & 0x1FF);
};

//
// included from /usr/include/linux/serial.h
//
//  struct serial_icounter_struct {
//      int cts, dsr, rng, dcd;
//      int rx, tx;
//      int frame, overrun, parity, brk;
//      int buf_overrun;
//      int reserved[9];
//  };
//
int wxSerialPort::Ioctl(int cmd, void *args)
{
    int count = 0;
    int err = 0;
    struct serial_icounter_struct info;
    wxSerialPort_EINFO einfo;

    switch (cmd)
    {
    case CTB_RESET:
        return SendBreak(0);
    case CTB_SER_GETEINFO:
        err = ioctl(fd, TIOCGICOUNT, &info);
        if (err) return err;
        einfo.brk = info.brk - save_info.brk;
        einfo.frame = info.frame - save_info.frame;
        einfo.overrun = info.overrun - save_info.overrun;
        einfo.parity = info.parity - save_info.parity;
        *(wxSerialPort_EINFO *)args = einfo;
        break;
    case CTB_SER_GETBRK:
        err = ioctl(fd, TIOCGICOUNT, &info);
        if (err) return err;
        if (last_info.brk != info.brk) count = 1;
        break;
    case CTB_SER_GETFRM:
        err = ioctl(fd, TIOCGICOUNT, &info);
        if (err) return err;
        if (last_info.frame != info.frame) count = 1;
        break;
    case CTB_SER_GETOVR:
        err = ioctl(fd, TIOCGICOUNT, &info);
        if (err) return err;
        if (last_info.overrun != info.overrun) count = 1;
        break;
    case CTB_SER_GETPAR:
        err = ioctl(fd, TIOCGICOUNT, &info);
        if (err) return err;
        if (last_info.parity != info.parity) count = 1;
        break;
    case CTB_SER_GETINQUE:
        err = ioctl(fd, TIOCINQ, &count);
        if (err) return err;
        *(int *)args = count;
        return 0;
    default:
        return -1;
    }
    last_info = info;
    return 0;
};

int wxSerialPort::IsOpen()
{
    return (fd != -1);
};

int wxSerialPort::OpenDevice(const char *devname, void *dcs)
{
    // if dcs isn't NULL, type cast
    if (dcs) m_dcs = *(wxSerialPort_DCS *)dcs;
    // open serial comport device for reading and writing,
    // don't wait (O_NONBLOCK)
    fd = open(devname, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd >= 0)
    {
        tcgetattr(fd, &t);
        save_t = t;

        // save the device name
        strncpy(m_devname, (char*)devname, sizeof(m_devname));
        // we write an eos to avoid a buffer overflow
        m_devname[sizeof(m_devname) - 1] = '\0';

        // fill the internal terios struct
        // default input/output speed is 19200 baud
        cfsetispeed(&t, AdaptBaudrate(m_dcs.baud));
        cfsetospeed(&t, AdaptBaudrate(m_dcs.baud));
        // parity settings
        if (m_dcs.parity == wxPARITY_NONE)
            t.c_cflag &= ~PARENB;
        else
        {
            t.c_cflag |= PARENB;
            if (m_dcs.parity == wxPARITY_ODD)
                t.c_cflag |= PARODD;
            else
                t.c_cflag &= ~PARODD;
        }
        // stopbits
        if (m_dcs.stopbits == 2)
            t.c_cflag |= CSTOPB;
        else
            t.c_cflag &= ~CSTOPB;
        // wordlen
        t.c_cflag &= ~CSIZE;
        if (m_dcs.wordlen == 7) t.c_cflag |= CS7;
        else if(m_dcs.wordlen == 6) t.c_cflag |= CS6;
        else if(m_dcs.wordlen == 5) t.c_cflag |= CS5;
        // this is the default
        else t.c_cflag |= CS8;
        // rts/cts
        if (m_dcs.rtscts == false)
            t.c_cflag &= ~CRTSCTS;
        else
            t.c_cflag |= CRTSCTS;

        t.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
        t.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON | IXOFF | IXANY);
        t.c_iflag |= IGNPAR;
        t.c_oflag &= ~OPOST;

        if (m_dcs.xonxoff == true)
        {
            t.c_iflag |= (IXON | IXOFF);
        }

        // look out!
        // MIN = 1 means, in TIME (1/10 secs) defined timeout
        // will be started AFTER receiving the first byte
        // so we must set MIN = 0. (timeout starts immediately, abort
        // also without readed byte)
        t.c_cc[VMIN] = 0;
        // timeout in 1/10 secs
        // no timeout for non blocked transfer
        t.c_cc[VTIME] = 0;
        // write the settings
        tcsetattr(fd, TCSANOW, &t);
        // it's careless, but in the moment we don't test
        // the return of tcsetattr (normally there is no error)

        // request the actual numbers of breaks, framing, overrun
        // and parity errors (because Linux summing all of them during
        // system lifetime, not only while serial port is open.
        ioctl(fd, TIOCGICOUNT, &save_info);
        // it's also careless, but we assume, that there was no error
        last_info = save_info;
    }
    return fd;
};

int wxSerialPort::Read(char *buf, size_t len)
{
    if (m_fifo->items() > 0)
    {
        return m_fifo->read(buf, len);
    }
    // Read() (using read() ) will return an 'error' EAGAIN as it is
    // set to non-blocking. This is not a true error within the
    // functionality of Read, and thus should be handled by the caller.
    int n = read(fd, buf, len);
    if ((n < 0) && (errno == EAGAIN)) return 0;
    return n;
};

int wxSerialPort::SendBreak(int duration)
{
    // the parameter is equal with linux
    return tcsendbreak(fd, duration);
};

int wxSerialPort::SetBaudRate(wxBaud baudrate)
{
    speed_t baud = AdaptBaudrate(baudrate);
    // setting the input baudrate
    if (cfsetispeed(&t, baud) < 0)
    {
        return -1;
    }
    // setting the output baudrate
    if (cfsetospeed(&t, baud) < 0)
    {
        return -1;
    }
    // take over
    m_dcs.baud = baudrate;
    return tcsetattr(fd, TCSANOW, &t);
};

int wxSerialPort::SetLineState(wxSerialLineState flags)
{
    return ioctl(fd, TIOCMBIS, &flags);
};

int wxSerialPort::Write(char *buf, size_t len)
{
    // Write() (using write() ) will return an 'error' EAGAIN as it is
    // set to non-blocking. This is not a true error within the
    // functionality of Write, and thus should be handled by the caller.
    int n = write(fd, buf, len);
    if ((n < 0) && (errno == EAGAIN)) return 0;
    return n;
};

#endif
