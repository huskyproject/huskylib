/* $Id$
 *  Provides compiler-independent functions like DOS findfirst() and findnext()
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

/* standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/* huskylib: compiler.h */
#include <compiler.h>

/* compiler-dependent headers */
#if defined(HAS_DIR_H)
    #include <dir.h>
#endif

#if defined(HAS_DOS_H)
    #include <dos.h>
#endif

/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>
#include <huskylib.h>
#include <ffind.h>



#ifdef __OS2__
    #define INCL_NOPM
    #define INCL_DOS
    #include <os2.h>
    #if defined(__FLAT__)
        #undef DosQPathInfo
        #define DosQPathInfo(a,b,c,d,e)  DosQueryPathInfo(a,b,c,d)
    #endif
#endif

/*
 *  FFindOpen;  Use like MSDOS "find first" function,  except be sure to
 *  release allocated system resources by caling FFindClose() with the
 *  handle returned by this function.
 *
 *  Returns: NULL == File not found.
 */

FFIND *_fast FFindOpen(const char *filespec, unsigned short attribute)
{
    FFIND *ff;

    ff = malloc(sizeof(FFIND));

    if (ff != NULL)
    {
#if defined(__TURBOC__) || defined(__DJGPP__)

        if (findfirst(filespec, &(ff->ffbuf), attribute) != 0)
        {
            free(ff);
            ff = NULL;
        }
        else
        {
            ff->ff_attrib = ff->ffbuf.ff_attrib;
            ff->ff_ftime  = ff->ffbuf.ff_ftime;
            ff->ff_fdate  = ff->ffbuf.ff_fdate;
            ff->ff_fsize  = ff->ffbuf.ff_fsize;
            memcpy(ff->ff_name, ff->ffbuf.ff_name, sizeof(ff->ff_name));
            ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';
        }

#elif defined(__MSC__) || defined(__WATCOMC__)

        if (_dos_findfirst(filespec, attribute, &(ff->ffbuf)) != 0)
        {
            free(ff);
            ff = NULL;
        }
        else
        {
            ff->ff_attrib = ff->ffbuf.attrib;
            ff->ff_ftime  = ff->ffbuf.wr_time;
            ff->ff_fdate  = ff->ffbuf.wr_date;
            ff->ff_fsize  = ff->ffbuf.size;
            memcpy(ff->ff_name, ff->ffbuf.name, sizeof(ff->ff_name));
            ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';
        }

#elif defined(__OS2__)

#if defined(__FLAT__)
        ULONG SearchCount = 1;
        FILEFINDBUF3 findbuf;
#else
        USHORT SearchCount = 1;
        FILEFINDBUF findbuf;
#endif

        ff->hdir = HDIR_CREATE;
#ifdef __EMX__
        if (!DosFindFirst((PCSZ) filespec, &ff->hdir, attribute, &findbuf, sizeof(findbuf), &SearchCount, 1L))
#else
        if (!DosFindFirst((PBYTE) filespec, &ff->hdir, attribute, &findbuf, sizeof(findbuf), &SearchCount, 1L))
#endif
        {
            ff->ff_attrib = (char)findbuf.attrFile;
            ff->ff_fsize = findbuf.cbFile;

            ff->ff_ftime = *((USHORT *) & findbuf.ftimeLastWrite);
            ff->ff_fdate = *((USHORT *) & findbuf.fdateLastWrite);

            strncpy(ff->ff_name, findbuf.achName, sizeof(ff->ff_name));
        }
        else
        {
            free(ff);
            ff = NULL;
        }

#elif defined(__UNIX__)

        char *p;
        int fin = 0;
        struct dirent *de;

        unused(attribute);

        p = strrchr(filespec, '/');
        if (p == NULL)
        {
            strcpy(ff->firstbit, ".");
            strcpy(ff->lastbit, filespec);
        }
        else if (p == filespec)
        {
            strcpy(ff->firstbit, "/");
            strcpy(ff->lastbit, filespec+1);
        }
        else
        {
            memcpy(ff->firstbit, filespec, p - filespec);
            ff->firstbit[p - filespec] = '\0';
            strcpy(ff->lastbit, p + 1);
        }
        ff->dir = opendir(ff->firstbit);
        if (ff->dir != NULL)
        {
            while (!fin)
            {
                de = readdir(ff->dir);
                if (de == NULL)
                {
                    closedir(ff->dir);
                    free(ff);
                    ff = NULL;
                    fin = 1;
                }
                else
                {
                    if (patmat(de->d_name, ff->lastbit))
                    {
                        strncpy(ff->ff_name, de->d_name, sizeof ff->ff_name);
                        ff->ff_fsize = -1L; /* All who wants to know it's size
					      * must read it by himself
					      */
                        fin = 1;
                    }
                }
            }
        }
        else
        {
            free(ff);
            ff = NULL;
        }

#elif defined(SASC)

        char *temp;
        int error;

        temp = strrchr(filespec, '/');
        if (temp == NULL)
        {
            temp = strrchr(filespec, '\\');
        }
        if (temp == NULL)
        {
            temp = strrchr(filespec, ':');
        }
        if (temp == NULL)
        {
            strcpy(ff->prefix, "");
        }
        else
        {
            memcpy(ff->prefix, filespec, temp - filespec + 1);
            *(ff->prefix + (temp - filespec + 1)) = '\0';
        }
        error = dfind(&ff->info, filespec, 0);
        if (error == 0)
        {
            strcpy(ff->ff_name, ff->prefix);
            strcat(ff->ff_name, ff->info.fib_FileName);
        }
        else
        {
            free(ff);
            ff = NULL;
        }

#elif defined(__RSXNT__) || defined(__MINGW32__) || defined(__MSVC__)

        ff->hDirA = FindFirstFile(filespec, &(ff->InfoBuf));
        ff->attrib_srch = (char)attribute;
        while (ff->hDirA != INVALID_HANDLE_VALUE)
        {
            if (strlen(ff->InfoBuf.cFileName) < sizeof(ff->ff_name))
            {
                if ((!(ff->InfoBuf.dwFileAttributes &
                        FILE_ATTRIBUTE_DIRECTORY)) ||
                        (ff->attrib_srch & MSDOS_SUBDIR))
                {
                    break;
                }
            }
            /* skip file for some reason */
            if (!FindNextFile(ff->hDirA, &(ff->InfoBuf)))
            {
                if (ff->hDirA != INVALID_HANDLE_VALUE)
                {
                    FindClose(ff->hDirA);
                }
                ff->hDirA = INVALID_HANDLE_VALUE;
            }
        }
        if (ff->hDirA != INVALID_HANDLE_VALUE)
        {
            strcpy(ff->ff_name, ff->InfoBuf.cFileName);
            ff->ff_fsize = ff->InfoBuf.nFileSizeLow;
            ff->ff_attrib = 0;
            if (ff->InfoBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                ff->ff_attrib |= MSDOS_SUBDIR;
            }
            if (ff->InfoBuf.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
            {
                ff->ff_attrib |= _A_HIDDEN;
            }
        }
        else
        {
            free(ff);
            ff = NULL;
        }

#else
#error Unable to determine compiler and target operating system!
#endif

    }

    return ff;
}

/*
 *  FFindNext: Returns 0 if next file was found, non-zero if it was not.
 */

int _fast FFindNext(FFIND * ff)
{
    int rc = -1;

    if (ff != NULL)
    {
#if defined(__TURBOC__) || defined(__DJGPP__)

        rc = findnext(&(ff->ffbuf));

        ff->ff_attrib = ff->ffbuf.ff_attrib;
        ff->ff_ftime  = ff->ffbuf.ff_ftime;
        ff->ff_fdate  = ff->ffbuf.ff_fdate;
        ff->ff_fsize  = ff->ffbuf.ff_fsize;
        memcpy(ff->ff_name, ff->ffbuf.ff_name, sizeof(ff->ff_name));
        ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';

#elif defined(__MSC__) || defined(__WATCOMC__)

        rc = _dos_findnext(&(ff->ffbuf));

        ff->ff_attrib = ff->ffbuf.attrib;
        ff->ff_ftime  = ff->ffbuf.wr_time;
        ff->ff_fdate  = ff->ffbuf.wr_date;
        ff->ff_fsize  = ff->ffbuf.size;
        memcpy(ff->ff_name, ff->ffbuf.name, sizeof(ff->ff_name));
        ff->ff_name[sizeof(ff->ff_name) - 1] = '\0';

#elif defined(__OS2__)

#if defined(__FLAT__)
        ULONG SearchCount = 1;
        FILEFINDBUF3 findbuf;
#else
        USHORT SearchCount = 1;
        FILEFINDBUF findbuf;
#endif

        if (ff->hdir && !DosFindNext(ff->hdir, &findbuf, sizeof(findbuf),
                                     &SearchCount))
        {
            ff->ff_attrib = (char)findbuf.attrFile;
            ff->ff_ftime = *((USHORT *) & findbuf.ftimeLastWrite);
            ff->ff_fdate = *((USHORT *) & findbuf.fdateLastWrite);
            ff->ff_fsize = findbuf.cbFile;
            strncpy(ff->ff_name, findbuf.achName, sizeof(ff->ff_name));
            rc = 0;
        }

#elif defined(__UNIX__)

        int fin = 0;
        struct dirent *de;

        while (!fin)
        {
            de = readdir(ff->dir);
            if (de == NULL)
            {
                closedir(ff->dir);
                ff->dir = NULL;
                fin = 1;
            }
            else
            {
                if (patmat(de->d_name, ff->lastbit))
                {
                    strncpy(ff->ff_name, de->d_name, sizeof ff->ff_name);
                    ff->ff_fsize = -1L; /* All who wants to know it's size
					 * must read it by himself
					 */
                    fin = 1;
                    rc = 0;
                }
            }
        }

#elif defined(SASC)
        int error = 0;

        error = dnext(&ff->info);
        if (error == 0)
        {
            strcpy(ff->ff_name, ff->prefix);
            strcat(ff->ff_name, ff->info.fib_FileName);
            rc = 0;
        }
#elif defined(__RSXNT__) || defined(__MINGW32__) || defined(__MSVC__)

        do
        {
            if (!FindNextFile(ff->hDirA, &(ff->InfoBuf)))
            {
                if (ff->hDirA != INVALID_HANDLE_VALUE)
                {
                    FindClose(ff->hDirA);
                }
                ff->hDirA = INVALID_HANDLE_VALUE;
            }
            else
            {

                if (strlen(ff->InfoBuf.cFileName) < sizeof(ff->ff_name))
                {
                    if ((!(ff->InfoBuf.dwFileAttributes &
                            FILE_ATTRIBUTE_DIRECTORY)) ||
                            (ff->attrib_srch & MSDOS_SUBDIR))
                    {
                        break;
                    }
                }
            }
        }
        while (ff->hDirA != INVALID_HANDLE_VALUE);

        if (ff->hDirA != INVALID_HANDLE_VALUE)
        {

            strcpy(ff->ff_name, ff->InfoBuf.cFileName);
            ff->ff_fsize = ff->InfoBuf.nFileSizeLow;
            ff->ff_attrib = 0;
            if (ff->InfoBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                ff->ff_attrib |= MSDOS_SUBDIR;
            }
            if (ff->InfoBuf.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
            {
                ff->ff_attrib |= _A_HIDDEN;
            }
            rc = 0;
        }

#else
#error Unable to determine compiler and target operating system!
#endif
    }

    return rc;
}

/*
 *  FFindClose: End a directory search.  Failure to call this function
 *  will result in unclosed file handles under OS/2, and unreleased
 *  memory in both DOS and OS/2.
 */

void _fast FFindClose(FFIND * ff)
{
    if (ff != NULL)
    {
#if defined(__TURBOC__) || defined(__DJGPP__)
#elif (defined(__WATCOMC__NT__)) || (defined(__MSC__) && !defined(__DOS__))
        _dos_findclose(&(ff->ffbuf));
#elif defined(__OS2__)
        if (ff->hdir)
        {
            DosFindClose(ff->hdir);
        }
#elif defined(__UNIX__)
        if (ff->dir)
        {
            closedir(ff->dir);
        }
#elif defined(SASC)
#elif defined(__RSXNT__) || defined(__MINGW32__) || defined(__MSVC__)
        if (ff->hDirA != INVALID_HANDLE_VALUE)
        {
            FindClose(ff->hDirA);
        }
#endif
        free(ff);
    }
}

/*
 *  FFindInfo: This function was added because it is SIGNIFICANTLY faster
 *  under OS/2 to call DosQPathInfo() rather than DosFindFirst() if all
 *  you are interested in is getting a specific file's date/time/size.
 *
 *  PLF Thu  10-17-1991  18:12:37
 */

FFIND *_fast FFindInfo(const char *filespec)
{
#if !defined(__OS2__)
    return FFindOpen(filespec, 0);
#else
    FFIND *ff;
    FILESTATUS fs;
    const char *f;

    ff = malloc(sizeof *ff);
    if (ff == NULL)
    {
        return NULL;
    }

    memset(ff, 0, sizeof *ff);
    if (!DosQPathInfo((PBYTE) filespec, FIL_STANDARD, (PBYTE) &fs, sizeof fs, 0L))
    {
        ff->ff_attrib = (char)fs.attrFile;
        ff->ff_ftime = *((USHORT *) & fs.ftimeLastWrite);
        ff->ff_fdate = *((USHORT *) & fs.fdateLastWrite);
        ff->ff_fsize = fs.cbFile;

        /* isolate file name */
        f = strrchr(filespec, '\\');
        if (f == NULL)
        {
            f = filespec;
        }
        else
        {
            f++;
        }
        strncpy(ff->ff_name, f, sizeof(ff->ff_name));
    }
    else
    {
        free(ff);
        return NULL;
    }
    return ff;
#endif
}

#ifdef TEST /* Text functions section ======================================*/

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

/* this simple function assumes the path ALWAYS has an ending backslash */

int walk(char *path)
{
    FFIND *ff;
    int done = FALSE;
    char full[127];

    strcpy(full, path);
#ifdef MSDOS
    strcat(full, "*.*");
#else
    strcat(full, "*");
#endif

    ff = FFindOpen(full, MSDOS_SUBDIR);
    if (ff != NULL)
    {
        done = FALSE;
        while (done != TRUE)
        {
            if (ff->ff_attrib & MSDOS_SUBDIR && ff->ff_name[0] != '.')
            {
                strcpy(full, path);
                strcat(full, ff->ff_name);
                puts(full);
#ifndef __UNIX__
                strcat(full, "\\");
#else
                strcat(full, "/");
#endif
                if (!walk(full))
                {
                    return FALSE;
                }
            }
            done = FFindNext(ff) != 0;
        }
        FFindClose(ff);
        return TRUE;
    }
    else
    {
        puts("FFindOpen() failed!");
    }
    return FALSE;
}

int main(void)
{
#ifndef __UNIX__
    return walk("\\") == TRUE ? EXIT_SUCCESS : EXIT_FAILURE;
#else
    return walk("/") == TRUE ? EXIT_SUCCESS : EXIT_FAILURE;
#endif
}

#endif
