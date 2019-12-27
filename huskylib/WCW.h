/* $Id$
 *  Watcom C for Win32 (NT) compiler-specific declarations
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

/* WATCOM C/C++ for Windows NT */

#ifndef HUSKY_WCW_H
#define HUSKY_WCW_H

#ifndef __WATCOMC__
    #error This file may be used only with Watcom C !
#endif
#if !defined(__WIN32__) && !defined(__NT__)
    #error This file may be used only with Watcom C Win32/NT target !
#endif


#    define _stdc
#    define _intr
#    define _intcast
#    define _veccast
#    define _fast
#    define farread  read
#    define farwrite write

#    define _XPENTRY pascal

/*#  define mysleep(x) Sleep(x*1000) */ /* winbase.h */

#    include <limits.h>
#ifndef MAXPATHLEN
    #define MAXPATHLEN NAME_MAX
#endif

#  define HAS_sopen     3   /* sopen() : in io.h, 3 or 4 parameters */

#endif
