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

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include "compiler.h"

#ifdef HAS_DOS_H
#include <dos.h>
#endif

typedef unsigned bit;

#ifndef SASC
typedef unsigned char byte;
#endif

typedef signed char sbyte;

typedef unsigned short word;
typedef signed short sword;

#ifdef __alpha    /* add other 64 bit systems here */
typedef unsigned int dword;
typedef signed   int sdword;
#else             /* 32 and 16 bit machines */
typedef unsigned long dword;
typedef signed long sdword;
#endif

#if !defined(__UNIX__) && !defined(SASC) && !defined(__MINGW32__)
typedef unsigned short ushort;
#endif

typedef signed short sshort;

#ifndef __UNIX__
typedef unsigned long ulong;
#endif

typedef signed long slong;

#endif

/*--- why didn't k&r put these into stdio.h? ---*/
#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif

