/* $Id$
 *  Provides crc calculation routines
 *  Collected & written by Stas Degteff <g@grumbler.org> 2:5080/102
 *  (c) Stas Degteff
 *  (c) HUSKY Developers Team
 *
 * Latest version may be foind on http://husky.sourceforge.net
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

#ifndef HUSKY_CRC_H
#define HUSKY_CRC_H

#include "compiler.h"
#include "huskyext.h"

#ifdef __cplusplus
extern "C" {
#endif
/* CRC32 initial value */
#define CRC32INIT ((dword)0xFFFFFFFFUL)
/* CRC16 initial value */
#define CRC16INIT ((word)0)
/* Calculate CRC32 for memory array
   str: array
   size: array size
   initcrc: initial value (start from 0xFFFFFFFFUL)
 */
HUSKYEXT dword memcrc32(const char * str, int size, dword initcrc);
/* Alias for memcrc32() */
#define crc32(x, y, z) memcrc32(x, y, z)

/* Calculate CRC32 for ASCIIZ string
   str: string
   initcrc: initial value (start from 0xFFFFFFFFUL)
 */
HUSKYEXT dword strcrc32(const char * str, dword initcrc);

/* Calculate CRC32 for file
   filename: file name for calculate CRC32
 */
HUSKYEXT dword filecrc32(const char * filename);

/* Calculate CRC16 for memory array
   str: array
   size: array size
   initcrc: initial value (start from 0x0000)
 */
HUSKYEXT hUINT16 memcrc16(const char * str, int size, hUINT16 initcrc);
/* Alias for memcrc16() */
#define crc16(x, y, z) memcrc32(x, y, z)

/* Calculate CRC16 for ASCIIZ string
   str: string
   initcrc: initial value (start from 0x0000)
 */
HUSKYEXT hUINT16 strcrc16(const char * str, hUINT16 initcrc);

/* Calculate CRC16 for file
   filename: file name for calculate CRC16
 */
HUSKYEXT word filecrc16(const char * filename);

/*=======================================================================
 * Calculating 16-bit checksum, rotating right before each addition;
 * overflow is discarded.
 * (This algorithm is the algorithm used by historic BSD UNIX systems as
 * the `sum` algorithm and by historic AT&T System V UNIX systems
 * as the `sum -r` algorithm.)
 *=======================================================================
 */
/* 16-bit checksum (sum -r) for ASCIIZ string */
HUSKYEXT word strsum16(const char * str);
#define strsumr strsum16

/* 16-bit checksum (sum -r) for array of bytes */
HUSKYEXT word memsum16(const char * str, unsigned size);
#define memsumr memsum16

/* 16-bit checksum (sum -r) for file */
HUSKYEXT word filesum16(const char * filename);
#define filesumr(fn) filesum16((fn), NULL)

/*=======================================================================
 * Calculating 32-bit checksum described in Draft 8 POSIX 1003.2
 * (This algorithm is the algorithm used by historic AT&T System V UNIX
 * systems as the `sum` algorithm.)
 *=======================================================================
 */
/* 32bit checksum for ASCIIZ string */
HUSKYEXT dword strsum32(const char * str);

/* 32bit checksum for array of bytes */
HUSKYEXT dword memsum32(const char * str, unsigned size);

/* 32bit checksum for file
 * plen: pointer to return file lenght, unuse if plen is NULL
 */
HUSKYEXT dword filesum32(const char * filename, unsigned long * plen);


#ifdef __cplusplus
}
#endif

#endif // ifndef HUSKY_CRC_H
