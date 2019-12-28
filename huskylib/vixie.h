/* $Id$
 *  include file to define general vixie-type things
 *  v1.0 vix 21jun86 [broken out of as.h]
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

#ifdef  DOCUMENTATION

There are two macros you can define before including this file which can
change the things defined by this file.

DVIXIE:
if defined, will cause enter/exit messages to be printed by the
ENTER/EXIT/EXITV macros.  If not defined, causes ENTER to do nothing,
          and EXIT/EXITV to generate 'return' without any messages.

          If defined, should be set to the name of the including module.

      MAIN:
          Should be defined for a program containing a main() function which
                  is linked with other modules which include this file.

                  Value is not important, only existence/nonexistence matters.

#endif  /*DOCUMENTATION*/


#ifndef HUSKY_VIXIE_H
#define HUSKY_VIXIE_H


                  /*--- debugging stuff ---*/
#define MAXPROC 256

#ifdef DVIXIE
#define ENTER(proc) { \
                        APC_PROCS[I_PROC] = proc; \
                        printf("ENTER(%d:%s.%s)\n", \
                                I_PROC, DVIXIE, APC_PROCS[I_PROC]); \
                        I_PROC++; \
                }
#define EXIT(value) { \
                        I_PROC--; \
                        printf("EXIT(%d:%s.%s)\n", \
                                I_PROC, DVIXIE, \
                                APC_PROCS[I_PROC]); \
                        return value; \
                }
#define EXITV { \
                        I_PROC--; \
                        printf("EXITV(%d:%s.%s)\n", \
                                I_PROC, DVIXIE, \
                                APC_PROCS[I_PROC]); \
                        return; \
                }
#else
#define ENTER(proc)
#define EXIT(value)     {return value;}
#define EXITV           return;
#endif

#ifdef MAIN
    int     I_PROC = 0;
    char    *APC_PROCS[MAXPROC];
#else
    extern  int     I_PROC;
    extern  char    *APC_PROCS[MAXPROC];
#endif

#endif
