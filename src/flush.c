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

#include <stdio.h>
#include <stdlib.h>

#include "../huskylib/compiler.h"


#if !defined(__UNIX__) && !defined(SASC)
#include <io.h>
#endif

#if defined(__UNIX__) || defined(__DJGPP__)
#include <unistd.h>
#endif

#if !defined(__IBMC__) && !defined(__DOS__) && !defined(__UNIX__) && !defined(__MINGW32__) && !defined(__MSVC__)
#include <dos.h>
#endif

#ifdef __OS2__

#  define INCL_NOPM
#  include <os2.h>

#  if defined(__WATCOMC__)
#    ifndef DosBufReset
#      define DosBufReset DosResetBuffer
#    endif
#  elif defined(__EMX__) || defined(__FLAT__)
#    undef DosBufReset
#    define DosBufReset DosResetBuffer
#  endif
#endif

#if defined(__NT__) || defined(__MINGW32__)
#  define WIN32_LEAN_AND_MEAN
#  define NOGDI
#  define NOUSER
#  define NOMSG
#  include <windows.h>
#endif

#if defined(__DJGPP__)
/*
void pascal far flush_handle2(int fh)
{
    fsync(fh);
}
*/
#define flush_handle2(f)  fsync(f)

#elif defined(__UNIX__) || defined(SASC)
/*
void pascal far flush_handle2(int fh)
{
    unused(fh);
}
*/
#define flush_handle2(f)  unused(f)


#elif defined(__OS2__)
/*
void pascal far flush_handle2(int fh)
{
    DosBufReset((HFILE) fh);
}
*/
#define flush_handle2(f)  DosBufReset((HFILE) f)

#elif defined(__NT__) || defined(__MINGW32__)

#ifdef __RSXNT__
#  include <emx/syscalls.h>

#  ifndef F_GETOSFD
#    define F_GETOSFD 6
#  endif

#  define flush_handle2(fh)  FlushFileBuffers((HANDLE) __fcntl((fh), F_GETOSFD, 0))

#else

#  define flush_handle2(fh)  FlushFileBuffers((HANDLE) (fh))

#endif

/*
void pascal far flush_handle2(int fh)
{
#ifdef __RSXNT__
    int nt_handle = __fcntl(fh, F_GETOSFD, 0);
#else
    int nt_handle = fh;
#endif
    FlushFileBuffers((HANDLE) nt_handle);
}
*/

#elif defined(__WATCOMC__DOS__)

#include <dos.h>
/*
void pascal far flush_handle2(int fh)
{
    _dos_commit(fh);
}
*/
#  define flush_handle2(fh)  _dos_commit(fh)

#elif defined (__DOS__)

/* refer to MS-DOS flush_handle2 code in FLUSHASM.ASM */
void pascal far flush_handle2(int fd);

#else

#error unknown compiler

#endif

/*
 *  This makes sure a file gets flushed to disk.  Thanks to Ray Duncan
 *  for this tip in his _Advanced MS-DOS_ book.
 */

void _fast flush_handle(FILE * fp)
{
    fflush(fp);

#if defined(__OS2__) || defined(__DOS__) || defined(__NT__) || defined(__TURBOC__) || defined(SASC) || defined(__DJGPP__)
    flush_handle2(fileno(fp));
#else
    {
        int nfd;

        nfd = dup(fileno(fp));
        if (nfd != -1)
        {
            close(nfd);
        }
    }
#endif
}
