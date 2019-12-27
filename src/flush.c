/* $Id$
 *  Provide compiler-independent functions to file buffers flush.
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
#include <stdio.h>
#include <stdlib.h>

/* huskylib: compiler.h */
#include <compiler.h>

/* standard headers */
#if defined(HAS_IO_H)
    #include <io.h>
#endif

#if defined(HAS_UNISTD_H)
    #include <unistd.h>
#endif

#if defined(HAS_DOS_H)
    #include <dos.h>
#endif


/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>


#ifdef __OS2__

    #define INCL_NOPM
    #include <os2.h>

    #if defined(__WATCOMC__)
        #ifndef DosBufReset
            #define DosBufReset DosResetBuffer
        #endif
    #elif defined(__EMX__) || defined(__FLAT__)
        #undef DosBufReset
        #define DosBufReset DosResetBuffer
    #endif
#endif

#if defined(__NT__) || defined(__MINGW32__)
    #define WIN32_LEAN_AND_MEAN
    #define NOGDI
    #define NOUSER
    #define NOMSG
    #include <windows.h>
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

#elif defined(__WIN32__)

#ifdef __RSXNT__
    #include <emx/syscalls.h>

    #ifndef F_GETOSFD
        #define F_GETOSFD 6
    #endif

    #define flush_handle2(fh)  FlushFileBuffers((HANDLE) __fcntl((fh), F_GETOSFD, 0))

#else

    #define flush_handle2(fh)  FlushFileBuffers((HANDLE) (fh))

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

#elif defined(__WATCOMC__DOS__) || defined(__MSC__)

#include <dos.h>
/*
void pascal far flush_handle2(int fh)
{
    _dos_commit(fh);
}
*/
#  define flush_handle2(fh)  _dos_commit(fh)

#elif defined (__DOS__)

void pascal far flush_handle2(int fd)
{
    union REGS in, out;

    in.h.ah=0x45;
#if defined(__DPMI__) && !defined(__DJGPP__)
    in.x.ebx=fd;
    int386(0x21, &in, &out);
#else  /* #elif defined(__DOS16__) || defined(__DJGPP__) */
    in.x.bx=fd;
    int86(0x21,&in,&out);
#endif

    if(out.h.cflag) return;

    in.h.ah=0x3e;
#if defined(__DPMI__) && !defined(__DJGPP__)
    in.x.ebx=out.x.eax;
    int386(0x21, &in, &out);
#else  /* #elif defined(__DOS16__) || defined(__DJGPP__) */
    in.x.bx=out.x.ax;
    int86(0x21,&in,&out);
#endif
}

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
