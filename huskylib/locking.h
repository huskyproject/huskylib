/* $Id$
 *  Provides declarations for compiler- and platform-independent
 *  file and record locking.
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

#ifndef HUSKY_LOCKING_H__
# define HUSKY_LOCKING_H__

#include "compiler.h"
#include "huskyext.h"  /* compiler see directory of this .h file */

/* file locking implementation
 */
HUSKYEXT int lock(int handle, long ofs, long length);

/* file unlocking implementation
 */
HUSKYEXT int unlock(int handle, long ofs, long length);

/* waitlock2() wait some (OS or runtime library default) time for a lock
 */
HUSKYEXT int waitlock(int handle, long ofs, long length);

/* waitlock2() wait <t> seconds for a lock
 * THERE SHOULD BE A BETTER WAY TO MAKE A TIMED LOCK
 */
HUSKYEXT int waitlock2(int handle, long ofs, long length, long t);

#ifndef HAS_sopen
    /* #define HAS_sopen 4  */ /* sopen() : 4 parameters */
    HUSKYEXT int sopen(const char *name, int oflag, int ishared, int mode);
#endif

#endif
