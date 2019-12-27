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

#ifndef HUSKY_HUSKYLIB_H__
#define HUSKY_HUSKYLIB_H__

/* standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

/* huskylib headers */
#include "compiler.h"
#include "huskyext.h"

#ifdef __MSVC__
    #include <crtdbg.h>
#endif
#include "calendar.h"
#include "crc.h"
#include "cvtdate.h"
/*#include "dirlayer.h" Don't include this file to prevent conflicts, please!!! */
#include "fexist.h"
#include "ffind.h"
#include "ftnaddr.h"
#include "locking.h"
#include "log.h"
#include "memory.h"
#include "recode.h"
#include "strext.h"
#include "temp.h"
#include "tree.h"
#include "unused.h"
#include "xstr.h"

#ifndef HUSKYLIB_VER_MAJOR
    #define HUSKYLIB_VER_MAJOR  1
#endif
#ifndef HUSKYLIB_VER_MINOR
    #define HUSKYLIB_VER_MINOR  9
#endif
#ifndef HUSKYLIB_VER_PATCH
    #define HUSKYLIB_VER_PATCH  0
#endif
#ifndef HUSKYLIB_VER_BRANCH
    #define HUSKYLIB_VER_BRANCH BRANCH_CURRENT
#endif

/* values for 5th parameter of GenVersionStr() */
typedef enum
{
    BRANCH_CURRENT=1, BRANCH_STABLE=2, BRANCH_RELEASE=3
} branch_t;

/*-- flush.c --*/

/* fflush() analog with file hanle as argument */
#ifdef __DOS__
    /* flushasm.asm for DOS, redefined for known implementations in flush.c */
    HUSKYEXT void pascal far flush_handle2(int fh);
#endif

/* Compiler-independent fflush() implementation */
HUSKYEXT void _fast flush_handle(FILE * fp);


/*-- adcase.c -------------------------------------------------------------*/

/*  ATTENTION: The adaptcase routine builds an internal cache which never
 *  expires. If you ever add files to or remove files to a subdirectory and
 *  later want to have adaptcase map this particular file name properly,
 *  you must call adaptcase_refresh_dir() with the subdirectory path name
 *  as argument!
 */

HUSKYEXT void adaptcase_refresh_dir(const char *directory);
HUSKYEXT void adaptcase(char *);


/*-- tdelay.c -------------------------------------------------------------*/
/* Compiler-independent sleep() implementation with precisious to milliseconds */
HUSKYEXT void _fast tdelay(int);

typedef struct
{
    unsigned long sec;   /* seconds */
    unsigned long msec;  /* milliseconds */
} hs_time;

/* Get snapshot of current time with precision up to milliseconds */
HUSKYEXT void husky_SetTimer(hs_time *timer_ctx);

/* Return difference between now and previous time snapshot */
HUSKYEXT dword husky_GetTimer(hs_time *timer_ctx);


/*-- genmsgid.c --*/
HUSKYEXT dword _XPENTRY GenMsgId(char *seqdir, unsigned long max_outrun);
HUSKYEXT dword _XPENTRY GenMsgIdEx(char *seqdir, unsigned long max_outrun,
                                   dword (*altGenMsgId)(void), char **errstr);

/*-- setfsize.c --*/
HUSKYEXT int _fast setfsize(int fd, long size);

/*-- mapfile.c --*/
/* Mapping file to memory implementation for several OS. */
HUSKYEXT void* MapFile(char* fname);

/* getfree.c */
/* Check free disk space */
/* named husky_GetDiskFreeSpace() to avoid conflict with windows's GetDiskFreeSpace() */
HUSKYEXT unsigned long husky_GetDiskFreeSpace(const char *path);
#define fc_GetDiskFreeSpace(p) husky_GetDiskFreeSpace(p)  /*for compatibility with old code; to be removed */

/* ioutil.c */
HUSKYEXT word getUINT16(FILE *in);
/*DOC
  Input:  in is an file stream opened for reading.
  Output: getUINT16 returns an word
  FZ:     the UINT15 is read from the stream using the method lowByte, highByte.
*/

HUSKYEXT int    fputUINT16(FILE *out, word word);
/*DOC
  Input:  out is an file opened for writing.
          word is the word which should be written
  Output: fputUIN16 returns the return of the second fputc call.
  FZ:     fputUINT16 writes word into the stream using the order lowByte, highByte.
*/

HUSKYEXT signed int fgetsUntil0(UCHAR *str, size_t n, FILE *f, char *filter);
/*DOC
  Input:  n-1 chars are read at most.
          str is a buffer with the length n.
          f is a file stream opened for reading.
		  filter is a string with characters that shoudn't be reading
  Output: fgetsUntil0 returns the number of chars read including the last \0
  FZ:     fgetsUntil0 reads chars into the buffer until eof(f) || n-1 are read || a \0 is encountered.
*/

