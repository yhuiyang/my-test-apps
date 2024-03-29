/*
 *  RockeyThread.cpp - A thread to communicate with Rockey4 USB dongle.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  $Id$
 *
 */

// ------------------------------------------------------------------------
// Headers
// ------------------------------------------------------------------------
#include <wx/wx.h>
#include "PWUpdater.h"
#include "RockeyThread.h"

#ifdef __WXMSW__
#include "Rockey4_ND_32.h"
#define ROCKEY  Rockey
#else
#include "rockey.h"
#define ROCKEY rockey
#endif

#define wxLOG_COMPONENT "PWUpdater/rockey"

// ------------------------------------------------------------------------
// Resources
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------
typedef enum
{
    ROCKEY_STATE_NOT_FOUND,
    ROCKEY_STATE_FOUND,
    ROCKEY_STATE_OPENED,
    ROCKEY_STATE_READ_USER_DATA,
    ROCKEY_STATE_KEY_INSERTED,
} eRockeyState;

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
RockeyThread::RockeyThread(wxEvtHandler *handler, const int id)
    : wxThread(wxTHREAD_DETACHED),
    _pHandler(handler),
    _threadEventId(id)
{
}

RockeyThread::~RockeyThread()
{
    wxCriticalSection &cs = wxGetApp().m_rockeyCS;
    RockeyThread *&pRockey = wxGetApp().m_pRockeyThread;

    cs.Enter();
    pRockey = NULL;
    cs.Leave();
}

wxThread::ExitCode RockeyThread::Entry()
{
    eRockeyState state = ROCKEY_STATE_NOT_FOUND;
    unsigned short u16Handle, u16Result, u16Ignore;
    unsigned short u16BasicPW1, u16BasicPW2, u16AdvPW1, u16AdvPW2;
#if defined (__WXMSW__)
    unsigned long u32Ignore, u32HwId;
#elif defined (__WXGTK__)
    unsigned int u32Ignore, u32HwId;
#endif
    unsigned long u32UserDataLen = 0;
    unsigned char buf[1024];
    unsigned short u16Pos, u16Len;
    wxString userData, user, contact, sn;
    size_t delimitPos1, delimitPos2;
    bool keyInserted = false;

    /* setup basic passwords, never setup adv password within application code */
    u16BasicPW1 = 0xAC31;
    u16BasicPW2 = 0x9C9C;

    while (!TestDestroy())
    {
        switch (state)
        {
        default:
        case ROCKEY_STATE_NOT_FOUND:

            if (keyInserted)
            {
                PostRemovedEvent();
                keyInserted = false;
            }

            u16Result = ROCKEY(RY_FIND, &u16Ignore, &u32HwId, &u32Ignore, &u16BasicPW1, &u16BasicPW2, &u16AdvPW1, &u16AdvPW2, &buf[0]);
            if (u16Result == ERR_SUCCESS)
                state = ROCKEY_STATE_FOUND;
            break;

        case ROCKEY_STATE_FOUND:

            u16Result = ROCKEY(RY_OPEN, &u16Handle, &u32HwId, &u32Ignore, &u16BasicPW1, &u16BasicPW2, &u16AdvPW1, &u16AdvPW2, &buf[0]);
            if (u16Result == ERR_SUCCESS)
                state = ROCKEY_STATE_OPENED;
            else
                state = ROCKEY_STATE_NOT_FOUND;
            break;

        case ROCKEY_STATE_OPENED:

            /* read user data length */
            u16Pos = 500;
            u16Len = 3;
            u16Result = ROCKEY(RY_READ, &u16Handle, &u32Ignore, &u32Ignore, &u16Pos, &u16Len, &u16Ignore, &u16Ignore, &buf[0]);
            if (u16Result == ERR_SUCCESS)
            {
                wxString lenStr;
                lenStr = wxString::From8BitData((const char *)&buf[0], 3);
                u32UserDataLen = 0;
                lenStr.ToULong(&u32UserDataLen);
                if (u32UserDataLen)
                    state = ROCKEY_STATE_READ_USER_DATA;
                else
                    state = ROCKEY_STATE_NOT_FOUND;
            }
            else
                state = ROCKEY_STATE_NOT_FOUND;
            break;

        case ROCKEY_STATE_READ_USER_DATA:

            /* read user data */
            u16Pos = 500 + ((u32UserDataLen < 10) ? 2 : ((u32UserDataLen < 100) ? 3 : 4));
            u16Len = u32UserDataLen & 0xFFFF;
            u16Result = ROCKEY(RY_READ, &u16Handle, &u32Ignore, &u32Ignore, &u16Pos, &u16Len, &u16Ignore, &u16Ignore, &buf[0]);
            if (u16Result == ERR_SUCCESS)
            {
                userData = wxString::From8BitData((const char *)&buf[0], u16Len);
                delimitPos2 = userData.find_last_of(' '); // hash start pos
                delimitPos1 = userData.find_last_of(' ', delimitPos2 - 1); // contact start pos
                user = userData.Mid(0, delimitPos1);
                contact = userData.Mid(delimitPos1 + 1, delimitPos2 - delimitPos1 - 1);
                if (!user.empty() && !contact.empty())
                    state = ROCKEY_STATE_KEY_INSERTED;
                else
                    state = ROCKEY_STATE_NOT_FOUND;
            }
            else
                state = ROCKEY_STATE_NOT_FOUND;
            break;

        case ROCKEY_STATE_KEY_INSERTED:

            if (!keyInserted)
            {
                PostInsertedEvent(user, contact);
                keyInserted = true;

                u16Result = ROCKEY(RY_CLOSE, &u16Handle, &u32Ignore, &u32Ignore, &u16Ignore, &u16Ignore, &u16Ignore, &u16Ignore, &buf[0]);
            }

            /* monitor key existence */
            u16Result = ROCKEY(RY_FIND, &u16Ignore, &u32HwId, &u32Ignore, &u16BasicPW1, &u16BasicPW2, &u16AdvPW1, &u16AdvPW2, &buf[0]);
            if (u16Result != ERR_SUCCESS)
                state = ROCKEY_STATE_NOT_FOUND;
#if defined (__WXGTK__)
            else
            {
                ROCKEY(RY_OPEN, &u16Handle, &u32HwId, &u32Ignore, &u16BasicPW1, &u16BasicPW2, &u16AdvPW1, &u16AdvPW2, &buf[0]);
                ROCKEY(RY_CLOSE, &u16Handle, &u32Ignore, &u32Ignore, &u16Ignore,&u16Ignore, &u16Ignore, &u16Ignore, &buf[0]);
            }
#endif
        }

        wxMilliSleep(1000);
    }

    return (wxThread::ExitCode)0;
}

void RockeyThread::PostInsertedEvent(const wxString &user,
                                     const wxString &contact)
{
    wxThreadEvent evt(wxEVT_COMMAND_THREAD, _threadEventId);
    RockeyMessage msg;

    msg.SetEvent(ROCKEY_EVENT_KEY_INSERTED);
    msg.SetUser(user);
    msg.SetContact(contact);
    evt.SetPayload<RockeyMessage>(msg);
    wxQueueEvent(_pHandler, evt.Clone());
}

void RockeyThread::PostRemovedEvent()
{
    wxThreadEvent evt(wxEVT_COMMAND_THREAD, _threadEventId);
    RockeyMessage msg;

    msg.SetEvent(ROCKEY_EVENT_KEY_REMOVED);
    evt.SetPayload<RockeyMessage>(msg);
    wxQueueEvent(_pHandler, evt.Clone());
}

