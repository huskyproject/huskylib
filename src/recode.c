/* $Id$
 *  Provides charsets translating routines
 *
 * Compiled from hpt/recode
 * by Stas Degteff <g@grumbler.org>, 2:5080/102@fidonet
 *
 * Code taken from ifmail written by Eugene G. Crosser <crosser@pccross.msk.su>
 * Ported to HPT by Dmitry Borowskoy <dim@bacup.ru>
 *
 * Portions copyright (C) Matthias Tichy
 *                        Fido:     2:2433/1245 2:2433/1247 2:2432/605.14
 *                        Internet: mtt@tichy.de
 * Portions copyright (C) Max Levenkov
 *                        Fido:     2:5000/117
 *                        Internet: sackett@mail.ru
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/* huskylib: compiler.h */
#include <compiler.h>


/* compiler-dependent headers */

/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

/* huskylib headers */
#include <huskylib.h>


/***  Declarations & defines  ***********************************************/

static char *intab  = NULL;
static char *outtab = NULL;

static int getctab(char *dest, char *charMapFileName );

/***  Implementation  *******************************************************/

void initCharsets(void)
{
    int i;
    intab	= (char *) smalloc(sizeof(char) * 256);
    outtab	= (char *) smalloc(sizeof(char) * 256);
    for (i = 0; i < 256; i++) intab[i] = outtab[i] = (char) i;
}

void doneCharsets(void)
{
    nfree(intab);
    nfree(outtab);
}

void recodeToInternalCharset(char *string)
{
    int c;

    if( !intab || !outtab ) initCharsets();

    if (string != NULL)
    {
        for( ; *string != '\000'; string++ )
        {
            c=((int)*string)&0xFF;
            *string = intab[c];
        }
    }

}

void recodeToTransportCharset(char *string)
{
    int c;

    if( !intab || !outtab ) initCharsets();

    if (string != NULL)
    {
        for( ; *string != '\000'; string++ )
        {
            c=((int)*string)&0xFF;
            *string = outtab[c];
        }
    }

}

/* Read translate tables from file
 * 1st parameter: input table file name (convert to internal charset)
 * 2nd parameter: output table file name (convert to transport charset)
 * Specify NULL instead file name if don't want set table
 * Return 0 if success.
 */
int getctabs(char *intabFileName, char *outtabFileName )
{
    int rc=0;
    if(intabFileName) rc += getctab(intab,intabFileName);
    if(outtabFileName) rc += getctab(outtab,outtabFileName);
    return rc;
}

/* Read specified translate table from file
 */
static int getctab(char *dest, char *charMapFileName )
{
    FILE *fp;
    char buf[512],*p,*q;
    int in,on,count;
    int line, rc=0;

    if( !intab || !outtab ) initCharsets();

    fp=fopen((char *)charMapFileName,"r");
    if (fp == NULL)
    {
        fprintf(stderr,"getctab: cannot open mapchan file \"%s\"\n", charMapFileName);
        return 1;
    }

    count=0;
    line = 0;
    while (fgets((char*)buf,sizeof(buf),fp))
    {
        line++;
        p=(char *)strtok((char*)buf," \t\n#");
        q=(char *)strtok(NULL," \t\n#");

        if (p && q)
        {
            in = ctoi((char *)p);
            if (in > 255)
            {
                fprintf(stderr, "getctab: %s: line %d: char val too big\n", charMapFileName, line);
                rc = 1;
                break;
            }
            on=ctoi((char *)q);
            if (in && on)
            {
                if( count++ < 256 ) dest[in]=(char)on;
                else
                {
                    fprintf(stderr,"getctab: char map table \"%s\" is too big\n",charMapFileName);
                    rc = 1;
                    break;
                }
            }
        }
    }
    fclose(fp);

    w_log('2',"read recoding table from %s", charMapFileName);
    return rc;
}
