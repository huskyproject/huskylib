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

#ifndef __HUSKYLIB_H__
#define __HUSKYLIB_H__

#include <stdio.h>
#include <time.h>

#include "compiler.h"
/*
 * This is compiler-specific stuff for huskylib only:
 *
 * HUSKYEXT - external variables & external functions call modifier
 *             (usualy 'extern' for static linkage)
 */

#ifdef _MAKE_DLL
#  if defined(_MSC_VER) && (_MSC_VER >= 1200)
#      ifndef _HUSKYEXT
#          define HUSKYEXT __declspec(dllimport)
#      else
#          define HUSKYEXT __declspec(dllexport)
#      endif /* _HUSKYEXT */
#  else
#      define HUSKYEXT extern
#  endif
#else
#  define HUSKYEXT extern
#endif


#include "calendar.h"
#include "cvtdate.h"
#include "fexist.h"
#include "ffind.h"
#include "locking.h"
#include "parsenn.h"
#include "patmat.h"
#include "strext.h"
#include "unused.h"


/*-- flush.c --*/
#ifdef __DOS__
/* flushasm.asm for DOS, redefined for known implementations in flush.c */
void pascal far flush_handle2(int fd);
#endif

void _fast flush_handle(FILE * fp);


/*-- tdelay.c --*/
HUSKYEXT void _fast tdelay(int);


/*-- memory.c --*/
#ifdef __LITTLE_ENDIAN__

#define put_dword(ptr, val)	(*(dword *)(ptr) = (val))
#define put_word(ptr, val)	(*(word *)(ptr) = (val))
#define get_dword(ptr)		(*(dword *)(ptr))
#define get_word(ptr)		(*(word *)(ptr))

#else

HUSKYEXT void put_word(byte *ptr, word value);
HUSKYEXT void put_dword(byte *ptr, dword value);
/*
 *  get_dword
 *
 *  Reads in a 4 byte word that is stored in little endian (Intel) notation
 *  and converts it to the local representation n an architecture-
 *  independent manner
 */

#define get_dword(ptr)            \
   ((dword)((unsigned char)(ptr)[0]) |           \
    (((dword)((unsigned char)(ptr)[1])) << 8)  | \
    (((dword)((unsigned char)(ptr)[2])) << 16) | \
    (((dword)((unsigned char)(ptr)[3])) << 24))  \

/*
 *  get_word
 *
 *  Reads in a 2 byte word that is stored in little endian (Intel) notation
 *  and converts it to the local representation in an architecture-
 *  independent manner
 */

#define get_word(ptr)         \
    ((word)((unsigned char)(ptr)[0]) |         \
     (((word)((unsigned char)(ptr)[1])) << 8 ))

#endif /* __LITTLE_ENDIAN__ */


/*-- genmsgid.c --*/
HUSKYEXT dword _XPENTRY GenMsgId(char *seqdir, unsigned long max_outrun);  
HUSKYEXT dword _XPENTRY GenMsgIdEx(char *seqdir, unsigned long max_outrun,
			    dword (*altGenMsgId)(void), char **errstr);

/* setfsize.c */
HUSKYEXT int _fast setfsize(int fd, long size);

/* mapfile.c */
/* Mapping file to memory implementation for several OS. */
HUSKYEXT void* MapFile(char* fname);

#endif /*__HUSKYLIB_H__ */
