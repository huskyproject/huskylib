/* $Id$
 *  Provide function to compare file names on all known file systems
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* huskylib: compiler.h */
#include <compiler.h>

#define DLLEXPORT
#include <huskylib.h>


/* Retrieve full file name (path+file)
Borland -     char* _fullpath
MSVC    -     char* _fullpath
Watcom  -     char* _fullpath
MinGw32 -     char* _fullpath
gcc/emx-os2 - int   _fullpath
gcc/linux -   char* realpath   (not in all distributions)
*/
#if defined (__UNIX__)
int cmpfnames(const char *file1, const char *file2)
{
    struct stat st1, st2;
    if (stat(file1, &st1) || stat(file2, &st2))
        return 1;
    if (st1.st_dev!=st2.st_dev || st1.st_ino!=st2.st_ino)
        return 1;
    return 0;
}

#elif  defined(__NT__)

int cmpfnames(const char *file1, const char *file2)
{
    char buf[256], path1[256], path2[256], *p;

    if (sstricmp(file1, file2) == 0) return 0;
    if (!GetShortPathName(file1, buf, sizeof(buf)))
        sstrncpy(buf, file1, sizeof(buf));
    if (!GetFullPathName(buf, sizeof(path1), path1, &p)) return 1;
    if (!GetShortPathName(file2, buf, sizeof(buf)))
        sstrncpy(buf, file2, sizeof(buf));
    if (!GetFullPathName(buf, sizeof(path2), path2, &p)) return 1;

    return sstricmp(path1, path2);
}

#elif defined (__OS2__)
int cmpfnames(const char *file1, const char *file2)
{
    char path1[256], path2[256];

    /* DosQueryPathInfo declaration in os2emx.h:
      ULONG DosQueryPathInfo (PCSZ pszPathName, ULONG ulInfoLevel, PVOID pInfoBuffer, ULONG ulInfoLength);
      (PCSZ defined as const char* or unsigned const char* for different conditions)
    */
    if(  DosQueryPathInfo( (PCSZ)file1, FIL_QUERYFULLNAME, path1, sizeof(path1) )
            || DosQueryPathInfo( (PCSZ)file2, FIL_QUERYFULLNAME, path2, sizeof(path2) )
      )
        return 1;

    return sstricmp(path1, path2);
}
#elif defined (__DJGPP__)
int cmpfnames(const char *file1, const char *file2)
{
    char *path1 = NULL, *path2 = NULL;
    int result;

    /* _truename() call DOS FN 0x60 (undocumented: return real file name)
       and store string into 2nd parameter (if NULL - malloc).
       Return value is pointer to this string or NULL.
       See c:\djgpp\src\libc\dos\dos\truename.c for details */

    result = sstricmp(file1, file2);  /* sstricmp() compare NULL strings also */
    if( result==0 || file1==NULL || file2==NULL )
        return result;
    path1 = _truename( file1, NULL );
    path2 = _truename( file2, NULL );
    if( path1 && path2 ) /* if _truename() error: use result of compare original filenames */
        result = sstricmp(path1, path2);  /* else compare pathnames */
    nfree(path1);
    nfree(path2);
    return result;
}
#elif defined(__DOS__) && !defined(__FLAT__)
#include <dos.h>
int cmpfnames(const char *file1, const char *file2)
{
    struct REGPACK r;
    char path1[128], path2[128];
    r.r_ds = FP_SEG(file1);
    r.r_si = FP_OFF(file1);
    r.r_es = FP_SEG(path1);
    r.r_di = FP_OFF(path1);
    r.r_ax = 0x6000;
    intr(0x21, &r);
    r.r_ds = FP_SEG(file2);
    r.r_si = FP_OFF(file2);
    r.r_es = FP_SEG(path2);
    r.r_di = FP_OFF(path2);
    r.r_ax = 0x6000;
    intr(0x21, &r);
    return sstricmp(path1, path2);
}
#elif defined(__WATCOMC__) && defined(__DOS__) && defined(__FLAT__)
#include <i86.h>
/*
struct REGPACKX {
	unsigned int   eax, ebx, ecx, edx, ebp, esi, edi;
	unsigned short ds, es, fs, gs;
	unsigned int   flags;
};
union REGPACK {
	struct REGPACKB h;
	struct REGPACKW w;
#if defined(__386__) && !defined(__WINDOWS_386__)
	struct REGPACKX x;
#else
	struct REGPACKW x;
#endif
};
*/
int cmpfnames(const char *file1, const char *file2)
{
    union REGPACK r;
    char path1[128], path2[128];
    r.x.ds = (unsigned short)FP_SEG(file1);
    r.x.esi = (unsigned int)  FP_OFF(file1);
    r.x.es = (unsigned short)FP_SEG(path1);
    r.x.edi = (unsigned int)  FP_OFF(path1);
    r.x.eax = 0x6000;
    intr(0x21, &r);
    r.x.ds = (unsigned short)FP_SEG(file2);
    r.x.esi = (unsigned int)  FP_OFF(file2);
    r.x.es = (unsigned short)FP_SEG(path2);
    r.x.edi = (unsigned int)  FP_OFF(path2);
    r.x.eax = 0x6000;
    intr(0x21, &r);
    return sstricmp(path1, path2);
}
#else /* Unknown OS */
int cmpfnames(const char *file1, const char *file2)
{
    return sstricmp(file1, file2);
}
#endif
