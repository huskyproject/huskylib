/* $Id$
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

#include <string.h>
#include <stdlib.h>

/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

HUSKYEXT char *_fast Strip_Trailing(char *str, char strip)
{
    int x;
    if (str && *str && str[x = strlen(str) - 1] == strip)
    {
        str[x] = '\0';
    }
    return str;
}

HUSKYEXT char *_fast Add_Trailing(char *str, char add)
{
    int x;
    if (str && *str && str[x = strlen(str) - 1] != add)
    {
        str[x + 1] = add;
        str[x + 2] = '\0';
    }
    return str;
}

HUSKYEXT char *_fast strocpy(char *d, const char *s)
{
    const char *orig;
    orig = s;
    memmove(d, s, strlen(s) + 1);
    return (char*)orig;
}

HUSKYEXT char *_fast firstchar(const char *strng, const char *delim, int findword)
{
    int isw=0, sl_s, wordno = 0;
    register int sl_d, x;
    register const char *string=strng;
    const char *oldstring=strng;

    /* We can't do *anything* if the string or delim is NULL or blank... */

    if ( !string || !*strng || !delim || !*delim )
    {
        return NULL;
    }

    sl_d = strlen(delim);

    for (string = strng; *string; string++)
    {
        for (x = 0, isw = 0; x <= sl_d; x++)
        {
            if (*string == delim[x])
            {
                isw=1;
            }
        }

        if (isw == 0)
        {
            oldstring = string;
            break;
        }
    }

    sl_s = strlen(string);

    for (wordno = 0; string - oldstring < sl_s; string++)
    {
        for (x = 0, isw = 0; x <= sl_d; x++)
        {
            if (*string == delim[x])
            {
                isw = 1;
                break;
            }
        }

        if (!isw && string == oldstring)
        {
            wordno++;
        }

        if (isw && string != oldstring)
        {
            for (x = 0, isw = 0; x <= sl_d; x++)
            {
                if (*(string + 1) == delim[x])
                {
                    isw = 1;
                    break;
                }
            }

            if (isw == 0)
            {
                wordno++;
            }
        }

        if (wordno == findword)
        {
            if (string == oldstring || string == oldstring + sl_s)
            {
                return (char*)string;
            }
            else
            {
                return (char*)string + 1;
            }
        }
    }

    return NULL;
}

HUSKYEXT char *extract_CVS_keyword(char *str)
{
    int l;
    char *tmp, *r;

    if(!str)
        return NULL;

    tmp = strchr(str, 0x20);

    if ((!tmp)||(!*(++tmp)))
        return NULL;

    l = strlen(tmp);

    if (l<3)
        return NULL;

    r = malloc(l-1);
    strncpy(r, tmp, l-2);
    r[l-2] = 0;

    return r;
}
