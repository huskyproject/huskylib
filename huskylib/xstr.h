/* $Id$
 *  Provides funstions to operate dinamically allocated strings.

 * Copyright (C) 1997-2000
 *
 * Kolya Nesterov
 *
 * Fido:     2:463/567
 * Kiev, Ukraine
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

#ifndef HUSKY_XSTR_H
#define HUSKY_XSTR_H

/* huskylib: compiler.h */
#include "compiler.h"

/* huskylib headers */
#include "huskyext.h"

/***  Declarations & defines  ***********************************************/

HUSKYEXT char *xstralloc(char **s, size_t add);
/*
 * Adds to variable "s" "add" bytes (allocates or reallocates it)
 */

HUSKYEXT char *xstrcat(char **s, const char *add);
/*
 * Appends "add" to variable "s", reallocating it when it needs
 */

#define NULLP ((void*)0)
HUSKYEXT char *xstrscat(char **s, ...);
/*
 * Appends a list of strings to "s", reallocating it when it needs
 * list is terminated with NULLP value
 */

HUSKYEXT char *xstrcpy(char **s, const char *add);
/*
 * Copys "add" to variable "s", reallocating it when it needs
 */

HUSKYEXT int xscatprintf(char **s, const char *format, ...);
/*
 * sprintf's "format" to the end of "s". Reallocates it when it needs
 * return value: number of bytes added.
 */

#endif
