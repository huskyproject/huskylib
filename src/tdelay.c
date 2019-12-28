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

#if !defined(_MSC_VER)
    #include <sys/time.h>
#endif

/* huskylib: compiler.h */
#include <compiler.h>


/* compiler-dependent headers */
#ifdef HAS_UNISTD_H
    #include <unistd.h>
#endif

#if defined(__DOS__) || defined(__DPMI__)
    #include <dos.h>
#endif


/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

/* huskylib headers */
#include <huskylib.h>


#if defined(HAS_sleep)
void _fast tdelay(int msecs)
{
    mysleep(msecs);
}
#else
# error "'sleep' not defined!"
#endif


#if defined(__WIN32__) || defined(__MINGW32__)
void husky_SetTimer(hs_time *timer_ctx)
{
    dword now;
    now = GetTickCount();
    timer_ctx->sec = now / 1000;
    timer_ctx->msec = now % 1000;
}

dword husky_GetTimer(hs_time *timer_ctx)
{
    dword now;
    dword diff;
    now = GetTickCount();
    diff = (((now/1000) - timer_ctx->sec) * 1000) + ((now % 1000) - timer_ctx->msec);
    return diff;
}

#elif defined (__UNIX__) || defined(__BEOS__) || defined(__DJGPP__) || defined(__CYGWIN__) || defined(__EMX__)
void husky_SetTimer(hs_time *timer_ctx)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    timer_ctx->sec = now.tv_sec;
    timer_ctx->msec = now.tv_usec / 1000;
}

dword husky_GetTimer(hs_time *timer_ctx)
{
    struct timeval now;
    dword diff;
    gettimeofday(&now, NULL);
    diff = ((now.tv_sec - timer_ctx->sec) * 1000) + ((now.tv_usec / 1000) - timer_ctx->msec);
    return diff;
}

#else
# error Unknown OS (husky_*Timer)
#endif
