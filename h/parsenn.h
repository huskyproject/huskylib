/*
 Huskylib
 $Id$
 */

#ifndef __PARSENN_H__
#define __PARSENN_H__

#include "compiler.h"
#include "typedefs.h"

#define ZONE_ALL  56685u
#define NET_ALL   56685u
#define NODE_ALL  56685u
#define POINT_ALL 56685u

void  _fast Parse_NetNode(char *netnode, word * zone, word * net, word * node, word * point);
void  _fast ParseNN(char *netnode, word * zone, word * net, word * node, word * point, word all);

#endif
