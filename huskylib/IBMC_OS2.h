/* $Id$
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

/* IBM C/Set++ for OS/2 */

#ifndef HUSKY_IBMC_OS2_H
#define HUSKY_IBMC_OS2_H

#  define _stdc
#  define _intr
#  define _intcast
#  define _veccast
#  define _fast
#  define _loadds

#  define cdecl
#  define pascal
#  define near
#  undef  far
#  define far

#  define farread read
#  define farwrite write
#  define mysleep(x) DosSleep(1000L*(x))
#  define sleep(x)   DosSleep(1000L*(x))
#  define HAS_sleep     1

#  define _XPENTRY pascal far

#  define HAS_MALLOC_H        /* use "#include <malloc.h>" for malloc() etc. */
#  define HAS_IO_H     1  /* may use "#include <io.h> */
#  define HAS_SHARE_H  1  /* may use "#include <share.h> */
#  define HAS_DIRECT_H 1  /* may use "#include <direct.h> */
#  define HAS_PROCESS_H   /* may use "#include <process.h> */


typedef unsigned bit;

typedef unsigned char byte;
typedef signed char sbyte;

typedef unsigned short word;
typedef signed short sword;

typedef unsigned long dword;
typedef signed long sdword;

typedef signed short sshort;
typedef unsigned short ushort;

typedef signed long slong;
typedef unsigned long ulong;

#endif
