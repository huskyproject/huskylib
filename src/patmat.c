/* $Id$
 *  Provides functions to pattern matching. Taken from sh sources
 *
 * Copyright (c) 1999-2003
 *      The Husky Developers Team
 *
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Kenneth Almquist.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/* standard headers */
#include <string.h>
#include <ctype.h>

/* huskylib: compiler.h */
#include <compiler.h>


/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

/* huskylib headers */
#include <huskylib.h>


/***  Declarations & defines  ***********************************************/

#define CTLESC '\\'

/***  Implementation  *******************************************************/

/* Returns true if the pattern matches the string.
 */
int xpatmat(const char *string, const char *pattern, const int ncase)
{
    register const char *p, *q;
    register char c;

    p = pattern;
    q = string;
    for (;;)
    {
        c = *p++;
        switch (c)
        {
        case '\0':
            goto breakloop;
        case CTLESC:
            if (*q++ != *p++)
                return 0;
            break;
        case '?':
            if (*q++ == '\0')
                return 0;
            break;
        case '#':
            if (*q == '\0')
                return 0;
            else if (!isdigit(*q++))
                return 0;
            break;
        case '*':
            c = *p;
            if (c != CTLESC && c != '?' && c != '*' && c != '[' && c != '#')
            {
                while (*q != c)
                {
                    if (ncase && toupper(*q) == toupper(c))
                        break;
                    if (*q == '\0')
                        return 0;
                    q++;
                }
            }
            do
            {
                if (xpatmat(q, p, ncase))
                    return 1;
            }
            while (*q++ != '\0');
            return 0;
        case '[':
        {
            const char *endp;
            int invert, found;
            char chr;

            endp = p;
            if (*endp == '!')
                endp++;
            for (;;)
            {
                if (*endp == '\0')
                    goto dft;	/* no matching ] */
                if (*endp == CTLESC)
                    endp++;
                if (*++endp == ']')
                    break;
            }
            invert = 0;
            if (*p == '!')
            {
                invert++;
                p++;
            }
            found = 0;
            chr = *q++;
            c = *p++;
            do
            {
                if (c == CTLESC)
                    c = *p++;
                if (*p == '-' && p[1] != ']')
                {
                    p++;
                    if (*p == CTLESC)
                        p++;
                    if (chr >= c && chr <= *p)
                        found = 1;
                    p++;
                }
                else
                {
                    if (chr == c)
                        found = 1;
                }
            }
            while ((c = *p++) != ']');
            if (found == invert)
                return 0;
            break;
        }
dft:
        default:
            if (ncase)
            {
                if (toupper(*q++) != toupper(c))
                    return 0;
            }
            else if (*q++ != c)
                return 0;
            break;
        }
    }
breakloop:
    if (*q != '\0')
        return 0;
    return 1;
}

#ifdef TEST

#include <stdio.h>

int main(void)
{
    printf("patmat(\"abcdefghi\", \"*ghi\"): %d\n", patmat("abcdefghi", "*ghi"));
    printf("patmat(\"abcdefghi\", \"??c??f*\"): %d\n", patmat("abcdefghi", "??c??f*"));
    printf("patmat(\"abcdefghi\", \"*dh*\"): %d\n", patmat("abcdefghi", "*dh*"));
    printf("patmat(\"abcdefghi\", \"*def\"): %d\n", patmat("abcdefghi", "*def"));
    return 0;
}

#endif
