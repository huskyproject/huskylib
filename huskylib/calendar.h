/**************************************************************************
 * $Id$
 * Calendar data declarations.
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
 **************************************************************************/

#ifndef HUSKY_CALENDAR_H__
#define HUSKY_CALENDAR_H__

#include "compiler.h"
#include "huskyext.h"  /* compiler see directory of this (calendar.h) file */
HUSKYEXT char _stdc months[][10];
HUSKYEXT char _stdc weekday[][10];
HUSKYEXT char _stdc months_ab[][4];
HUSKYEXT char _stdc weekday_ab[][4];

#endif
