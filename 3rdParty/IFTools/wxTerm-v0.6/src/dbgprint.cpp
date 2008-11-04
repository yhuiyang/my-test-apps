/////////////////////////////////////////////////////////////////////////////
// Name:        dbgprint.cpp
// Purpose:     
// Author:      Joachim Buermann
// Id:          $Id: dbgprint.cpp,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#include "dbgprint.h"

#ifdef __WIN32__

#include <stdarg.h>
#include <stdio.h>
#include <windows.h>

void dbgprintf(const char* format,...)
{
    char buf[256];
    va_list argptr;
    va_start(argptr,format);

    vsprintf(buf,format,argptr);
    OutputDebugString(buf);

    va_end(argptr);
};
#endif
