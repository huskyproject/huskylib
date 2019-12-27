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

/* standard headers */
#include <stdlib.h>
#include <stdio.h>


/* huskylib: compiler.h */
#include <compiler.h>
#include <unused.h>


/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

/***  Declarations & defines  ***********************************************/

/* DEBUG */
#if DEBUG_TREE==1
    #define MAIN
    #define DVIXIE "tree.c"
    #define		PRMSG(msg)	printf("DEBUG: '%s'\n", msg);
#else
    #define		PRMSG(msg)
#endif

/* huskylib headers */
#include <vixie.h>
#include <tree.h>
#include <memory.h>

static unsigned long tr_count;

/***  Implementation  *******************************************************/

static char *tree_srch_real(tree **ppr_tree, int (*pfi_compare)(char *, char *), char *pc_user)
{
    register int	i_comp;

    ENTER("tree_srch")

    if (*ppr_tree)
    {
        i_comp = (*pfi_compare)(pc_user, (**ppr_tree).tree_p);
        if (i_comp > 0)
            EXIT(tree_srch_real(
                     &(**ppr_tree).tree_r,
                     pfi_compare,
                     pc_user
                 ))
            if (i_comp < 0)
                EXIT(tree_srch_real(
                         &(**ppr_tree).tree_l,
                         pfi_compare,
                         pc_user
                     ))

                /* not higher, not lower... this must be the one.
                 */
                EXIT((**ppr_tree).tree_p)
            }

    /* grounded. NOT found.
     */
    EXIT(NULL)
}

char *tree_srch(tree **ppr_tree, int (*pfi_compare)(char *, char *), char *pc_user)
{
    if(!(**ppr_tree).tree_r)
        return NULL;
    return (char *)tree_srch_real(&(**ppr_tree).tree_r, pfi_compare, pc_user);
}

/*  return value = 1 new item added */
/*  return value = 0 item was replased */
static int sprout(tree **ppr, char *pc_data, int *pi_balance,
                  int (*pfi_compare)(char *, char *), int (*pfi_delete)(char *),
                  char need_balance)
{
    tree	*p1, *p2;
    int	cmp;
    int   nRet;
    ENTER("sprout")

    /* are we grounded?  if so, add the node "here" and set the rebalance
     * flag, then exit.
     */
    if (!*ppr)
    {
        PRMSG("grounded. adding new node, setting h=true")
        *ppr = (tree *) smalloc(sizeof(tree));
        (*ppr)->tree_l = NULL;
        (*ppr)->tree_r = NULL;
        (*ppr)->tree_b = 0;
        (*ppr)->tree_p = pc_data;
        (*ppr)->need_b = need_balance;
        if (pi_balance)
            *pi_balance = need_balance;
        EXIT(1)
    }

    /* compare the data using routine passed by caller.
     */
    cmp = (*pfi_compare)(pc_data, (*ppr)->tree_p);

    /* if LESS, prepare to move to the left.
     */
    if (cmp < 0)
    {
        PRMSG("LESS. sprouting left.")
        nRet = sprout(&(*ppr)->tree_l, pc_data, pi_balance,
                      pfi_compare, pfi_delete, need_balance);
        if (*pi_balance)  	/* left branch has grown longer */
        {
            PRMSG("LESS: left branch has grown")
            switch ((*ppr)->tree_b)
            {
            case 1:	/* right branch WAS longer; balance is ok now */
                PRMSG("LESS: case 1.. balnce restored implicitly")
                (*ppr)->tree_b = 0;
                *pi_balance = FALSE;
                break;
            case 0:	/* balance WAS okay; now left branch longer */
                PRMSG("LESS: case 0.. balnce bad but still ok")
                (*ppr)->tree_b = -1;
                break;
            case -1:
                /* left branch was already too long. rebalnce */
                PRMSG("LESS: case -1: rebalancing")
                p1 = (*ppr)->tree_l;
                if (p1->tree_b == -1)  	/* LL */
                {
                    PRMSG("LESS: single LL")
                    (*ppr)->tree_l = p1->tree_r;
                    p1->tree_r = *ppr;
                    (*ppr)->tree_b = 0;
                    *ppr = p1;
                }
                else  			/* double LR */
                {
                    PRMSG("LESS: double LR")

                    p2 = p1->tree_r;
                    p1->tree_r = p2->tree_l;
                    p2->tree_l = p1;

                    (*ppr)->tree_l = p2->tree_r;
                    p2->tree_r = *ppr;

                    if (p2->tree_b == -1)
                        (*ppr)->tree_b = 1;
                    else
                        (*ppr)->tree_b = 0;

                    if (p2->tree_b == 1)
                        p1->tree_b = -1;
                    else
                        p1->tree_b = 0;
                    *ppr = p2;
                } /*else*/
                (*ppr)->tree_b = 0;
                *pi_balance = FALSE;
                break;
            } /*switch*/
        } /*if*/
        EXIT(nRet)
    } /*if*/

    /* if MORE, prepare to move to the right.
     */
    if (cmp > 0)
    {
        PRMSG("MORE: sprouting to the right")
        nRet = sprout(&(*ppr)->tree_r, pc_data, pi_balance,
                      pfi_compare, pfi_delete, need_balance);
        if (*pi_balance)  	/* right branch has grown longer */
        {
            PRMSG("MORE: right branch has grown")

            switch ((*ppr)->tree_b)
            {
            case -1:
                PRMSG("MORE: balance was off, fixed implicitly")
                (*ppr)->tree_b = 0;
                *pi_balance = FALSE;
                break;
            case 0:
                PRMSG("MORE: balance was okay, now off but ok")
                (*ppr)->tree_b = 1;
                break;
            case 1:
                PRMSG("MORE: balance was off, need to rebalance")
                p1 = (*ppr)->tree_r;
                if (p1->tree_b == 1)  	/* RR */
                {
                    PRMSG("MORE: single RR")
                    (*ppr)->tree_r = p1->tree_l;
                    p1->tree_l = *ppr;
                    (*ppr)->tree_b = 0;
                    *ppr = p1;
                }
                else  			/* double RL */
                {
                    PRMSG("MORE: double RL")

                    p2 = p1->tree_l;
                    p1->tree_l = p2->tree_r;
                    p2->tree_r = p1;

                    (*ppr)->tree_r = p2->tree_l;
                    p2->tree_l = *ppr;

                    if (p2->tree_b == 1)
                        (*ppr)->tree_b = -1;
                    else
                        (*ppr)->tree_b = 0;

                    if (p2->tree_b == -1)
                        p1->tree_b = 1;
                    else
                        p1->tree_b = 0;

                    *ppr = p2;
                } /*else*/
                (*ppr)->tree_b = 0;
                *pi_balance = FALSE;
                break;
            } /*switch*/
        } /*if*/
        EXIT(nRet)
    } /*if*/

    /* not less, not more: this is the same key!  replace...
     */
    PRMSG("I found it!  Replacing data value")
    *pi_balance = FALSE;
    if (pfi_delete)
        (*pfi_delete)((*ppr)->tree_p);
    (*ppr)->tree_p = pc_data;
    EXIT(0)
}