HUSKYEXT char   *shell_expand(char *str);
/*DOC
   Input: str is a \0 terminated string which must have been malloc'ed
   Ouput: a pointer to a \0 terminated string is returned which must be free'd
   FZ:    shell_expand expands the strings just like ~/.msged to /home/mtt/.msged
          see sh(1) for further explanations
   Note:  *str re-allocated if need
*/

/* will be moved to huskylib */
HUSKYEXT int move_file(const char *from, const char *to, const int force_rewrite);
/* DOC
   Input:  source and destination filename
   Output: 0 if OK, != 0 and errno set on error
   FZ:     Move a file, works even over file system boundaries,
   replace file if the destination file already exists and force_rewrite !=0
*/

/* will be moved to huskylib */
HUSKYEXT int copy_file(const char *from, const char *to, const int force_rewrite);
/* DOC
 * copy file into other location
 * rewrite existing file if third parameter not zero
 * return 0 if success; else return -1 and set errno
 */

/* cmdcall()
 * Call external command (using spawnvp() if possible to prevent drop command
 * exit code by buggy shell, e.g. command.com)
 * Return exit code of the executed command.
 */
#if defined(HAS_spawnvp) && ( defined(__DOS__) || defined(__WIN32__) )
    HUSKYEXT int cmdcall(const char *cmd);
#else
    #define cmdcall(cmd) system(cmd)
#endif

/* Converts decimal value to octal [useful for chmod()] */
HUSKYEXT unsigned int dec2oct(unsigned int decimal);

/* try to create and lock lockfile */
/* returns -1 if fail */
/* returns filedescriptor of lockfile if success */
HUSKYEXT int lockFile(const char *lockfile, int advisoryLock);

#ifndef createLockFile
    #define createLockFile(x) lockFile(x, 0)
#endif

/* close and remove lockfile */
HUSKYEXT int FreelockFile(const char *lockfile, int fh);

/* will be moved to huskylib */
HUSKYEXT  char    *GetFilenameFromPathname(const char* pathname);
/*   Get the object name from the end of a full or partial pathname.
    The GetFilenameFromPathname function gets the file (or directory) name
    from the end of a full or partial pathname. Returns The file (or directory)
    name
*/

/* will be moved to huskylib */
#define basename(f) GetFilenameFromPathname(f)

/* will be moved to huskylib */
/*  Get the object name from the end of a full or partial pathname (OS-independed).
    This function gets the file (or directory) name from the end of a full
    or partial pathname for any path style: UNIX, DOS or mixed (mixed style
    may be used in Windows NT OS family).
    Returns the file (or directory) name: pointer to part of all original pathname.
*/
HUSKYEXT char *OS_independed_basename(const char *pathname);

/* will be moved to huskylib */
/* Return directory part of pathname (without filename, '/' or '\\' present at end)
 * Return value is pointer to malloc'ed string;
 * if pathname is filenfme without directory return current directory (./ or .\)
 */
HUSKYEXT char    *GetDirnameFromPathname(const char* pathname);

#define dirname(f) GetDirnameFromPathname(f)

/* will be moved to huskylib */
HUSKYEXT void fillCmdStatement(char *cmd, const char *call, const char *archive, const char *file, const char *path);

/*-- patmat.c --*/
/*HUSKYEXT int patmat( const char *raw, const char *pat );
HUSKYEXT int patimat(char *raw, char *pat );*/
HUSKYEXT int xpatmat(const char *raw, const char *pat, const int ncase);
#define patmat(raw, pat)  xpatmat(raw, pat, 0)
#define patimat(raw, pat) xpatmat(raw, pat, 1)

/*-- qksort.c --*/
/* quick sort integer array */
HUSKYEXT void _fast qksort(int a[], size_t n);

/*-- cmpfname.c --*/
/* compare file names on all known file systems */
HUSKYEXT int cmpfnames(const char *file1, const char *file2);


/*-- genverst.c --*/

/* Generate version string like
 * programname/platform[-compiler] <major>.<minor>.<patchlevel>-<branch> [<cvs date>]
 *
 * Return malloc'ed pointer
 *
 * Examples:
 * "program/w32-MVC 1.2.3-release"
 * "program/DPMI-DJGPP 1.2.3-stable 01-10-2002"
 * "program/FreeBSD 1.3.0-current 01-10-2002"
 *
 * Require cvs_date.h in module hearer files directory
 */

HUSKYEXT char *GenVersionStr( const char *programname, unsigned major,
                              unsigned minor, unsigned patchlevel, unsigned branch, const char *cvsdate );


/*-- version.c --*/

/* Check version of huskylib library
 * return zero if test failed; non-zero if passed
 * test cvs need for DLL version only, using #include <fidoconf/cvsdate.h>
 */
HUSKYEXT int CheckHuskylibVersion( int need_major, int need_minor,
                                   int need_patch, branch_t need_branch, const char *cvs );

#endif /*__HUSKYLIB_H__ */
