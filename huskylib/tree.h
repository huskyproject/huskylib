/* tree.h - declare structures used by tree.c
 * vix 27jun86 [broken out of tree.c]
 */


#ifndef	_TREE_FLAG
#define	_TREE_FLAG

#include <huskylib/huskylib.h>

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

#endif /*_TREE_FLAG*/
