/*
 *  SMAPI; Modified Squish MSGAPI
 *
 *  Squish MSGAPI0 is copyright 1991 by Scott J. Dudley.  All rights reserved.
 *  Modifications released to the public domain.
 *
 *  Use of this file is subject to the restrictions contain in the Squish
 *  MSGAPI0 licence agreement.  Please refer to licence.txt for complete
 *  details of the licencing restrictions.  If you do not find the text
 *  of this agreement in licence.txt, or if you do not have this file,
 *  you should contact Scott Dudley at FidoNet node 1:249/106 or Internet
 *  e-mail Scott.Dudley@f106.n249.z1.fidonet.org.
 *
 *  In no event should you proceed to use any of the source files in this
 *  archive without having accepted the terms of the MSGAPI0 licensing
 *  agreement, or such other agreement as you are able to reach with the
 *  author.
 */

#ifndef __CVTDATE_H__
#define __CVTDATE_H__

#include "compiler.h"
#include "typedefs.h"

#if defined(__WATCOMC__) || defined(__DJGPP__) || defined(__MSVC__)
typedef unsigned short bits;
#else
typedef unsigned bits;  /* AIX does not allow any modifier after unsigned */
#endif

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

SMAPI_EXT void _fast ASCII_Date_To_Binary(char *msgdate, union stamp_combo *d_written);
union stamp_combo *_fast Get_Dos_Date(union stamp_combo *st);
SMAPI_EXT struct tm *_fast DosDate_to_TmDate(union stamp_combo *dosdate, struct tm *tmdate);
SMAPI_EXT union stamp_combo *_fast TmDate_to_DosDate(struct tm *tmdate, union stamp_combo *dosdate);
SMAPI_EXT char *_fast sc_time(union stamp_combo *sc, char *string);
SMAPI_EXT char *_fast fts_time(char *string, struct tm *tmdate);

#endif
