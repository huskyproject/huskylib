/* $Id$
 *  Watcom C for DOS-DPMI (DOS4G) compiler-specific declarations
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

/* WATCOM C/C++ for DOS/DMPI (DOS4G) */

#ifndef HUSKY_WCX_H
#define HUSKY_WCX_H

#ifndef __WATCOMC__
    #error This file may be used only with Watcom C !
#endif
#if defined(__DOS4G__)
    #error This file may be used only with Watcom C DOS4G target !
#endif


#  define _stdc      cdecl
#  define _intr      interrupt far
#  define _intcast   void (_intr *)()
#  define _veccast   _intcast
#  define _fast      pascal

#  define farread    read
#  define farwrite   write

#  define _XPENTRY   pascal

#  define HAS_sopen     3   /* sopen() : in io.h, 3 or 4 parameters */

#endif
