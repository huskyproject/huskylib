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

/* MinGW32 & cygwin's 'gcc -mno-cygwin' */

/* Applied to:
   - MINGW32 for 32-bit Windows NT on Intel and AXP;
   - MINGW32 cross-compiler from unixes;
   - Cygwin GCC with option -mno-cygwin.
*/

#ifndef HUSKY_MINGW32_H
#define HUSKY_MINGW32_H

#ifndef __LITTLE_ENDIAN__
#  define __LITTLE_ENDIAN__
#endif

#  define _stdc
#  define _intr
#  define _intcast
#  define _veccast
#  define _fast
#  define _loadds
#  define cdecl
#  define pascal __stdcall
#  define near
#  undef  far
#  define far
#  define _XPENTRY
#  define HUSKYEXT extern

#  define farread _read
#  define farwrite _write
#  define fdopen _fdopen
#  define close _close
#  define open _open
#  define lseek _lseek
#  define tell _tell
#  define write _write
#  define read _read

#  define sleep(sec) _sleep((sec)*1000l)
#  define mysleep(sec) _sleep((sec)*1000l)
#  define HAS_sleep     1

#  define mymkdir mkdir

/*#  define strcasecmp  stricmp*/
/*#  define strncasecmp strnicmp*/

  int unlock(int handle, long ofs, long length);
  int lock(int handle, long ofs, long length);

/* older mingw headers are too lazy ... */
#  include <share.h>
#  define sopen _sopen
#  ifndef SH_DENYRW
#    define SH_DENYRW 0x10
#  endif
#  ifndef SH_DENYWR
#    define SH_DENYWR 0x20
#  endif
#  ifndef SH_DENYRD
#    define SH_DENYRD 0x30
#  endif
#  ifndef SH_DENYNO
#    define SH_DENYNO 0x40
#  endif

#  define HAS_spawnvp    /* spawnvp() present */
#  define HAS_mktime     /* time.h */
#  define HAS_strftime   /* time.h */
#  define HAS_ACCESS    /* access() in io.h */
#  define HAS_sopen

#  define HAS_MALLOC_H   /* may use "#include <malloc.h>" for malloc() etc. */
#  define HAS_IO_H       /* may use "#include <io.h> */
#  define HAS_SHARE_H    /* may use "#include <share.h> */
#  define HAS_DIRENT_H   /* may use "#include <dirent.h> */
#  define HAS_SYS_UTIME_H  /* #include <sys/utime.h> in alternate to <utime.h> */
#  define HAS_PROCESS_H   /* may use "#include <process.h> */

#  define USE_STAT_MACROS

typedef unsigned bit;

typedef unsigned char byte;
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
