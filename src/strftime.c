/* $Id$
 *  Provide strftime() implementation independent on compiler and/or system
 *  libraries.
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

/* This file contents our own implementation of strftime() function for using
 * with compilers where this function not implemented in clib.
 */

/* standard headers */
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

/***  Implementation  *******************************************************/

#ifdef HAS_strftime
/* Use function instead macro to prevent compiler warning or error.
 * (If file do not content any code some compilers report about error.)
 */
size_t cdecl strftim(char *string, size_t maxsize, const char *format, const struct tm *current_time)
{
    return strftime(string, maxsize, format, current_time);
}

#else
/* We own implementation instead strftime() */

/* Note: TZ environment variable MUST be defined to use the %Z function
 * at least in form "SET TZ=XYZ"
 */

size_t cdecl strftim(char *string, size_t maxsize, const char *format, const struct tm *current_time)
{
    const char *in;

    char *out, *scrptr;

    char temp[250];

    maxsize = min(maxsize, 230);

    for (in = format, out = temp; *in; in++)
    {
        if ((int)(out - (int)temp) >= maxsize)
        {
            break;
        }

        if (*in == '%')
        {
            switch (*++in)
            {
            case 'a':
                strcpy(out, weekday_ab[current_time->tm_wday]);
                break;

            case 'A':
                strcpy(out, weekday[current_time->tm_wday]);
                break;

            case 'b':
                strcpy(out, months_ab[current_time->tm_mon]);
                break;

            case 'B':
                strcpy(out, months[current_time->tm_mon]);
                break;

            case 'c':
                sprintf(out, "%02d-%02d-%02d %02d:%02d:%02d",
                        current_time->tm_mon + 1, current_time->tm_mday,
                        current_time->tm_year, current_time->tm_hour,
                        current_time->tm_min, current_time->tm_sec);
                break;

            case 'd':
                sprintf(out, "%02d", current_time->tm_mday);
                break;

            case 'H':
                sprintf(out, "%02d", current_time->tm_hour);
                break;

            case 'I':
                sprintf(out, "%02d",
                        current_time->tm_hour >= 0 && current_time->tm_hour <= 12 ?
                        current_time->tm_hour : current_time->tm_hour - 12);
                break;

            case 'j':
                sprintf(out, "%03d", current_time->tm_yday + 1);
                break;

            case 'm':
                sprintf(out, "%02d", (current_time->tm_mon) + 1);
                break;

            case 'M':
                sprintf(out, "%02d", current_time->tm_min);
                break;

            case 'p':
                strcpy(out, current_time->tm_hour < 12 ? "am" : "pm");
                break;

            case 'S':
                sprintf(out, "%02d", current_time->tm_sec);
                break;

            case 'U': /* Not from Sunday, BUG!!! */
                sprintf(out, "%02d", current_time->tm_yday / 7);
                break;

            case 'w':
                sprintf(out, "%d", current_time->tm_wday);
                break;

            case 'W': /* Not from Monday, BUG!!! */
                sprintf(out, "%02d", (current_time->tm_yday) / 7);
                break;

            case 'x':
                sprintf(out, "%02d-%02d-%02d", current_time->tm_mon + 1,
                        current_time->tm_mday, current_time->tm_year);
                break;

            case 'X':
                sprintf(out, "%02d:%02d:%02d", current_time->tm_hour,
                        current_time->tm_min, current_time->tm_sec);
                break;

            case 'y':
                sprintf(out, "%02d", current_time->tm_year % 100);
                break;

            case 'Y':
                sprintf(out, "%02d", current_time->tm_year + 1900);
                break;

            case 'Z':
                scrptr = getenv("TZ");
                if (scrptr != NULL)
                {
                    /*!!! todo1: replace with extract 3 first alpha chars
                                 to prevent illegal value
                          todo2: implement full parsing TZ env.var */
                    strncpy(out, scrptr, 3);
                    out[3] = '\0';
                    strUpper(out);
                }
                else
                {
                    static char firstcall=1;
                    if(firstcall)
                    {
                        firstcall=0;
                        fprintf( stderr, "Please set the TZ enviroment variable!");
                    }
                    strcpy(out, "??T");
                }
                break;

            case '%':
                strcpy(out, "%");
                break;
            }

            out += strlen(out);
        }
        else
        {
            *out++ = *in;
        }
    }

    *out = '\0';

    strcpy(string, temp);

    return strlen(string);
}
#endif
