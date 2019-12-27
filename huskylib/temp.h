/* $Id$
 *  Provides temp files & directories operating functions
 *
 * (c) Stas Degteff <g@grumbler.org>, 2:5080/102@fidonet
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

#ifndef HUSKY_TEMP_H__
#define HUSKY_TEMP_H__

/* standard headers */
#include <stdio.h>

/* huskylib: compiler.h */
#include "compiler.h"

/* huskylib headers */
#include "huskyext.h"

/***  Declarations & defines  ***********************************************/

/* Default temporary files suffix (==extension in DOS-like OS)
 * (re)define it in your program source if want other.
 */
#ifndef TEMPFILESUFFIX
    #define TEMPFILESUFFIX "tmp"
#endif


/* Create new file with random name & specified suffix in specified directory.
 * path = temporary directory
 * ext = file name suffix
 * mode = 'w' or 'b' - file open mode (text or binary); default = fopen() default
 * Place to 'name' variable name of created file (from malloc() memory pool),
 * if name is not NULL its free().
 * Return file descriptor or NULL
 */
HUSKYEXT FILE *createTempFileIn(const char *path, const char *ext, char mode, char **name);


/* Create new file with random name & default suffix (tmp) in text mode.
 * pconfig = fidoconfig structure pointer
 * Place to 'name' variable name of created file (from malloc() memory pool),
 * if name is not NULL its free().
 * Return file descriptor or NULL
 */
HUSKYEXT FILE *createTempTextFile(char *tempDir, char **name);


/* Create new file with random name & default suffix (tmp) in binary mode.
 * pconfig = fidoconfig structure pointer
 * Place to 'name' variable name of created file (from malloc() memory pool),
 * if name is not NULL its free().
 * Return file descriptor or NULL
 */

HUSKYEXT FILE *createTempBinFile(char *tempDir, char **name);
/* Create new file with random name & default suffix (binary mode).
 * pconfig = fidoconfig structure pointer
 * Place to 'name' variable name of created file (from malloc() memory pool),
 * if name is not NULL its free().
 * Return file descriptor or NULL
 */
#define createTempFile(pconfig,name) (createTempBinFile(pconfig,name))

#endif
