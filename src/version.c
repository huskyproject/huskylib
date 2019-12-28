/* $Id$
 *  Provides function to check huskylib version.
 *  Written at 2002 by Stas Degteff 2:5080/102
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

#include <string.h>
#include "compiler.h"

/* export functions from DLL */
#define DLLEXPORT
#include "huskylib.h"


/* Check version of huskylib library
 * return zero if test failed; non-zero if passed
 * test cvs need for DLL version only, using #include <fidoconf/cvsdate.h>
 */
int CheckHuskylibVersion( int need_major, int need_minor,
                          int need_patch, branch_t need_branch, const char *cvs )
{
    /* We don't need check pathlevel: see huskybse/develop-docs/ */

    static
#include "../cvsdate.h"   /* char cvs_date[]=datestring; */

    if( need_major==HUSKYLIB_VER_MAJOR && need_minor==HUSKYLIB_VER_MINOR )
    {
        if(need_branch==BRANCH_CURRENT)
        {
            if(need_patch) fprintf(stderr, "Huskylib: strange, current patch level can't be non-zero\n");
            return (HUSKYLIB_VER_BRANCH==BRANCH_CURRENT) && !(cvs && strcmp(cvs,cvs_date));
        }
        else return HUSKYLIB_VER_BRANCH!=BRANCH_CURRENT;
    }
    return 0;
}
