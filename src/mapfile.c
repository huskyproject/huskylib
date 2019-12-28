/* $Id$
 *
 * Mapping file to memory implementation for several OS.
 * Code initially written by Alexander Lelyakin 2:5020/969.115
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
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>


/* huskylib: compiler.h */
#include <compiler.h>

/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>


/***  Implementation  *******************************************************/

#if defined(__NT__) /* WIN32 version */

#ifndef __MINGW32__
    #ifndef STRICT
        #define STRICT 1
    #endif
#endif
#include <windows.h>

void*MapFile(char*fname)
{
    long len;
    HANDLE h, hm;

    h = CreateFile( fname, GENERIC_READ, FILE_SHARE_READ, NULL,
                    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if (h==INVALID_HANDLE_VALUE) return NULL;

    len = GetFileSize( h, NULL );
    hm = CreateFileMapping( h, NULL, PAGE_READONLY, 0, len, NULL );

    return MapViewOfFile( hm, FILE_MAP_READ, 0, 0, len );
}

#elif defined(__UNIX__) && !defined(__EMX__) /* EMX is not unix but macro __UNIX__ is predefined */
/* tested on linux-glibc2, freebsd 4.2 */
#include <sys/mman.h>
#ifdef HAS_UNISTD_H
    #include <unistd.h>
#endif

void* MapFile( char* fname )
{
    int fd;
    int len;

    fd = open( fname, O_RDONLY );
    if (fd==-1) return NULL;
    len=lseek( fd, 0, SEEK_END );
    return mmap( NULL, len, PROT_READ, MAP_PRIVATE, fd, 0 );
}
#else
/* No file mapping. Just read file to memory
   please check include file names/dirs for your OS */
#ifdef HAS_IO_H
    #include <io.h>
#endif
#ifdef HAS_UNISTD_H
    #include <unistd.h>
#endif

void* MapFile(char* fname)
{
    int fd;
    long len;
    void *data;

    fd = open(fname,O_RDONLY);
    if(fd==-1) return NULL;

    len = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    data = malloc(len);
    if(!data)
    {
        close(fd);
        return NULL;
    }
    if(read(fd,data,len)!=len)
    {
        free(data);
        close(fd);
        return NULL;
    }
    close(fd);
    return data;
}
#endif
