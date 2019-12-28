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

/* standard headers */
#include <stdio.h>
#include <time.h>
#include <string.h>             /* for memcpy */
#include <assert.h>

/* huskylib headers */
#include <compiler.h>
#define DLLEXPORT
#include <huskyext.h>
#include <calendar.h>
#include <cvtdate.h>

#ifdef HAS_STRINGS_H
    #include <strings.h>
#endif

/***  Implementation  *******************************************************/

static int is_dst = -1;

/* Get time zone offset */

int _fast gettz(void)
{
    static int tz = 0xBAD;

    if (tz == 0xBAD)
    {
        struct tm *tm;
        time_t t, gt;

        t = time(NULL);
        tzset();
        tm = gmtime (&t);
        tm->tm_isdst = 0;
        gt = mktime(tm);
        tm = localtime (&t);
        tm->tm_isdst = 0;
        tz = (int)(((long)mktime(tm)-(long)gt));
    }
    return tz;
}

/* Find out the current status of daylight savings time */

static void near InitCvt(void)
{
    time_t tnow;
    tnow = time(NULL);
    is_dst = !!(localtime(&tnow)->tm_isdst);
}

/* Convert a DOS-style bitmapped date into a 'struct tm'-type date. */

struct tm *_fast DosDate_to_TmDate(union stamp_combo *dosdate, struct tm *tmdate)
{
    if (is_dst == -1)
    {
        InitCvt();
    }
    if (dosdate)
    {
        if (dosdate->ldate == 0)
        {
            time_t t=0;
            struct tm *tm;
            tm = gmtime(&t);
            memcpy(tmdate, tm, sizeof(*tm));
            return tmdate;
        }

        if (tmdate)
        {
            tmdate->tm_mday = dosdate->msg_st.date.da;
            tmdate->tm_mon = dosdate->msg_st.date.mo - 1;
            tmdate->tm_year = dosdate->msg_st.date.yr + 80;

            tmdate->tm_hour = dosdate->msg_st.time.hh;
            tmdate->tm_min = dosdate->msg_st.time.mm;
            tmdate->tm_sec = dosdate->msg_st.time.ss << 1;

            tmdate->tm_isdst = is_dst;
        }
    }
    return tmdate;
}

/* Convert a 'struct tm'-type date into an Opus/DOS bitmapped date */

union stamp_combo *_fast TmDate_to_DosDate(struct tm *tmdate, union stamp_combo *dosdate)
{
    if(tmdate && dosdate)
    {
        dosdate->msg_st.date.da = tmdate->tm_mday;
        dosdate->msg_st.date.mo = tmdate->tm_mon + 1;
        dosdate->msg_st.date.yr = tmdate->tm_year - 80;

        dosdate->msg_st.time.hh = tmdate->tm_hour;
        dosdate->msg_st.time.mm = tmdate->tm_min;
        dosdate->msg_st.time.ss = tmdate->tm_sec >> 1;
    }

    return dosdate;
}

static void print02d(char **str, int i)
{
    if(!(str && *str))
    {
        assert(0); /* for debug version */
        return;
    }

    if(i >= 100 || i < 0)
    {
        assert(0); /* for debug version */
        /* for release version */
        *(*str)++='X';
        *(*str)++='X';
    }
    else
    {
        *(*str)++=(char)(i/10+'0');
        *(*str)++=(char)(i%10+'0');
    }
}

char *_fast sc_time(union stamp_combo *sc, char *string)
{
    if(sc && string)
    {
        if (sc->msg_st.date.yr == 0)
        {
            *string = '\0';
        }
        else
        {
#if 0
            sprintf(string, "%02d %s %02d  %02d:%02d:%02d", sc->msg_st.date.da,
                    months_ab[sc->msg_st.date.mo - 1], (sc->msg_st.date.yr + 80) % 100,
                    sc->msg_st.time.hh, sc->msg_st.time.mm, sc->msg_st.time.ss << 1);
#else
            print02d(&string, sc->msg_st.date.da);
            *string++=' ';
            strcpy(string, months_ab[sc->msg_st.date.mo - 1]);
            string += strlen(string);
            *string++=' ';
            print02d(&string, (sc->msg_st.date.yr + 80) % 100);
            *string++=' ';
            *string++=' ';
            print02d(&string, sc->msg_st.time.hh);
            *string++=':';
            print02d(&string, sc->msg_st.time.mm);
            *string++=':';
            print02d(&string, sc->msg_st.time.ss << 1);
            *string = '\0';
#endif
        }
    }
    return string;
}

