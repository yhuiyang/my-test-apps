/////////////////////////////////////////////////////////////////////////////
// Name:        debug.h
// Purpose:     only for debugging
// Author:      Joachim Buermann
// Id:          $Id: dbgprint.h,v 1.1.1.1 2005/05/11 07:40:05 jb Exp $
// Copyright:   (c) 2001-2004 Joachim Buermann
/////////////////////////////////////////////////////////////////////////////

#ifndef __DEBUG_H
#define __DEBUG_H
/*
debug.h
*/

#ifdef __WIN32__
void dbgprintf(const char* format,...);
#else
# define dbgprintf printf
#endif

#endif
