/* $Id$
 *  Provides functions to operate with linked binary tree.
 *
 * This program text was created by Paul Vixie using examples from the book:
 * "Algorithms & Data Structures," Niklaus Wirth, Prentice-Hall, 1986, ISBN
 * 0-13-022005-1.
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

#ifndef	HUSKY_TREE_H
#define	HUSKY_TREE_H

/* huskylib: compiler.h */
#include "compiler.h"

/* huskylib headers */
#include "huskyext.h"

/***  Declarations & defines  ***********************************************/

typedef struct  tree_s
{
    struct  tree_s  *tree_l, *tree_r;
    short   tree_b;
    char*   tree_p;
    char    need_b;
}
tree;

HUSKYEXT void  tree_init(tree **ppr_tree, char need_balance);

HUSKYEXT char* tree_srch(tree **ppr_tree, int (*pfi_compare)(char *, char *),char * pc_user);

HUSKYEXT int   tree_add(tree **ppr_tree, int (*pfi_compare)(char *, char *), char *pc_user, int (*pfi_delete)(char *));

HUSKYEXT int   tree_delete(tree **ppr_p, int (*pfi_compare)(char *, char *), char *pc_user, int (*pfi_uar)(char *));

HUSKYEXT int   tree_trav(tree **ppr_tree, int (*pfi_uar)(char *));

HUSKYEXT void  tree_mung(tree **ppr_tree, int (*pfi_uar)(char *));

HUSKYEXT unsigned long tree_count(tree **ppr_tree);

HUSKYEXT int tree_srchall(tree **ppr_tree, int (*pfi_compare)(char *, char *),char * pc_user);

#endif
