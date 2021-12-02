/* $Id$
 *  Provides function to generate program version string
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
#include "huskyext.h"
#include "huskylib.h"
/* Generate version string like
 * programname/platform[-compiler] <major>.<minor>.<patchlevel>-<branch> [<cvs date>]
 *
 * Examples:
 * "program/w32-MVC 1.2.3-release"
 * "program/DPMI-DJGPP 1.2.3-stable 01-10-2002"
 * "program/FreeBSD 1.3.0-current 01-10-2002"
 * Return malloc'ed pointer
 */
char * GenVersionStr(const char * programname,
                     unsigned major,
                     unsigned minor,
                     unsigned patchlevel,
                     unsigned branch,
                     const char * cvsdate)
{
    char * _version_str = NULL; /* compose to this var */
    char * platform;
    char * cbranch = NULL;   /* branch string */

#ifdef UNAME                 /* Use '-DUNAME' compiler parameter */
    platform = "/" UNAME;

#elif defined (__linux__)    /*  gcc on Linux                    */
    platform = "/lnx";

#elif defined (__FreeBSD__)  /*  gcc on FreeBSD                  */
    platform = "/fbsd";

#elif defined (__NetBSD__)   /*  gcc on NetBSD                   */
    platform = "/nbsd";

#elif defined (__OpenBSD__)  /*  gcc on OpenBSD                  */
    platform = "/obsd";

#elif defined (__BSD__)      /*  gcc on other BSD clone          */
    platform = "/bsd";

#elif defined (__SUN__)      /*  SunOS (Solaris)                 */
#  if defined (__GNUC__)
    platform = "/sun-gcc";
#  else
    platform = "/sun";
#  endif

#elif defined (__APPLE__) && defined (__MACH__)
    platform = "/mac";

#elif defined (_AIX)
    platform = "/aix";

#elif defined (__osf__)
    platform = "/osf";

#elif defined (__hpux)
    platform = "/hpux";

#elif defined (__OS2__) || defined (OS2)
#  if defined (__TURBOC__) /* Borland C/C++ for OS/2 */
    platform = "/os2-bc";
#  elif defined (_MSC_VER) /* Microsoft C or Microsoft QuickC for OS/2 */
    platform = "/os2-msc";
#  elif defined (__WATCOMC__)
    platform = "/os2-wc";
#  elif defined (__IBMC__) /* IBM C/Set++ for OS/2 */
    platform = "/os2-ibmc";
#  elif defined (__HIGHC__)/* MetaWare High C/C++ for OS/2 */
    platform = "/os2-hc";
#  elif defined (__EMX__)  /* EMX for 32-bit OS/2 */
    platform = "/os2-emx";
#  else
    platform = "/os2";
#  endif

#elif defined (__HIGHC__) /* MetaWare High C/C++ for OS/2 */
    platform = "/os2-hc";

#elif defined (__IBMC__) && !defined (UNIX)
    /* IBM C/Set++ for OS/2 */
    platform = "/os2-ibmc";

#elif defined (__MSVC__)
#    if !defined (_MAKE_DLL_MVC_) && defined (_WIN64)
    platform = "/w64-mvc";
#    elif defined (_MAKE_DLL_MVC_) && defined (_WIN64)
    platform = "/w64-mvcdll";
#    elif defined (_MAKE_DLL_MVC_) && !defined (_WIN64)
    platform = "/w32-mvcdll";
#    else
    platform = "/w32-mvc";
#    endif
#elif (defined (__MINGW32__) && defined (_WIN64)) || defined (__MINGW64__)
#    if defined (_MAKE_DLL)
    platform = "/w64-mgwdll";
#    else
    platform = "/w64-mgw";
#    endif
#elif defined (__MINGW32__)
#    if defined (_MAKE_DLL)
    platform = "/w32-mgwdll";
#    else
    platform = "/w32-mgw";
#    endif
#elif defined (__WIN32__)

#  if defined (__TURBOC__) /* Borland C/C++ for Win32 */
    platform = "/w32-bc";
#  elif defined (__WATCOMC__)
    platform = "/w32-wc";
#  elif defined (__EMX__)    /* RSX for Windows NT */
    platform = "/w32-rsx";
#  else
    platform = "/w32";
#  endif

#elif defined (_WINDOWS)
#  if defined (__WATCOMC__)
    platform = "/win-wc";
#  else
    platform = "/win";
#  endif

#  elif defined (__EMX__)    /* EMX for 32-bit OS/2 */
    platform = "/emx";

#elif defined (__DJGPP__)
    platform = "/dpmi-djgpp";

#elif defined (__DOS__)
#  ifdef __DJGPP__
    platform = "/dpmi-djgpp";
#  elif defined (__WATCOMC__) && defined (__DOS4G__)
    platform = "/dos4g-wc";
#  elif defined (__WATCOMC__) && defined (__FLAT__)
    platform = "/dpmi-wc";
#  elif defined (__WATCOMC__) && !defined (__FLAT__)
    platform = "/dos-wc";
#  elif defined (__TURBOC__)
    platform = "/dos-bc";
#  elif defined (__MSC__) /* Microsoft C or Microsoft QuickC for MS-DOS */
    platform = "/dos-msc";
#  elif defined (__FLAT__)
    platform = "/dpmi";
#  else
    platform = "/dos";
#  endif

#elif defined (__BEOS__)
    platform = "/beos";

#elif defined (SASC)                          /* SAS C for AmigaDOS */
    platform = "/amiga-sasc";

#elif defined (__UNIX__)
    platform = "/unix";

#else  /* ifdef UNAME */
    platform = "";
#ifdef __GNUC__
#warning Unknown platform and compiler!
#else
#pragma message("Unknown platform and compiler!")
#endif
#endif /* ifdef UNAME */

    switch(branch)
    {
        case BRANCH_CURRENT:
            cbranch = "-cur";

            if(!(minor & 1))
            {
                fprintf(stderr,
                        __FILE__ ":%u: illegal usage of GenVersionStr(): minor value for current branch must be odd!\n",
                        __LINE__);
            }

            if(patchlevel)
            {
                fprintf(stderr,
                        __FILE__ ":%u: illegal usage of GenVersionStr(): patchlevel value for current branch must be zero!\n",
                        __LINE__);
                /*                         pathlevel = 0;*/
            }

            break;

        case BRANCH_STABLE:
            cbranch = "-sta";

            if(minor & 1)
            {
                fprintf(stderr,
                        __FILE__ ":%u: illegal usage of GenVersionStr(): minor value for stable branch must be even!\n",
                        __LINE__);
            }

            break;

        case BRANCH_RELEASE:
            cbranch = "-rel";

            if(minor & 1)
            {
                fprintf(stderr,
                        __FILE__ ":%u: illegal usage of GenVersionStr(): minor value for release branch must be even!\n",
                        __LINE__);
            }

            break;
    } /* switch */

    if(branch == BRANCH_RELEASE || branch == BRANCH_STABLE)
    {
        xscatprintf(&_version_str,
                    "%s%s %u.%u.%u%s",
                    programname,
                    platform,
                    major,
                    minor,
                    patchlevel,
                    cbranch);
    }
    else
    {
        xscatprintf(&_version_str,
                    "%s%s %u.%u %s",
                    programname,
                    platform,
                    major,
                    minor,
                    cvsdate);
    }

    return _version_str;
} /* GenVersionStr */