char *_fast fts_time(char *string, struct tm *tmdate)
{
    union stamp_combo dosdate;
    return sc_time(TmDate_to_DosDate(tmdate, &dosdate), string);
}

/* Date couldn't be determined, so set it to Jan 1st, 1980 */

static void near StandardDate(union stamp_combo *d_written)
{
    d_written->msg_st.date.yr = 0;
    d_written->msg_st.date.mo = 1;
    d_written->msg_st.date.da = 1;

    d_written->msg_st.time.hh = 0;
    d_written->msg_st.time.mm = 0;
    d_written->msg_st.time.ss = 0;
}

void _fast ASCII_Date_To_Binary(char *msgdate, union stamp_combo *d_written)
{
    char temp[80];

    int dd, yy, mo, hh, mm, ss, x;

    time_t timeval;
    struct tm *tim;

    timeval = time(NULL);
    tim = localtime(&timeval);

    if (*msgdate=='\0') /* If no date... */
    {
        /* OG: localtime must only generated, if msgadate == '' & yr = 1980
               A little bit more speed !
        */
        /* max: and why you leave generation upper *msgdate=='\0' ?
                but you can't remove, because it needed at the end of function
            think you must remove localtime generation below */

        if (d_written->msg_st.date.yr == 0)
        {
            timeval=time(NULL);
            tim=localtime(&timeval);

            /* Insert today's date */
            fts_time(msgdate, tim);

            StandardDate(d_written);
        }
        else /* If msgdate = '' & yr > 1980, date_written seems to be ok ! */
        {
            if (d_written->msg_st.date.mo == 0 ||
                    d_written->msg_st.date.mo > 12)
                d_written->msg_st.date.mo = 1;
            sprintf(msgdate,
                    "%02d %s %02d  %02d:%02d:%02d",
                    d_written->msg_st.date.da,
                    months_ab[d_written->msg_st.date.mo-1],
                    (d_written->msg_st.date.yr+80) % 100,
                    d_written->msg_st.time.hh,
                    d_written->msg_st.time.mm,
                    d_written->msg_st.time.ss);
        }
        return;
    }

    if (sscanf(msgdate, "%d %s %d %d:%d:%d", &dd, temp, &yy, &hh, &mm, &ss) == 6)
    {
        x = 1;
    }
    else if (sscanf(msgdate, "%d %s %d %d:%d", &dd, temp, &yy, &hh, &mm) == 5)
    {
        ss = 0;
        x = 1;
    }
    else if (sscanf(msgdate, "%*s %d %s %d %d:%d", &dd, temp, &yy, &hh, &mm) == 5)
    {
        x = 2;
    }
    else if (sscanf(msgdate, "%d/%d/%d %d:%d:%d", &mo, &dd, &yy, &hh, &mm, &ss) == 6)
    {
        x = 3;
    }
    else
    {
        x = 0;
        StandardDate(d_written);
        return;
    }

    if (x == 1 || x == 2)
    {
        /* Formats one and two have ASCII date, so compare to list */

        for (x = 0; x < 12; x++)
        {
            if (stricmp(temp, months_ab[x])==0)
            {
                d_written->msg_st.date.mo = x + 1;
                break;
            }
        }

        if (x == 12)
        {
            /* Invalid month, use January instead. */
            d_written->msg_st.date.mo = 1;
        }
    }
    else
    {
        /* Format 3 don't need no ASCII month */
        d_written->msg_st.date.mo = mo;
    }

    /* Use sliding window technique to interprete the year number */
    while (yy <= tim->tm_year - 50) yy += 100;
    while (yy >  tim->tm_year + 50) yy -= 100;

    d_written->msg_st.date.yr = yy - 80;
    d_written->msg_st.date.da = dd;

    d_written->msg_st.time.hh = hh;
    d_written->msg_st.time.mm = mm;
    d_written->msg_st.time.ss = ss >> 1;
}

union stamp_combo *_fast Get_Dos_Date(union stamp_combo *st)
{
    time_t now;
    struct tm *tm;
    now = time(NULL);
    tm = localtime(&now);
    return TmDate_to_DosDate(tm, st);
}
