/////////////////////////////////////////////////////////////////////////////
// Name:        timer.h
// Purpose:     simple wrapper file
// Author:      Joachim Buermann
// Id:          $Id: timer.h,v 1.1.1.1 2004/11/24 10:30:11 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TIMER_BASE_H
#define _WX_TIMER_BASE_H

#if defined (__WXMSW__)
#include <windows.h>

/*!
  \brief A data struct, using from class timer.
*/
struct timer_control
{
    /*!
      under win32, we used msec internally
    */
    unsigned int msecs;
    /*!
      covers the adress of the exitflag
    */
    int *exitflag;
    /*!
      the running thread steadily polls this variable (only under win32).
      If it's not zero, the thread terminate.
     */
    MMRESULT stop;
    /*! 
      covers the adress of the exit function. NULL, if
      there was no exit function.
    */
    void *(*exitfnc)(void *);
};

/*!
  \class timer
  \brief A thread based timer class for handling timeouts in
  an easier way.

  On starting every timer instance will create it's own thread.
  The thread makes simply nothing, until it's given time is over.
  After that, he set a variable, refer by it's adress to one
  and exit.

  There are a lot of situations, which the timer class must handle.
  The timer instance leaves his valid range (for example, the 
  timer instance is local inside a function, and the function fished)
  BEFORE the thread was ending. In this case, the destructor must 
  terminate the thread in a correct way. (This is very different
  between the OS. threads are a system resource like file descriptors
  and must be deallocated after using it).

  The thread should be asynchronously stopped. Means, under all
  circumstance, it must be possible, to finish the timer and start
  it again.

  Several timer instance can be used simultanously.
*/
class timer
{
protected:
    /*!
      \brief win32 needs this
    */
    DWORD id;
    /*!
      \brief in win32 internal handle for the timer thread
    */
    MMRESULT h;
    /*!
      control covers the time interval, the adress
      of the exitflag, and if not null, a function, which will
      be called on the end.
    */
    timer_control control;
    /*!
      here we store the time interval, whilst the timer run.
      This is waste!!!
    */
    unsigned int timer_secs;
public:
    /*!
      The constructor creates an timer object with the given
      properties. The timer at this moment is not started. This
      will be done with the start() member function.
      \param msec time interval after that the the variable
      pointed by exitflag is setting to one.
      \param exitflag the adress of an integer, which was set
      to one after the given time interval.
      \warning The integer variable shouldn't leave it's valid
      range, before the timer was finished. So never take a
      local variable.
      \param exitfnc A function, which was called after msec.
      If you don't want this, refer a NULL pointer.
    */
    timer(unsigned int msec, int *exitflag, void *(*exitfnc)(void *));
    /*!
      the destructor. If his was called (for example by leaving the
      valid range of the timer object), the timer thread automaticaly
      will finished. The exitflag wouldn't be set, also the exitfnc
      wouldn't be called.
    */
    ~timer();
    /*!
      starts the timer. But now a thread will created and started.
      After this, the timer thread will be running until he was stopped 
      by calling stop() or reached his given time interval.
    */
    int start();
    /*!
      stops the timer and canceled the timer thread. After timer::stop() a new
      start() will started the timer from beginning.
    */
    int stop();
};

/*!
  \fn kill_all_timer()
  \warning This function don't works under win32 and will be removed next
  time. So don't use that!
*/
void kill_all_timer();

/*!
  \fn
  A plattform independent function, to go to sleep for the given
  time interval.
  \param ms time interval in milli seconds
*/
void sleepms(unsigned int ms);

#elif defined (__WXGTK__)
#include <pthread.h>

/*!
  \brief A data struct, using from class timer.
*/
struct timer_control
{
    /*!
      under linux, we used usec internally
    */
    unsigned int usecs;
    /*!
      covers the adress of the exitflag
    */
    int *exitflag;
    /*! 
      covers the adress of the exit function. NULL, if
      there was no exit function.
    */
    void *(*exitfnc)(void *);
};

/*!
  \class timer
  \brief A thread based timer class for handling timeouts in
  an easier way.

  On starting every timer instance will create it's own thread.
  The thread makes simply nothing, until it's given time is over.
  After that, he set a variable, refer by it's adress to one
  and exit.

  There are a lot of situations, which the timer class must handle.
  The timer instance leaves his valid range (for example, the 
  timer instance is local inside a function, and the function fished)
  BEFORE the thread was ending. In this case, the destructor must 
  terminate the thread in a correct way. (This is very different
  between the OS. threads are a system resource like file descriptors
  and must be deallocated after using it).

  The thread should be asynchronously stopped. Means, under all
  circumstance, it must be possible, to finish the timer and start
  it again.

  Several timer instance can be used simultanously.
*/
class timer
{
protected:
    /*!
      control covers the time interval, the adress
      of the exitflag, and if not null, a function, which will
      be called on the end.
    */
    timer_control control;
    /*!
      stopped will be set by calling the stop() method.
      Internaly the timer thread steadily tests the state of 
      this variable. If stopped not zero, the thread will be
      finished.
    */
    int stopped;
    /*!
      under linux we use the pthread library. tid covers the
      identifier for a separate threads.
    */
    pthread_t tid;
    /*!
      here we store the time interval, whilst the timer run.
      This is waste!!!
    */
    unsigned int timer_secs;
    
public:
    /*!
      The constructor creates an timer object with the given
      properties. The timer at this moment is not started. This
      will be done with the start() member function.
      \param msec time interval after that the the variable
      pointed by exitflag is setting to one.
      \param exitflag the adress of an integer, which was set
      to one after the given time interval.
      \warning The integer variable shouldn't leave it's valid
      range, before the timer was finished. So never take a
      local variable.
      \param exitfnc A function, which was called after msec.
      If you don't want this, refer a NULL pointer.
    */
    timer(unsigned int msec, int *exitflag, void *(*exitfnc)(void *));
    /*!
      the destructor. If his was called (for example by leaving the
      valid range of the timer object), the timer thread automaticaly
      will finished. The exitflag wouldn't be set, also the exitfnc
      wouldn't be called.
    */
    ~timer();
    /*!
      starts the timer. But now a thread will created and started.
      After this, the timer thread will be running until he was stopped 
      by calling stop() or reached his given time interval.
    */
    int start();
    /*!
      stops the timer and canceled the timer thread. After timer::stop() a new
      start() will started the timer from beginning.
    */
    int stop();
};

/*!
  \brief sleepms
  A plattform independent function, to go to sleep for the given
  time interval.
  \param ms time interval in milli seconds
*/
void sleepms(unsigned int ms);

#endif

#endif
// __TIMER_BASE_H

