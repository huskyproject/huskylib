/* $Id$
 *  Provides compiler-independent types and functions to read directory contents
 *
 *  Copyright (C) 1998-1999
 *
 *  Matthias Tichy
 *
 *  Fido:     2:2433/1245 2:2433/1247 2:2432/605.14
 *  Internet: mtt@tichy.de
 *
 *  Grimmestr. 12         Buchholzer Weg 4
 *  33098 Paderborn       40472 Duesseldorf
 *  Germany               Germany
 *
 *  Latest version may be foind on http://husky.sourceforge.net
 *
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

#ifndef HUSKY_DIRLAYER_H
#define HUSKY_DIRLAYER_H

/* standard headers */
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


/* huskylib: compiler.h */
#include "compiler.h"


/* compiler-dependent headers */
#ifdef HAS_DIRENT_H
    #include <dirent.h>
#endif


/* huskylib headers */
#include "huskyext.h"
#include "ffind.h"


/***  Declarations & defines  ***********************************************/


#ifdef __MSVC__
    #define NAME_MAX        _MAX_PATH

#endif

#if defined(__IBMC__) && !defined(__UNIX__)   /* only define it for IBM VisualAge C++ */

    #define INCL_DOSERRORS
    #define INCL_DOSFILEMGR
    #include <os2.h>

    #define NAME_MAX        255             /* maximum filename */

    /* File attribute constants for d_attr field */

    #define _A_NORMAL       0x00    /* Normal file - read/write permitted */
    #define _A_RDONLY       0x01    /* Read-only file */
    #define _A_HIDDEN       0x02    /* Hidden file */
    #define _A_SYSTEM       0x04    /* System file */
    #define _A_VOLID        0x08    /* Volume-ID entry */
    #define _A_SUBDIR       0x10    /* Subdirectory */
    #define _A_ARCH         0x20    /* Archive file */

#endif

#ifndef NAME_MAX
    #define NAME_MAX        128     /* maximum filename */
#endif

typedef struct husky_dirent
{
    char   d_attr;          /* file's attribute (DOS-based OS) */
    /*  unsigned short int d_time;     */     /* file's time */
    /*  unsigned short int d_date;     */     /* file's date */
    long   d_size;              /* file's size */
    char   d_name[NAME_MAX+1];  /* file's name */
    char   d_mask[NAME_MAX+1];  /* file's search mask */
#ifdef HAS_DIRENT_H
    DIR   *internal_DIR;       /* system/compiler DIR structure */
#else
    FFIND *ff;       /* for FFindOpen()/FFindInfo()/FFindNext()/FFindClose() */
    int    d_first;  /* flag for 1st time (set by husky_opendir(), reset by husky_readdir() */
#endif
} husky_DIR;

/* The husky_opendir() function opens the directory named by filename,
 * associates a directory stream with it and returns a pointer to be used
 * to identify the directory stream in subsequent operations.  The pointer
 * NULL is returned if filename cannot be accessed, or if it cannot malloc(3)
 * enough memory to hold the whole thing.
 */
HUSKYEXT husky_DIR* husky_opendir(const char*);

/* The husky_readdir() function returns a pointer to the next file name in
 * directory. husky_readdir() skips current and parent directory entries
 * ("." & "..") for miltiplatform compatibility.
 * It returns NULL upon reaching the end of the directory or detecting an
 * invalid seekdir() operation.
 */
HUSKYEXT char* husky_readdir(husky_DIR*);

/* The husky_closedir() function closes the named directory stream and frees
 * the structure associated with the dirp pointer, returning 0 on success.  On
 * failure, -1 is returned and the global variable errno is set to indicate
 * the error.
 */
HUSKYEXT int  husky_closedir(husky_DIR*);

/* The husky_rewinddir() function resets the position of the named directory
 * stream to the beginning of the directory.
 */
HUSKYEXT void  husky_rewinddir(husky_DIR* dir);

#endif
