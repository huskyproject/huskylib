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

#ifndef HUSKY_PARSENN_H__
#define HUSKY_PARSENN_H__

#include "huskyext.h"  /* compiler see directory of this .h file */

#ifdef __cplusplus
extern "C" {
#endif

#define ZONE_ALL  56685u
#define NET_ALL   56685u
#define NODE_ALL  56685u
#define POINT_ALL 56685u

/*HUSKYEXT void _fast Parse_NetNode(char *netnode, word * zone, word * net, word * node, word * point);*/
#define Parse_NetNode(netnode, zone, net, node, point) ParseNN(netnode, zone, net, node, point, FALSE)
HUSKYEXT void _fast ParseNN(char *netnode, word * zone, word * net, word * node, word * point, word all);

#ifdef __cplusplus
}
#endif

#endif
