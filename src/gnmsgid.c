/* $Id$
 *  Standalone msgid generator: print new msgid to stdout
 *
 *  Copyright 2003 by Alexander Reznikov, 2:4600/220@fidonet,
 *                                        homebrewer@yandex.ru
 *  and Husky Team.
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* huskylib: compiler.h */
#include <compiler.h>

#ifdef HAS_STRINGS_H
    #include <strings.h>
#endif

/* huskylib headers */
#define DLLIMPORT
#include <huskyext.h>
#include <huskylib.h>
#include <strext.h>

#include "../cvsdate.h"


/***  Declarations & defines  ***********************************************/

#define check_stricmp(a, b) (stricmp(a, b) == 0)
#define str_or_unknown(str) (str? str: "unknown")

char gnmsgid_rev[]  = "$Revision$";
char gnmsgid_date[] = "$Date$";


/***  Implementation  *******************************************************/


int outrunparse(char *line, unsigned long *seqoutrun)
{
    char *p;
    while (isspace((int)(*line))) line++;
    if (!isdigit((int)(*line)))
    {
        fprintf(stderr, "Bad SeqOutrun value '%s'!\n", line);
        return 1;
    }
    *seqoutrun = (unsigned long)atol(line);
    p = line;
    while (isdigit((int)(*p))) p++;
    if (*p == '\0') return 0;
    if (p[1])
    {
        fprintf(stderr, "Bad SeqOutrun value '%s'!\n", line);
        return 1;
    }
    switch (tolower(*p))
    {
    case 'y':
        *seqoutrun *= 365;
    case 'd':
        *seqoutrun *= 24;
    case 'h':
        *seqoutrun *= 60*60;
        break;
    case 'w':
        *seqoutrun *= 7l*24*60*60;
        break;
    case 'm':
        *seqoutrun *= 31l*24*60*60;
        break;
    default:
        fprintf(stderr, "Bad SeqOutrun value '%s'!\n", line);
        return 1;
    }
    return 0;
}

void printusage(void)
{
    printf("gnmsgid - standalone msgid generator using husky library\n");
    printf("\nUsage:\n");
    printf("gnmsgid [-h] [-v] [-seqdir <path>] [-seqoutrun <outrun>] [<num>]\n");
    printf("where\n");
    printf("  -h         - print usage information and exit\n");
    printf("  -v         - print version and exit\n");
    printf("  -seqdir    - specify msgid sequencer directory (see husky docs)\n");
    printf("  -seqoutrun - specify msgid outrun value (see husky docs)\n");
    printf("  <num>      - number msgid to generate\n");
}


void printversion(void)
{
    /*    char *rev, *date, *gen_rev, *gen_date; */
    char *rev, *date;

    rev = extract_CVS_keyword(gnmsgid_rev);
    date = extract_CVS_keyword(gnmsgid_date);

    printf("gnmsgid - standalone msgid generator using husky library\n");
    printf("\nCopyright (c) Alexander Reznikov, 2:4600/220@fidonet\n");
    printf("Copyright (c) HUSKY development team.\n\n");
    printf("gnmsgid.c revision:  %s\n", str_or_unknown(rev));
    printf("gnmsgid.c date:      %s\n", str_or_unknown(date));
    printf("huskylib CVS date:   %s\n", cvs_date);

    if (rev) free(rev);
    if (date) free(date);
}

int main(int argc, char *argv[])
{
    int i, j, perr, usage, parsed, version;
    char *s;
    char *seqdir;
    unsigned long seqoutrun;
    int num;
    dword msgid;
    char *msgiderr;

    seqdir = NULL;
    seqoutrun = 0;
    usage = 0;
    version = 0;
    perr = 0;
    num = 1;
    parsed = 0;

    for(i=1; i<argc; i++)
    {
        if ((argv[i][0] == '-')||(argv[i][0] == '/'))
        {
            s = argv[i]+1;
            if (check_stricmp(s, "dir")||check_stricmp(s, "seqdir"))
            {
                i++;
                if (i<argc)
                {
                    if (seqdir)
                        free(seqdir);
                    seqdir = strdup(argv[i]);
                }
                else
                {
                    fprintf(stderr, "'%s' option require parameter!\n", s-1);
                    perr = 1;
                    break;
                }
                continue;
            }
            if (check_stricmp(s, "outrun")||check_stricmp(s, "seqoutrun")||check_stricmp(s, "maxoutrun"))
            {
                i++;
                if (i<argc)
                {
                    if (outrunparse(argv[i], &seqoutrun)!=0)
                    {
                        perr = 1;
                        break;
                    }
                }
                else
                {
                    fprintf(stderr, "'%s' option require parameter!\n", s-1);
                    perr = 1;
                    break;
                }
            }
            if (check_stricmp(s, "?")||check_stricmp(s, "h")||check_stricmp(argv[i], "--help"))
            {
                usage = 1;
                break;
            }
            if (check_stricmp(s, "v")||check_stricmp(argv[i], "--version"))
            {
                version = 1;
                break;
            }
            fprintf(stderr, "Illegal parameter: '%s'!\n", argv[i]);
            perr = 1;
            break;
        }
        if (parsed!=0)
        {
            fprintf(stderr, "Illegal parameter: '%s'!\n", argv[i]);
            perr = 1;
            break;
        }
        s = argv[i];
        for(j=strlen(argv[i]); j>0; j--, s++)
        {
            if (!isdigit((int)(*s)))
            {
                fprintf(stderr, "Invalid <num> parameter ('%s')!\n", argv[i]);
                perr = 1;
                break;
            }
        }
        if (perr!=0)
            break;

        num = atoi(argv[i]);

        if (num<1)
        {
            fprintf(stderr, "Invalid <num> parameter ('%s')!\n", argv[i]);
            perr = 1;
        }
        break;
    }

    if (perr==0)
    {
        if (usage)
        {
            printusage();
            return 1;
        }

        if (version)
        {
            printversion();
            return 1;
        }

        for(i=1; i<=num; i++)
        {
            msgid = GenMsgIdEx(seqdir, seqoutrun, NULL, &msgiderr);
            if (msgiderr)
                fprintf(stderr, "warning (id #%i): %s, fall to ugly old algorythm\n", i, msgiderr);
            printf("%08lx\n", (ULONG)msgid);
        }
    }
    return -1;
}
