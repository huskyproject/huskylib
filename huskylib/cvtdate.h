/* $Id$
 *  Provides date format converters to and from FTN usable.
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

#ifndef HUSKY_CVTDATE_H__
#define HUSKY_CVTDATE_H__

#include "compiler.h"
#include "huskyext.h"  /* compiler see directory of this (cvtdate.h) file */

#if defined(__WATCOMC__) || defined(__DJGPP__) || defined(__MSVC__)
    /*typedef unsigned short bits;*/
#else
    /*typedef unsigned bits;*/  /* AIX does not allow any modifier after unsigned */
#endif
typedef bit bits;

/* DOS-style datestamp */

struct _stamp
{
    struct
    {
        bits da:5;
        bits mo:4;
        bits yr:7;
    }
    date;

    struct
    {
        bits ss:5;
        bits mm:6;
        bits hh:5;
    }
    time;
};

struct _dos_st
{
    word date;
    word time;
};

/* Union so we can access stamp as "int" or by individual components */

union stamp_combo
{
    dword ldate;
    struct _stamp msg_st;
    struct _dos_st dos_st;
};

typedef union stamp_combo SCOMBO;

HUSKYEXT int _fast
gettz(void);
HUSKYEXT void _fast
ASCII_Date_To_Binary(char *msgdate, union stamp_combo *d_written);
HUSKYEXT union stamp_combo *_fast
    Get_Dos_Date(union stamp_combo *st);
HUSKYEXT struct tm *_fast
DosDate_to_TmDate(union stamp_combo *dosdate, struct tm *tmdate);
HUSKYEXT union stamp_combo *_fast
    TmDate_to_DosDate(struct tm *tmdate, union stamp_combo *dosdate);
HUSKYEXT char *_fast
sc_time(union stamp_combo *sc, char *string);
HUSKYEXT char *_fast
fts_time(char *string, struct tm *tmdate);

#endif
