/* $Id$
 *  Provides compiler-independent functions like DOS findfirst() and findnext()
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

#ifndef HUSKY_FFIND_H__
#define HUSKY_FFIND_H__

#include <stdio.h>

#include "compiler.h"  /* compiler see directory of this (ffind.h) file */
#include "huskyext.h"


#ifdef HAS_DOS_H
    #include <dos.h>
#endif
#if defined(HAS_UNISTD_H)
    #include <unistd.h>
#endif
#if defined(HAS_DIRENT_H)
    #include <dirent.h>
#endif
#if defined(HAS_DIR_H)
    #include <dir.h>
#endif


#if defined(__RSXNT__) || defined(__MINGW32__) || defined(__MSVC__)
    #define WIN32_LEAN_AND_MEAN
    #define NOGDI
    #define NOUSER
    #define NOMSG

    #ifdef __RSXNT__
        /* The RSXNT winsock.h conflicts with EMX
        io.h. As we do not need sockets anyway, we
        just prevent their inclusion. */
        #define _WINSOCK_H
    #endif
    #if defined(__MINGW32__)
        /* For HTick compatibility */
        #define _WINUSER_H
    #endif
    #include <windows.h>
#endif


struct ffind
{
    /* this is the public area of the struct */
    char ff_attrib;
    unsigned short ff_ftime;
    unsigned short ff_fdate;
    long ff_fsize;
    char ff_name[256];

    /* now comes the privat area where search handles or similiar are stored */

#if defined(__TURBOC__) || defined(__DJGPP__)
    struct ffblk ffbuf;

#elif defined(__WATCOMC__) || defined(__MSC__)
    struct find_t ffbuf;
    unsigned long hdir;   /* directory handle from DosFindFirst */

#elif defined(__OS2__) || defined(__EMX__)
#if defined(__386__) || defined(__FLAT__)
    unsigned long hdir;   /* directory handle from DosFindFirst */
#else
    unsigned short hdir;  /* directory handle from DosFindFirst */
#endif

#elif defined(SASC)
    struct FileInfoBlock info;
    char newfile[FILENAME_MAX];
    char prefix[FILENAME_MAX];

#elif defined(__UNIX__)
    DIR *dir;
    char firstbit[FILENAME_MAX];
    char lastbit[FILENAME_MAX];

#elif defined(__RSXNT__) || defined(__MINGW32__) || defined(__MSVC__)
#ifndef _A_HIDDEN
#   define _A_HIDDEN       0x02
#endif
    WIN32_FIND_DATA InfoBuf;
    HANDLE hDirA;
    char attrib_srch;

#else
#error Unable to determine compiler and target operating system!
#endif
};

typedef struct ffind FFIND;

/*
 * I prefixed the functions below with an additional F in order to
 * prevent name clashes with the Win32 API
 */

HUSKYEXT FFIND *_fast FFindOpen(const char *filespec, unsigned short attribute);
HUSKYEXT FFIND *_fast FFindInfo(const char *filespec);
HUSKYEXT int _fast FFindNext(FFIND * ff);
HUSKYEXT void _fast FFindClose(FFIND * ff);

#define MSDOS_READONLY  0x01
#define MSDOS_HIDDEN    0x02
#define MSDOS_SYSTEM    0x04
#define MSDOS_VOLUME    0x08
#define MSDOS_SUBDIR    0x10
#define MSDOS_ARCHIVE   0x20
#define MSDOS_RSVD1     0x40
#define MSDOS_RSVD2     0x80

#endif
