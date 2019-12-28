/* $Id$
 *  Provides functions to file and directory existence check
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

/* These are compilers that have both a working stat() and (important!) the
   S_ISREG and S_ISDIR macros. The problem is that while stat() is POSIX, those
   macros are not. For compilers that do not provide these macros, we revert to
   the old "ffind" method. */

/* standard headers */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


/* huskylib: compiler.h */
#include <compiler.h>

/* standard headers */
#ifdef HAS_UNISTD_H
    #include <unistd.h>
#endif

#ifdef HAS_DIRECT_H
    #include <direct.h>
#endif

#ifdef HAS_DOS_H
    #include <dos.h>
#endif

#ifdef USE_STAT_MACROS
    /* These are compilers that have both a working stat() and (important!) the
    S_ISREG and S_ISDIR macros. The problem is that while stat() is POSIX, those
    macros are not. For compilers that do not provide these macros, we revert to
    the old "ffind" method. */
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>
#include <fexist.h>

#ifdef USE_STAT_MACROS

/* This is the nice code that works on UNIX and every other decent platform.
   It has been contributed by Alex S. Aganichev */

int _fast fexist(const char *filename)
{
    struct stat s;

    if (stat (filename, &s))
        return FALSE;
    return S_ISREG(s.st_mode);
}

long _fast fsize(const char *filename)
{
    struct stat s;

    if (stat (filename, &s))
        return -1L;
    return s.st_size;
}

time_t _fast fmtime(const char *filename)
{
    struct stat s;

    if (stat (filename, &s))
        return -1L;
    return s.st_mtime;
}

int _fast direxist(const char *directory)
{
    struct stat s;
    int rc;

#if !defined(__WATCOMC__) && !defined(__MSVC__) && !defined(__MINGW32__)
    rc = stat (directory, &s);
#else
    char *tempstr, *p;
    size_t l;
    tempstr = strdup(directory);
    if (tempstr == NULL)
    {
        return FALSE;
    }

    /* Root directory of any drive always exists! */

    if ((isalpha((int)tempstr[0]) && tempstr[1] == ':' && (tempstr[2] == '\\' || tempstr[2] == '/') &&
            !tempstr[3]) || (strcmp(tempstr, "\\")==0) )
    {
        free(tempstr);
        return TRUE;
    }

    l = strlen(tempstr);
    if (tempstr[l - 1] == '\\' || tempstr[l - 1] == '/')
    {
        /* remove trailing backslash */
        tempstr[l - 1] = '\0';
    }

    for (p=tempstr; *p; p++)
    {
        if (*p == '/')
            *p='\\';
    }

    rc = stat (tempstr, &s);

    free(tempstr);
#endif
    if (rc)
        return FALSE;
    return S_ISDIR(s.st_mode);
}

#else /* USE_STAT_MACROS not defined */

/* Here comes the ugly platform specific and sometimes even slow code. */

