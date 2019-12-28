/* $Id$
 *  Provide functions to operate with memory arrays.
 *  Check to low memory when allocated.
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
#include <string.h>

/* huskylib: compiler.h */
#include <compiler.h>

#define DLLEXPORT
#include <huskylib.h>

#ifndef __LITTLE_ENDIAN__
/*
 *  put_dword
 *
 *  Writes a 4 byte word in little endian notation, independent of the local
 *  system architecture.
 */

void put_dword(byte *ptr, dword value)
{
    ptr[0] = (value & 0xFF);
    ptr[1] = (value >> 8) & 0xFF;
    ptr[2] = (value >> 16) & 0xFF;
    ptr[3] = (value >> 24) & 0xFF;
}

/*
 *  put_word
 *
 *  Writes a 4 byte word in little endian notation, independent of the local
 *  system architecture.
 */

void put_word(byte *ptr, word value)
{
    ptr[0] = (value & 0xFF);
    ptr[1] = (value >> 8) & 0xFF;
}
#endif

/* safe malloc, realloc, calloc */

void *smalloc(size_t size)
{
    void *ptr = (void *)malloc(size);
    if (ptr == NULL)
    {
        w_log(LL_CRIT, "out of memory");
        abort();
    }
    return ptr;
}

void *srealloc(void *ptr, size_t size)
{
    void *newptr = (void *)realloc(ptr, size);
    if (newptr == NULL)
    {
        w_log(LL_CRIT, "out of memory");
        abort();
    }
    return newptr;
}

void *scalloc(size_t nmemb, size_t size)
{
    void *ptr = smalloc(size*nmemb);
    memset(ptr,'\0',size*nmemb);
    return ptr;
}

void *memdup(void *p, size_t size)
{
    void *newp;
    newp = smalloc(size);
    memcpy(newp, p, size);
    return newp;
}