static int tree_add_real(tree **ppr_tree, int (*pfi_compare)(char *, char *),
                         char *pc_user, int (*pfi_delete)(char *), char need_balance)
{
    /* int	sprout(); */
    int	i_balance = FALSE;
    int   nRet = 0;

    ENTER("tree_add")
    nRet = sprout(ppr_tree, pc_user, &i_balance, pfi_compare, pfi_delete,
                  need_balance);
    EXIT(nRet)
}

int tree_add(tree **ppr_tree, int (*pfi_compare)(char *, char *),
             char *pc_user, int (*pfi_delete)(char *))
{
    return tree_add_real(&(**ppr_tree).tree_r,
                         pfi_compare,
                         pc_user,
                         pfi_delete,
                         (**ppr_tree).need_b
                        );
}

static void balanceR(tree **ppr_p, int *pi_balance)
{
    tree	*p1, *p2;
    int	b1, b2;

    ENTER("balanceR")
    PRMSG("right branch has shrunk")
    switch ((*ppr_p)->tree_b)
    {
    case 1:
        PRMSG("was imbalanced, fixed implicitly")
        (*ppr_p)->tree_b = 0;
        break;
    case 0:
        PRMSG("was okay, is now one off")
        (*ppr_p)->tree_b = -1;
        *pi_balance = FALSE;
        break;
    case -1:
        PRMSG("was already off, this is too much")
        p1 = (*ppr_p)->tree_l;
        b1 = p1->tree_b;
        if (b1 <= 0)
        {
            PRMSG("single LL")
            (*ppr_p)->tree_l = p1->tree_r;
            p1->tree_r = *ppr_p;
            if (b1 == 0)
            {
                PRMSG("b1 == 0")
                (*ppr_p)->tree_b = -1;
                p1->tree_b = 1;
                *pi_balance = FALSE;
            }
            else
            {
                PRMSG("b1 != 0")
                (*ppr_p)->tree_b = 0;
                p1->tree_b = 0;
            }
            *ppr_p = p1;
        }
        else
        {
            PRMSG("double LR")
            p2 = p1->tree_r;
            b2 = p2->tree_b;
            p1->tree_r = p2->tree_l;
            p2->tree_l = p1;
            (*ppr_p)->tree_l = p2->tree_r;
            p2->tree_r = *ppr_p;
            if (b2 == -1)
                (*ppr_p)->tree_b = 1;
            else
                (*ppr_p)->tree_b = 0;
            if (b2 == 1)
                p1->tree_b = -1;
            else
                p1->tree_b = 0;
            *ppr_p = p2;
            p2->tree_b = 0;
        }
        break;
    }
    EXITV
}


