/* $Id$
 *  Provides compiler-independent functions to read directory contents
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

/* standard headers */
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/* huskylib: compiler.h */
#include <compiler.h>


/* compiler-dependent headers */
#ifdef HAS_IO_H
    #include <io.h>
#endif

#ifdef HAS_DIR_H
    #include <dir.h>
#endif

#ifdef HAS_DIRENT_H
    #include <dirent.h>
#endif

#ifdef HAS_DIRECT_H
    #include <direct.h>
#endif


/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

/* huskylib headers */
#include <dirlayer.h>
#include <memory.h>
#include <strext.h>
#ifndef HAS_DIRENT_H
    #include <ffind.h>
#endif
#include <fexist.h>

/***  Implementation  *******************************************************/

#ifdef HAS_DIRENT_H
/* use compiler or system library: opendir/readdir/closedir */

husky_DIR* husky_opendir(const char* mask)
{
    husky_DIR dir;

    if(!mask) return NULL;
    memset(&dir,0,sizeof(dir));

    strncpy( dir.d_mask, mask, sizeof(dir.d_mask)-3 ); /* preserve two chars for "\\*"
                                                d_mask filled by zero in previous source line */
#if 0
    {
        char *ch, *ch2;
        ch = strrchr(d_mask,'/');
        ch2 = strrchr(d_mask,'\\');
        ch = max(ch,ch2);
        if (ch && ch[1]=='\0') strcat(dir->d_mask,"*");
        else strcat(dir->d_mask,"\\*");
    }
#endif
    dir.internal_DIR = opendir(mask);
    if(!dir.internal_DIR) return NULL;

    return memdup(&dir,sizeof(dir));
}

char* husky_readdir(husky_DIR* dir)
{
    struct dirent *de;
    if(!dir || !dir->internal_DIR) return 0;

    do
    {
        de = readdir(dir->internal_DIR);
        if(!de || !de->d_name) return NULL;
    }
    while( strcmp(de->d_name,".")==0 || strcmp(de->d_name,"..")==0 );
    strnzcpy(dir->d_name, de->d_name, sizeof(dir->d_name));
    dir->d_attr = 0;       /* Use stat() for this! */
    dir->d_size = -1;

    return dir->d_name;
}

int  husky_closedir(husky_DIR* dir)
{
    int rc;

    if(!dir || !dir->internal_DIR) return 0;

    rc = closedir(dir->internal_DIR);
    if(!rc)
    {
        free(dir);
    }

    return rc;
}

void  husky_rewinddir(husky_DIR* dir)
{
    if(!dir || !dir->internal_DIR) return;

    rewinddir(dir->internal_DIR);
    dir->d_name[0] = '\0';
}

#else  /* Use ffind.c */

husky_DIR* husky_opendir(const char* mask)
{
    husky_DIR dir;

    if(!mask) return NULL;

    if(!direxist(mask))
    {
        errno = ENOENT;
        return NULL;
    }
    memset(&dir,0,sizeof(dir));

    strncpy( dir.d_mask, mask, sizeof(dir.d_mask)-3 ); /* preserve two chars for "\\*"
                                                d_mask filled by zero in previous source line */
    {
        char *ch, *ch2;
        ch = strrchr(dir.d_mask,'/');
        ch2 = strrchr(dir.d_mask,'\\');
        ch = max(ch,ch2);
        if (ch && ch[1]=='\0') strcat(dir.d_mask,"*");
        else strcat(dir.d_mask,"\\*");
    }
    dir.d_first++;
    return memdup(&dir,sizeof(dir));
}

/* Note: FFindInfo/FFindNext skips "." and ".." entries */
char* husky_readdir(husky_DIR* dir)
{
    if(!dir) return 0;

    dir->d_name[0] = '\0';
    dir->d_attr = 0;
    dir->d_size = 0;

    if(dir->d_first)
    {
        dir->ff = FFindInfo(dir->d_mask);
        if((dir->ff)==NULL) return NULL;
        dir->d_first=0;
    }
    else
    {
        if(FFindNext(dir->ff)) return NULL;
    }

    strnzcpy(dir->d_name, dir->ff->ff_name, sizeof(dir->d_name));
    dir->d_attr = dir->ff->ff_attrib;
    dir->d_size = dir->ff->ff_fsize;

    return dir->d_name;
}


int  husky_closedir(husky_DIR* dir)
{
    if(!dir) return -1;

    FFindClose(dir->ff);
    free(dir);

    return 0;
}


void  husky_rewinddir(husky_DIR* dir)
{
    if(!dir) return;

    if(dir->ff) FFindClose(dir->ff);
    dir->ff = NULL;
    dir->d_first = 1;
    dir->d_name[0] = '\0';
    dir->d_attr = 0;
    dir->d_size = 0;
}

#endif

#ifdef TEST

int main(int argc, char **argv)
{
    husky_DIR *dd;
    char *name=".";

    if(argc>1) name = argv[1];

    dd=husky_opendir(name);
    while( (name=husky_readdir(dd)) )
        printf("%s\n",name);

    husky_closedir(dd);
    return 0;
}
#endif
