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
#include "huskyext.h"
#include "calendar.h"
#include "crc.h"
#include "cvtdate.h"
#include "dirlayer.h"
#include "fexist.h"
#include "ffind.h"
#include "locking.h"
#include "log.h"
#include "memory.h"
#include "parsenn.h"
#include "recode.h"
#include "strext.h"
#include "temp.h"
#include "tree.h"
#include "unused.h"
#include "xstr.h"


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

/* getfree.c */
/* Check free disk space */
HUSKYEXT ULONG fc_GetDiskFreeSpace(const char *path);

/* ioutil.c */
HUSKYEXT UINT16 getUINT16(FILE *in);
/*DOC
  Input:  in is an file stream opened for reading.
  Output: getUINT16 returns an UINT16
  FZ:     the UINT15 is read from the stream using the method lowByte, highByte.
*/

HUSKYEXT int    fputUINT16(FILE *out, UINT16 word);
/*DOC
  Input:  out is an file opened for writing.
          word is the UINT16 which should be written
  Output: fputUIN16 returns the return of the second fputc call.
  FZ:     fputUINT16 writes word into the stream using the order lowByte, highByte.
*/

HUSKYEXT INT    fgetsUntil0(UCHAR *str, size_t n, FILE *f, char *filter);
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
#  define cmdcall(cmd) system(cmd)
#endif

/* Converts decimal value to octal [useful for chmod()] */
unsigned int dec2oct(unsigned int decimal);

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


/* patmat.c */
HUSKYEXT int patmat( const char *raw, const char *pat );
HUSKYEXT int patimat(char *raw, char *pat );


#endif /*__HUSKYLIB_H__ */