static void del(tree **ppr_r, int *pi_balance, tree **ppr_q,
                int (*pfi_uar)(char *), int *pi_uar_called)
{
    /*	void	balanceR();*/

    ENTER("del")

    if ((*ppr_r)->tree_r != NULL)
    {
        del(&(*ppr_r)->tree_r, pi_balance, ppr_q, pfi_uar,
            pi_uar_called);
        if (*pi_balance)
            balanceR(ppr_r, pi_balance);
    }
    else
    {
        if (pfi_uar)
            (*pfi_uar)((*ppr_q)->tree_p);
        *pi_uar_called = TRUE;
        (*ppr_q)->tree_p = (*ppr_r)->tree_p;
        *ppr_q = *ppr_r;
        *ppr_r = (*ppr_r)->tree_l;
        *pi_balance = TRUE;
    }

    EXITV
}


static void balanceL(tree **ppr_p, int *pi_balance)
{
    tree	*p1, *p2;
    int	b1, b2;

    ENTER("balanceL")
    PRMSG("left branch has shrunk")

    switch ((*ppr_p)->tree_b)
    {
    case -1:
        PRMSG("was imbalanced, fixed implicitly")
        (*ppr_p)->tree_b = 0;
        break;
    case 0:
        PRMSG("was okay, is now one off")
        (*ppr_p)->tree_b = 1;
        *pi_balance = FALSE;
        break;
    case 1:
        PRMSG("was already off, this is too much")
        p1 = (*ppr_p)->tree_r;
        b1 = p1->tree_b;
        if (b1 >= 0)
        {
            PRMSG("single RR")
            (*ppr_p)->tree_r = p1->tree_l;
            p1->tree_l = *ppr_p;
            if (b1 == 0)
            {
                PRMSG("b1 == 0")
                (*ppr_p)->tree_b = 1;
                p1->tree_b = -1;
                *pi_balance = FALSE;
            }
            else
            {
                PRMSG("b1 != 0")
                (*ppr_p)->tree_b = 0;
                p1->tree_b = 0;
            }
            *ppr_p = p1;
        }
        else
        {
            PRMSG("double RL")
            p2 = p1->tree_l;
            b2 = p2->tree_b;
            p1->tree_l = p2->tree_r;
            p2->tree_r = p1;
            (*ppr_p)->tree_r = p2->tree_l;
            p2->tree_l = *ppr_p;
            if (b2 == 1)
                (*ppr_p)->tree_b = -1;
            else
                (*ppr_p)->tree_b = 0;
            if (b2 == -1)
                p1->tree_b = 1;
            else
                p1->tree_b = 0;
            *ppr_p = p2;
            p2->tree_b = 0;
        }
        break;
    }
    EXITV
}


