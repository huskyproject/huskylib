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

/* SAS C for AmigaDOS defines ************************************/
#ifndef HUSKY_SASC_H
#define HUSKY_SASC_H

#define _stdc
#define _intr
#define _intcast
#define _veccast
#define _fast
#define _loadds

#define cdecl
#define pascal
#define near
#undef  far
#define far

#define farread read
#define farwrite write
#define mymkdir(a) mkdir((a))

#define unlock(a,b,c) unused(a)
#define lock(a,b,c) 0
#define mysleep(x) unused(x)

#include <fcntlh.>
#ifndef O_BINARY
# define O_BINARY 0 /* O_BINARY flag has no effect under UNIX */
#endif

#error "Don't know how to implement record locking."
/* Using an executable that does no support record locking is
   discouraged in a multitasking environment. If you want to
   do it anyway, you may uncomment this line. Record locking is used
   to obtain a lock on the very first byte of a SQD file which
   indicates that no other program should use the message area now.
*/

#define SH_DENYNONE 0
/*#define sopen(a,b,c,d) open((a),(b),(d))*/

#define _XPENTRY

typedef unsigned bit;

/*typedef unsigned char byte;*/
typedef signed char sbyte;

typedef unsigned short word;
typedef signed short sword;

typedef unsigned long dword;
typedef signed long sdword;

typedef signed short sshort;
/*typedef unsigned short ushort;*/

typedef signed long slong;
typedef unsigned long ulong;

#endif
