/* $Id$
 *  Provides compiler-independent sleep() implementation with precisious
 *  to milliseconds
 *
 *  Latest version may be foind on http://husky.sourceforge.net
 *
 *  Written by Scott Dudley
 *  777 Downing St.    FidoNet     1:249/106
 *  Kingston, Ont.     Internet    sjd@f106.n249.z1.fidonet.org
 *  Canada  K7M 5N3    BBS         1-613-634-3058, V.32bis
 *
 *  Copyright 1989-1994 by SCI Communications.
 *  Copyright 1997-2003 Husky Developers Team.
 *
 * HUSKYLIB: common defines, types and functions for HUSKY
 *
 * This is part of The HUSKY Fidonet Software project:
 * see http://husky.sourceforge.net for details
 *
 *
 * HUSKYLIB is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * HUSKYLIB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see file COPYING. If not, write to the
 * Free Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * See also http://www.gnu.org, license may be found here.
 */

/* standard headers */
#include <stdlib.h>
#include <time.h>


/* huskylib: compiler.h */
#include <compiler.h>


/* compiler-dependent headers */
#ifdef HAS_UNISTD_H
#  include <unistd.h>
#endif

#if defined(__DOS__) || defined(__DPMI__)
#include <dos.h>
#endif


/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

/* huskylib headers */
#include <huskylib.h>


/***  Declarations & defines  ***********************************************/

HUSKYEXT void _fast tdelay(int);

/***  Implementation  *******************************************************/

#if defined(__OS2__)
#define INCL_NOPM
#define INCL_DOS

#include <os2.h>

  void _fast tdelay(int msecs)
  {
      DosSleep((ULONG)msecs);   /*ULONG defined in os2.h*/
  }

#elif defined(__DOS__)

  void _fast tdelay(int msecs)
  {
    clock_t ctEnd;

    ctEnd = clock() + (long)msecs * (long)CLK_TCK / 1000L;

    while (clock() < ctEnd)
      ;
  }

#elif defined(__MINGW32__)

  void _fast tdelay(int msecs)
  {
    _sleep((dword)msecs);
  }

#elif defined(__WIN32__)
/* win32/nt not mingw or `cygwin -mno-cygwin`  (MS VC, Borland C/win32, Watcom C)*/

#include <windows.h>
/*# if defined(__TURBOC__)
   extern void __stdcall Sleep(dword ms);
# else
   extern void Sleep(dword ms);
# endif*/
   void _fast tdelay(int msecs)
   {
    Sleep((dword)msecs);
   }

#elif defined(__BEOS__)

#include <be/kernel/scheduler.h>

  void _fast tdelay(int msecs)
  {
    snooze(msecs*1000l);
  }

#elif defined(__UNIX__)

  void _fast tdelay(int msecs)
  {
    usleep(msecs*1000l);
  }

#elif defined(__WATCOMC__)
  void _fast tdelay(int msecs)
  {
    sleep(msecs);
  }
#else
#error Unknown OS
#endif