static int _delete(tree **ppr_p, int (*pfi_compare)(char *, char *), char *pc_user, int (*pfi_uar)(char *),
                   int *pi_balance, int *pi_uar_called)
{
    /*	void	del(), balanceL(), balanceR(); */
    tree	*pr_q;
    int	i_comp, i_ret;

    ENTER("_delete")

    if (*ppr_p == NULL)
    {
        PRMSG("key not in tree")
        EXIT(FALSE)
    }

    i_comp = (*pfi_compare)((*ppr_p)->tree_p, pc_user);
    if (i_comp > 0)
    {
        PRMSG("too high - scan left")
        i_ret = _delete(&(*ppr_p)->tree_l, pfi_compare, pc_user, pfi_uar,
                        pi_balance, pi_uar_called);
        if (*pi_balance)
            balanceL(ppr_p, pi_balance);
    }
    else if (i_comp < 0)
    {
        PRMSG("too low - scan right")
        i_ret = _delete(&(*ppr_p)->tree_r, pfi_compare, pc_user, pfi_uar,
                        pi_balance, pi_uar_called);
        if (*pi_balance)
            balanceR(ppr_p, pi_balance);
    }
    else
    {
        PRMSG("equal")
        pr_q = *ppr_p;
        if (pr_q->tree_r == NULL)
        {
            PRMSG("right subtree null")
            *ppr_p = pr_q->tree_l;
            *pi_balance = TRUE;
        }
        else if (pr_q->tree_l == NULL)
        {
            PRMSG("right subtree non-null, left subtree null")
            *ppr_p = pr_q->tree_r;
            *pi_balance = TRUE;
        }
        else
        {
            PRMSG("neither subtree null")
            del(&pr_q->tree_l, pi_balance, &pr_q, pfi_uar,
                pi_uar_called);
            if (*pi_balance)
                balanceL(ppr_p, pi_balance);
        }
        if (!*pi_uar_called && pfi_uar)
            (*pfi_uar)(pr_q->tree_p);
        nfree(pr_q);
        i_ret = TRUE;
    }
    EXIT(i_ret)
}


static int tree_delete_real(tree **ppr_p, int (*pfi_compare)(char *, char *), char *pc_user, int (*pfi_uar)(char *))
{
    int	i_balance = FALSE, i_uar_called = FALSE;

    ENTER("tree_delete");
    EXIT(_delete(ppr_p, pfi_compare, pc_user, pfi_uar,
                 &i_balance, &i_uar_called))
}

int tree_delete(tree **ppr_p, int (*pfi_compare)(char *, char *), char *pc_user, int (*pfi_uar)(char *))
{
    return tree_delete_real(&(**ppr_p).tree_r, pfi_compare, pc_user, pfi_uar);
}

static int tree_trav_real(tree **ppr_tree, int (*pfi_uar)(char *))
{
    ENTER("tree_trav")

    if (!*ppr_tree)
        EXIT(TRUE)

        if (!tree_trav_real(&(**ppr_tree).tree_l, pfi_uar))
            EXIT(FALSE)
            if (!(*pfi_uar)((**ppr_tree).tree_p))
                EXIT(FALSE)
                if (!tree_trav_real(&(**ppr_tree).tree_r, pfi_uar))
                    EXIT(FALSE)
                    EXIT(TRUE)
                }

int tree_trav(tree **ppr_tree, int (*pfi_uar)(char *))
{
    if (*ppr_tree)
    {
        return tree_trav_real(&(**ppr_tree).tree_r, pfi_uar);
    }
    return 0;
}

static void tree_mung_real(tree **ppr_tree, int (*pfi_uar)(char *))
{
    ENTER("tree_mung")
    if (*ppr_tree)
    {
        tree_mung_real(&(**ppr_tree).tree_l, pfi_uar);
        tree_mung_real(&(**ppr_tree).tree_r, pfi_uar);
        if (pfi_uar)
            (*pfi_uar)((**ppr_tree).tree_p);
        nfree(*ppr_tree);
        /* 		*ppr_tree = NULL; */
    }
    EXITV
}

void tree_mung(tree **ppr_tree, int (*pfi_uar)(char *))
{
    tree_mung_real(&(**ppr_tree).tree_r, pfi_uar);
    if (pfi_uar)
        (*pfi_uar)((**ppr_tree).tree_p);
    nfree(*ppr_tree);
}

static int countEach(char *pc_data)
{

    unused(pc_data);

    ENTER("count")
    tr_count++;
    EXIT(TRUE)
}

unsigned long tree_count(tree **ppr_tree)
{
    tr_count = 0;
    tree_trav(ppr_tree, countEach);
    return tr_count;
}

int tree_srchall(tree **ppr_tree, int (*pfi_compare)(char *, char *), char *pc_user)
{
    ENTER("tree_srchall")

    if (!*ppr_tree)
        EXIT(TRUE)

        if (!tree_srchall(&(**ppr_tree).tree_l, pfi_compare, pc_user))
            EXIT(FALSE)
            if (!(*pfi_compare)(pc_user, (**ppr_tree).tree_p))
                EXIT(FALSE)
                if (!tree_srchall(&(**ppr_tree).tree_r, pfi_compare, pc_user))
                    EXIT(FALSE)
                    EXIT(TRUE)
                }

void tree_init(tree **ppr_tree, char need_balance)
{
    ENTER("tree_init")
    *ppr_tree = NULL;
    sprout(ppr_tree, NULL, NULL, NULL, NULL, need_balance);
    EXITV
}
