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

#ifndef __STREXT_H__
#define __STREXT_H__

#include "huskyext.h"  /* compiler see directory of this .h file */

HUSKYEXT char *_fast Strip_Trailing(char *str, char strip);
HUSKYEXT char *_fast Add_Trailing(char *str, char add);

/* Copy src to dst; return src
 */
HUSKYEXT char *_fast strocpy(char *dst, const char *src);

/* Return pointer to 1st char of specified word in string (by word number)
 */
HUSKYEXT char* _fast firstchar(const char *strng, const char *delim, int findword);

/* Extract RCS/CVS keyword value from "$Keyword: value$"
 */
HUSKYEXT char *extract_CVS_keyword(char *str);

#endif