int _fast fexist(const char *filename)
{
    FFIND *ff;

    ff = FFindInfo(filename);

    if (ff)
    {
        FFindClose(ff);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

long _fast fsize(const char *filename)
{
    FFIND *ff;
    FILE  *fp;
    long ret = -1L;

    ff = FFindInfo(filename);

    if (ff)
    {
#ifndef __UNIX__
        ret = ff->ff_fsize;
        if (ret == -1L)
#endif
        {
            fp = fopen(filename, "rb");
            fseek(fp, 0, SEEK_END);
            ret = ftell(fp);
            fclose(fp);
        }
        FFindClose(ff);
    }

    return ret;
}

time_t _fast fmtime(const char *filename)
{
#ifdef __UNIX__
    return -1L;
#else
    FFIND *ff;
    FILE  *fp;
    time_t ret = -1L;

    ff = FFindInfo(filename);

    if (ff)
    {
        struct tm t;
        t.tm_sec  = ff->ff_ftime & 0x1f;
        t.tm_min  = (ff->ff_ftime >> 5) & 0x3f;
        t.tm_hour = (ff->ff_ftime >> 11) & 0x1f;
        t.tm_mday = ff->ff_fdate & 0x1f;
        t.tm_mon  = ((ff->ff_fdate >> 5) & 0x0f) - 1;
        t.tm_year = ((ff->ff_fdate >> 9) & 0x07f) + 1980 - 1900;
        t.tm_isdst = -1;
        FFindClose(ff);
        return mktime(&t);
    }

    return ret;
#endif
}

#if defined(__DOS__) || defined(__DPMI__)

int _fast direxist(const char *directory)
{
    FFIND *ff;
    char *tempstr;
    int ret;

    tempstr = (char *)malloc(strlen(directory) + 5);
    if (tempstr == NULL)
    {
        return FALSE;
    }

    strcpy(tempstr, directory);

    Add_Trailing(tempstr, '\\');

    /* Root directory of any drive always exists! */

    if ((isalpha(tempstr[0]) && tempstr[1] == ':' && ((tempstr[2] == '\0') || ((tempstr[2] == '\\' ||
            tempstr[2] == '/') && tempstr[3] == '\0'))) || (stricmp(tempstr, "\\")==0) )
    {
        ret = TRUE;
    }
    else
    {
        Strip_Trailing(tempstr, '\\');

        ff = FFindOpen(tempstr, MSDOS_SUBDIR | MSDOS_HIDDEN | MSDOS_READONLY);

        ret = ff != NULL && (ff->ff_attrib & MSDOS_SUBDIR);

        if (ff)
        {
            FFindClose(ff);
        }
    }

    free(tempstr);
    return ret;

}

#elif defined(__OS2__) || defined(__NT__)

#ifdef __OS2__
    #define INCL_DOSFILEMGR
    #include <os2.h>
#else
    #define WIN32_LEAN_AND_MEAN
    #define NOGDI
    #define NOUSER
    #define NOMSG
    #include <windows.h>
#endif

int _fast direxist(const char *directory)
{
    char *tempstr, *p;
    size_t l;
#if defined(__NT__) || defined(__CYGWIN__)
    DWORD attr;
#else
    FILESTATUS3 s;
#endif


    tempstr = strdup(directory);
    if (tempstr == NULL)
    {
        return FALSE;
    }

    /* Root directory of any drive always exists! */

    if ((isalpha((int)tempstr[0]) && tempstr[1] == ':' && (tempstr[2] == '\\' || tempstr[2] == '/') &&
            !tempstr[3]) || (strcmp(tempstr, "\\")==0) )
    {
        free(tempstr);
        return TRUE;
    }

    l = strlen(tempstr);
    if (tempstr[l - 1] == '\\' || tempstr[l - 1] == '/')
    {
        /* remove trailing backslash */
        tempstr[l - 1] = '\0';
    }

    for (p=tempstr; *p; p++)
    {
        if (*p == '/')
            *p='\\';
    }

#ifdef __OS2__
    if (DosQueryPathInfo((PSZ)tempstr, FIL_STANDARD,
                         (PVOID)&s, sizeof(s)) == 0)
    {
        free (tempstr);
        if (s.attrFile & FILE_DIRECTORY)
            return TRUE;
        else
            return FALSE;
    }
    free (tempstr);
    return FALSE;
#else
    attr = GetFileAttributes(tempstr);
    free(tempstr);
    if ((attr != 0xFFFFFFFF) && (attr & FILE_ATTRIBUTE_DIRECTORY))
        return TRUE;
    else
        return FALSE;
#endif
}

#elif defined(__UNIX__)

int _fast direxist(const char *directory)
{
    FILE *fp;

    fp = fopen(directory, "rb");
    if (fp != NULL)
    {
        fclose(fp);
        return 1;
    }
    return 0;
}

#else

#error Unknown compiler!

#endif

#endif

int _createDirectoryTree(const char *pathName)
{

    char *start, *slash;
    char limiter=PATH_DELIM;
    int i;

    if (!pathName)
        return 1;

    start = (char *) malloc(strlen(pathName)+2);
    if (!start)
        return 1;

    strcpy(start, pathName);
    i = strlen(start)-1;
    if (start[i] != limiter)
    {
        start[i+1] = limiter;
        start[i+2] = '\0';
    }
    slash = start;

#ifndef __UNIX__
    /*  if there is a drivename, jump over it */
    if (slash[1] == ':') slash += 2;
#endif

    /*  jump over first limiter */
    slash++;

    while ((slash = strchr(slash, limiter)) != NULL)
    {
        *slash = '\0';

        if (!direxist(start))
        {
            if (!fexist(start))
            {
                /*  this part of the path does not exist, create it */
                if (mymkdir(start) != 0)
                {
                    free(start);
                    return 1;
                }
            }
            else
            {
                free(start);
                return 1;
            }
        }

        *slash++ = limiter;
    }

    free(start);

    return 0;
}


#ifdef TEST

int main(void)
{
    printf("asdfe=%d\n", direxist("c:\\asdfe"));
    printf("blank=%d\n", direxist("c:\\blank"));
    printf("tc=%d\n", direxist("c:\\tc"));
    printf("c:\\=%d\n", direxist("c:\\"));
    printf("d:\\=%d\n", direxist("d:\\"));
    printf("e:\\=%d\n", direxist("e:\\"));
    printf("f:\\=%d\n", direxist("f:\\"));
    return 0;
}

#endif
