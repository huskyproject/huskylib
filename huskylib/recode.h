/* $Id$
 *  Provides declarations to charsets translating routines
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

#ifndef HUSKY_RECODE_H
#define HUSKY_RECODE_H
/* huskylib: compiler.h */
#include "compiler.h"
/* huskylib headers */
#include "huskyext.h"
/***  Declarations & defines  ***********************************************/

#ifdef __cplusplus
extern "C" {
#endif
/* Allocate and initialise charset tables
 */
HUSKYEXT VOID initCharsets(VOID);
/* Release memory, allocated by initCharsets()
 */
HUSKYEXT VOID doneCharsets(VOID);
/* Translate using "intab" table
 */
HUSKYEXT VOID recodeToInternalCharset(char * string);

/* Translate using "outtab" table
 */
HUSKYEXT VOID recodeToTransportCharset(char * string);

/* Read translate tables from file
 * 1st parameter: input table file name (convert to internal charset)
 * 2nd parameter: output table file name (convert to transport charset)
 * Specify NULL instead file name if don't want set table
 * Return 0 if success.
 */
HUSKYEXT int getctabs(char * intabFileName, char * outtabFileName);

#ifdef __cplusplus
}
#endif

#endif // ifndef HUSKY_RECODE_H
