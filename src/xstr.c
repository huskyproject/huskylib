/* $Id$
 *  Provides funstions to operate dinamically allocated strings.

 * Copyright (C) 1997-2000
 *
 * Kolya Nesterov
 *
 * Fido:     2:463/567
 * Kiev, Ukraine
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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


/* huskylib: compiler.h */
#include <compiler.h>

/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

/* huskylib headers */
#include <memory.h>
#include <xstr.h>


/***  Declarations & defines  ***********************************************/

#if defined(VSPRINTF_ONLY)
    #undef HAS_vsnprintf
    #undef HAS_vasprintf
#endif

#define N_PRINTFBUF     1024

/***  Implementation  *******************************************************/

char *xstralloc(char **s, size_t add)
{
    int n;
    if (*s == NULL)
    {
        *s = smalloc(add + 1);
        **s = '\0';
        n = 0;
    }
    else
    {
        *s = srealloc(*s, (n = strlen(*s)) + add + 1);
    };
    if (*s == NULL)
    {
        fprintf(stderr, "out of memory");
        abort();
    }
    return *s + n;
}

char *xstrcat(char **s, const char *add)
{
    if(add == NULL)
        return *s;
    return strcat(xstralloc(s, strlen(add)), add);
}

char *xstrcpy(char **s, const char *add)
{
    nfree(*s);
    return xstrcat(s, add);
}


char *xstrscat(char **s, ...)
{
    va_list	ap;
    char	*q, *p;
    int	ncat;
    for (va_start(ap, s), ncat = 0; (p = va_arg(ap, char *)) != NULLP; )
        ncat += strlen(p);
    va_end(ap);
    p = xstralloc(s, ncat);
    for (va_start(ap, s); (q = va_arg(ap, char *)) != NULLP; )
        p = strcat(p, q);
    va_end(ap);
    return p;
}

int xscatprintf(char **s, const char *format, ...)
{
    va_list ap;
#if defined(HAS_vasprintf)
    char *addline;
#elif defined(HAS_vsnprintf)
    char *addline;
    int  nmax;
#else
    char addline[N_PRINTFBUF];
#endif
    int  nprint;

    va_start(ap, format);
#if defined(HAS_vasprintf)
    vasprintf(&addline, format, ap);
#elif defined(HAS_vsnprintf)
    addline = NULL;
    for (nmax = N_PRINTFBUF; ; )
    {
        xstralloc(&addline, nmax);
        nprint = vsnprintf(addline, nmax, format, ap);
        /* If that worked, return the string. */
        if (nprint > -1 && nprint < nmax)
            break;
        /* Else try again with more space. */
        if (nprint > -1)
            nmax = nprint+1;  /* precisely what is needed */
        else
            nmax += N_PRINTFBUF;        /* twice the old size */
    };
#else
    nprint = vsprintf(addline, format, ap);
    if (nprint > N_PRINTFBUF)
    {
        fprintf(stderr, "sprintf buffer overflow at xscatprintf.\n" \
                "used %d bytes instead of %d\n" \
                "format leading to this was : %s\n"\
                "please tell the developers\n", nprint,
                N_PRINTFBUF, format);
        abort();
    };
#endif
    va_end(ap);
    xstrcat(s, addline);
#if defined(HAS_vasprintf) || defined(HAS_vsnprintf)
    free(addline);
#endif
    return nprint;
}

#ifdef TEST

int main(void)
{
    char *s = NULL;
    xstralloc(&s, 10);
    strcpy(s, "1234567890");
    xstrcat(&s, " test");
    xstrscat(&s, " this", " one", NULL);
    xscatprintf(&s, " %d %d", 3, 4);
    printf("%s", s);
    return strcmp(s, "1234567890 test this one 3 4");
}

#endif
