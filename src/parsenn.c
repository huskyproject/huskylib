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

/* standard headers */
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* huskylib: compiler.h */
#include <compiler.h>

/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

#include <parsenn.h>
#include <strext.h>

/***  Declarations & defines  ***********************************************/

const static char *colon = ":";
const static char *slash = "/";

/***  Implementation  *******************************************************/

/*HUSKYEXT void _fast Parse_NetNode(char *netnode, word * zone, word * net, word * node, word * point)
{
    ParseNN(netnode, zone, net, node, point, FALSE);
}*/

HUSKYEXT void _fast ParseNN(char *netnode, word * zone, word * net, word * node, word * point, word all)
{
    char *p;

    p = netnode;

    if (all && point)
    {
        *point = POINT_ALL;
    }

    if (all && toupper(*netnode) == 'W')
    {
        /* World */

        if (zone)
        {
            *zone = ZONE_ALL;
        }

        if (net)
        {
            *net = NET_ALL;
        }

        if (node)
        {
            *node = NODE_ALL;
        }

        return;
    }


    /* if we have a zone (and the caller wants the zone to be passed back) */

    if (strchr(netnode, ':'))
    {
        if (zone)
        {
            if (all && toupper(*p) == 'A')
            {
                /* All */
                *zone = ZONE_ALL;
            }
            else
            {
                *zone = (word) atoi(p);
            }
        }

        p = firstchar(p, colon, 2);
    }

    /* if we have a net number */

    if (p && *p)
    {
        if (strchr(netnode, '/'))
        {
            if (net)
            {
                if (all && toupper(*p) == 'A')
                {
                    /* All */
                    *net = NET_ALL;
                }
                else
                {
                    *net = (word) atoi(p);
                }
            }

            p = firstchar(p, slash, 2);
        }
        else if (all && toupper(*p) == 'A')
        {
            /* If it's in the form "1:All" or "All" */

            if (strchr(netnode, ':') == NULL && zone)
            {
                *zone = ZONE_ALL;
            }

            *net = NET_ALL;
            *node = NODE_ALL;
            p += 3;
        }
    }

    /* If we got a node number... */

    if (p && *p && node && *netnode != '.')
    {
        if (all && toupper(*p) == 'A')
        {
            /* All */

            *node = NODE_ALL;

            /* 1:249/All implies 1:249/All.All too... */

            if (point && all)
            {
                *point = POINT_ALL;
            }
        }
        else
        {
            *node = (word) atoi(p);
        }
    }

    if (p)
    {
        while (*p && isdigit((int)(*p)))
        {
            p++;
        }
    }

    /* And finally check for a point number... */

    if (p && *p == '.')
    {
        p++;

        if (point)
        {
            if (!p && *netnode == '.')
            {
                p = netnode + 1;
            }

            if (p && *p)
            {
                *point = (word) atoi(p);

                if (all && toupper(*p) == 'A')
                {
                    /* All */
                    *point = POINT_ALL;
                }
            }
            else
            {
                *point = 0;
            }
        }
    }
}
