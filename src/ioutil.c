/* $Id$
 *  Provide functions to compiler-indepenent I/O and file/dirname operations.
 *  (c) Husky Developers Team
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

/* standard headers */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <ctype.h>

/* huskylib: compiler.h */
#include <compiler.h>


/* compiler-dependent headers */
#ifdef HAS_PWD_H
    #include <pwd.h>
#endif

#ifdef HAS_SYS_UTIME_H
    #include <sys/utime.h>
#elif defined(HAS_UTIME_H)
    #include <utime.h>
#endif


/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>

#include <huskylib.h>


/***  Declarations & defines  ***********************************************/


/***  Implementation  *******************************************************/


#ifdef __TURBOC__
    #pragma warn -sig
#endif

word getUINT16(FILE *in)
{
    unsigned char dummy;

    dummy = (unsigned char) getc(in);
    return (dummy + (unsigned char) getc(in) * 256);
}

int fputUINT16(FILE *out, word data)
{
    unsigned char dummy;

    dummy = (unsigned char)(data % 256);        /*  write high Byte */
    fputc(dummy, out);
    dummy = (unsigned char)(data / 256);        /*  write low Byte */
    return fputc(dummy, out);
}

#ifdef __TURBOC__
    #pragma warn +sig
#endif


signed int fgetsUntil0(unsigned char *str, size_t n, FILE *f, char *filter)
{
    size_t i;

    for (i=0; i<n-1 ; i++ )
    {

        do
        {
            str[i] = (unsigned char)getc(f);
        }
        while (filter && *filter && str[i] && strchr(filter, str[i]) != NULL);

        /*  if end of file */
        if (feof(f))
        {
            str[i] = 0;
            return i+1;
        } /* endif */

        if (0 == str[i])
        {
            return i+1;
        } /* endif */

    } /* endfor */

    str[n-1] = 0;
    return n;
}

char *shell_expand(char *str)
{
    char *slash = NULL, *ret = NULL, c;
#ifdef __UNIX__
    struct passwd *pw = NULL;
#endif
    char *pfix = NULL;

    if (str == NULL)
    {
        return str;
    }
    if (*str == '\0' || str[0] != '~')
    {
        return str;
    }
    for (slash = str; *slash != '/' && *slash != '\0'
#ifndef __UNIX__
            && *slash != '\\'
#endif
            ; slash++);
    c = *slash;
    *slash = 0;

    if (str[1] == '\0')
    {
        pfix = getenv("HOME");
#ifdef __UNIX__
        if (pfix == NULL)
        {
            pw = getpwuid(getuid());
            if (pw != NULL)
            {
                pfix = pw->pw_dir;
            }
        }
#endif
    }
#ifdef __UNIX__
    else
    {
        pw = getpwnam(str + 1);
        if (pw != NULL)
        {
            pfix = pw->pw_dir;
        }
    }
#endif
    *slash = c;

    if (pfix == NULL)  /* could not find an expansion */
    {
        return str;
    }

    ret = smalloc(strlen(slash) + strlen(pfix) + 1);
    strcpy(ret, pfix);
    strcat(ret, slash);
    nfree(str);
    return ret;
}

#if defined(__DOS__) && !defined(__FLAT__) || defined(__WIN16__)
    /* _WINDOWS : 16-bit windows */
    #define MOVE_FILE_BUFFER_SIZE 16384
#else
    #define MOVE_FILE_BUFFER_SIZE 128000
#endif

#if defined(__NT__)
/* This function helps in dealing with files opened elsewhere with a flag FILE_SHARE_DELETE.
 * remove() on them will result in state when the file is going to be deleted, it but won't be
 * actually out of the way until whoever has it opened closes it.
 * Here is alternative --
 * rename file first (which will have effect immediately), remove afterwards. */
void fix_pending_delete(const char *to)
{
    int size;
    char *oldfile;
    size = strlen(to);
    oldfile = (char*) smalloc(size + 1 +1 + 6);
    memcpy(oldfile, to, size);
    memcpy(oldfile + size, ".XXXXXX", 8);
    if(mktemp(oldfile) == oldfile && !rename(to, oldfile))
        remove(oldfile); /* worked just fine, now try remove it */
    else /* something went wrong, let's just remove it */
        remove(to);
    nfree(oldfile);
}
#endif

int move_file(const char *from, const char *to, const int force_rewrite)
{
#if !(defined(USE_SYSTEM_COPY) && (defined(__NT__) || defined(__OS2__)))
    int rc;

#ifdef HAVE_CMPFNAMES  /* check cmpfnames for all OS and remove this condition */
    if ( cmpfnames((char*)from,(char*)to) == 0 )
        return 0;
#endif

    w_dbglog( LL_DEBUGZ, __FILE__ ":%u:move_file(%s,%s,%d)", __LINE__, from, to, force_rewrite );
    if(force_rewrite)
    {
        if(fexist(to))
        {
#if defined(__NT__)
            fix_pending_delete(to);
#else
            remove(to);
#endif
        }
    }
    else if(fexist(to))
    {
        errno=EEXIST;
        return -1;
    }

    w_dbglog( LL_DEBUGZ, __FILE__ ":%u:move_file()", __LINE__ );
    rc = rename(from, to);
    if (!rc)                 /* rename succeeded. fine! */
    {
#elif defined(__NT__) && defined(USE_SYSTEM_COPY)
    int rc;

#ifdef HAVE_CMPFNAMES  /* check cmpfnames for all OS and remove this condition */
    if ( cmpfnames((char*)from,(char*)to) == 0 )
        return 0;
#endif

    if(force_rewrite)
    {
        if(fexist(to))
        {
            fix_pending_delete(to);
        }
    }
    else if(fexist(to))
    {
        errno=EEXIST;
        return -1;
    }
    rc = MoveFile(from, to);
    if (rc == TRUE)
    {
#elif defined(__OS2__) && defined(USE_SYSTEM_COPY)
    USHORT rc;

#ifdef HAVE_CMPFNAMES  /* check cmpfnames for all OS and remove this condition */
    if ( cmpfnames((char*)from,(char*)to) == 0 )
        return 0;
#endif

    if(force_rewrite)
        remove(to);
    else if(fexist(to))
    {
        errno=EEXIST;
        return -1;
    }
    rc = DosMove((PSZ)from, (PSZ)to);
    if (!rc)
    {
#endif
        return 0;
    }

    /* Rename did not succeed, probably because the move is accross
       file system boundaries. We have to copy the file. */

    if (copy_file(from, to, force_rewrite))
    {
        w_log( LL_WARN, "Moving file from '%s' to '%s' failed, copy over failed too. This may result in loss of information and inconsistent state of the system.", from, to );
        return -1;
    }
    remove(from);
    return 0;
}


int copy_file(const char *from, const char *to, const int force_rewrite)
{
#if !(defined(USE_SYSTEM_COPY) && (defined(__NT__) || defined(OS2)))
    char *buffer;
    size_t read;
    FILE *fin, *fout;
    struct stat st;
# if defined(_MAKE_DLL_MVC_)
    struct _utimbuf ut;
# else
    struct utimbuf ut;
# endif
    int fh=-1;

    w_dbglog( LL_DEBUGZ, __FILE__ ":%u:copy_file(%s,%s,%d)", __LINE__, from, to, force_rewrite );

#ifdef HAVE_CMPFNAMES  /* check cmpfnames for all OS and remove this condition */
    if ( cmpfnames((char*)from,(char*)to) == 0 )
        return 0;
#endif

    buffer = malloc(MOVE_FILE_BUFFER_SIZE);
    if (buffer == NULL)	return -1;

    memset(&st, 0, sizeof(st));
    if (stat(from, &st))
    {
        nfree(buffer);
        return -1; /* file does not exist */
    }

    w_dbglog( LL_DEBUGZ, __FILE__ ":%u:copy_file()", __LINE__);
    fin = fopen(from, "rb");        /* todo: use open( ..., O_CREAT| ..., ...)
                                     * to prevent file overwrite */
    if (fin == NULL)
    {
        nfree(buffer);
        return -1;
    }
    w_dbglog( LL_DEBUGZ, __FILE__ ":%u:copy_file()", __LINE__);
    fh = open( to, (force_rewrite ? 0 : O_EXCL) | O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, S_IREAD | S_IWRITE );
    if( fh<0 )
    {
        fh=errno;
        fclose(fin);
        nfree(buffer);
        errno=fh;
        return -1;
    }
#if defined(__UNIX__)
    /*     flock(to,O_EXLOCK); */
    w_log( LL_DEBUGZ, __FILE__ ":%u:copy_file()", __LINE__);
    /* try to save file ownership if it is possible */
    if (fchown(fh, st.st_uid, st.st_gid) != 0)
        fchmod(fh, st.st_mode & 01777);
    else
        fchmod(fh, st.st_mode);
#endif
    w_log( LL_DEBUGZ, __FILE__ ":%u:copy_file()", __LINE__);
    fout = fdopen(fh, "wb");
    if (fout == NULL)
    {
        fh=errno;
        nfree(buffer);
        fclose(fin);
        errno=fh;
        return -1;
    }

    while ((read = fread(buffer, 1, MOVE_FILE_BUFFER_SIZE, fin)) > 0)
    {
        if (fwrite(buffer, 1, read, fout) != read)
        {
            fh=errno;
            fclose(fout);
            fclose(fin);
            remove(to);
            nfree(buffer);
            errno=fh;
            w_log( LL_DEBUGZ, __FILE__ ":%u:copy_file() failed", __LINE__);
            return -1;
        }
    }

    nfree(buffer);
    if (ferror(fout) || ferror(fin))
    {
        fh=errno;
        fclose(fout);
        fclose(fin);
        remove(to);
        errno=fh;
        w_log( LL_DEBUGZ, __FILE__ ":%u:copy_file() failed", __LINE__);
        return -1;
    }
    fclose(fin);
    if (fclose(fout))
    {
        fh=errno;
        remove(to);
        errno=fh;
        w_log( LL_DEBUGZ, __FILE__ ":%u:copy_file() failed", __LINE__);
        return -1;
    }
    ut.actime = st.st_atime;
    ut.modtime = st.st_mtime;
# if defined(_MAKE_DLL_MVC_)
    _utime(to, &ut);
# else
    utime(to, &ut);
# endif
#elif defined (__NT__) && defined(USE_SYSTEM_COPY)
    int rc = 0;

    if ( cmpfnames((char*)from,(char*)to) == 0 )
        return 0;

    if(force_rewrite)
        remove(to);            /* if CopyFile can't work file deleted..... */
    else if(fexist(to))
    {
        errno=EEXIST;
        return -1;
    }

    rc = CopyFile(from, to, FALSE);
    if (rc == FALSE)
    {
        remove(to);
        return -1;
    }
#elif defined (OS2) && defined(USE_SYSTEM_COPY)
    USHORT rc;

#ifdef HAVE_CMPFNAMES  /* check cmpfnames for all OS and remove this condition */
    if ( cmpfnames((char*)from,(char*)to) == 0 )
        return 0;
#endif

    if(force_rewrite)
        remove(to);            /* if DosCopy can't work file deleted..... */
    else if(fexist(to))
    {
        errno=EEXIST;
        return -1;
    }

    rc = DosCopy((PSZ)from, (PSZ)to, 1);
    if (rc)
    {
        remove(to);
        return -1;
    }
#endif
    w_log( LL_DEBUGZ, __FILE__ ":%u:copy_file() OK", __LINE__);
    return 0;
}

#ifndef cmdcall

#if !defined(P_WAIT) && defined(_P_WAIT)
#define P_WAIT		_P_WAIT
#endif

/* make parameters list for spawnvp(), spawnv()
 * Return malloc()'ed array of pointers to difference parts of one string,
 * 1st element points to string begin.
 * Return NULL if argument is NULL
 */
static char **mk_lst(const char *a)
{
    char *p, *q, **list=NULL, end=0, num=0;

    if(!a)
    {
        w_log(LL_ERR, "NULL command line!");
        return NULL;
    }
    while (*a && isspace(*a)) a++; /* Left spaces trim */
    p=q=sstrdup(a);
    while (*p && !end)
    {
        while (*q && !isspace(*q)) q++;
        if (*q=='\0') end=1;
        *q ='\0';
        list = (char **) srealloc(list, ++num*sizeof(char*));
        list[num-1]=(char*)p;
        if (!end)
        {
            p=q+1;
            while(isspace(*p)) p++;
        }
        q=p;
    }
    list = (char **) srealloc(list, (++num)*sizeof(char*));
    list[num-1]=NULL;

    return list;
}

int cmdcall(const char *cmd)
{
    int cmdexit=-1;
    int signal;
    char **list;

    if( (list = mk_lst(cmd)) != NULL )
    {
        w_log(LL_DEBUGV, "spawnvp(P_WAIT, %s, ...)", list[0] );
#if defined(__WATCOMC__) || defined(__MINGW32__)
        cmdexit = spawnvp(P_WAIT, list[0], (const char * const *)list);
#else
#pragma message("spawnvp")
        cmdexit = spawnvp(P_WAIT, list[0], list);
#endif
        nfree(list[0]);
        nfree(list);
    }

    if( (signal=(cmdexit & 0xff)) != 0 ) /* system error! */
        w_log(LL_ERROR, "Command execute error (spawnwp()): signal %i (Run command '%s')", signal, cmd);
    return (cmdexit & 0xFF00)>>8;   /* return code */
}
#else
/*
int cmdcall(const char *cmd)
{ return system(cmd); }
*/
#endif

int lockFile(const char *lockfile, int advisoryLock)
{
    int fh = -1;

    if(!lockfile)
        return fh;

    if (advisoryLock > 0)
    {
        while(advisoryLock > 0)
        {
            fh=open(lockfile,O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
            if (fh<0)
            {
                /*                fprintf(stderr,"cannot open/create lock file: %s wait %d seconds\n",lockfile, advisoryLock);*/
                advisoryLock--;
            }
            else
            {
                if (write(fh," ", 1)!=1)
                {
                    /*                    fprintf(stderr,"can't write to lock file! wait %d seconds\n", advisoryLock);*/
                    close(fh);
                    fh = -1;
                    advisoryLock--;
                }
                else if (lock(fh,0,1)<0)
                {
                    /*                    fprintf(stderr,"lock file used by another process! %d seconds\n", advisoryLock);*/
                    close(fh);
                    fh = -1;
                    advisoryLock--;
                }
            }
            if(fh < 0)
                sleep(1);
            else
                break;
        }
    }
    else     /*  normal locking */
    {
        fh=open(lockfile, O_CREAT|O_RDWR|O_EXCL,S_IREAD|S_IWRITE);
    }
    if(fh < 0)
    {
        fprintf(stderr,"cannot create new lock file: %s\n",lockfile);
        fprintf(stderr,"lock file probably used by another process! exit...\n");
    }
    return fh;
}

int FreelockFile(const char *lockfile, int fh)
{
    if(fh > 0)
        close(fh);
    if(lockfile)
        remove(lockfile);

    return 0;
}

/* converts decimal value to octal. useful for chmod() */
unsigned int dec2oct(unsigned int decimal)
{
    char tmpstr[6];
    unsigned int mode;

    mode = decimal;
    sprintf(tmpstr, "%u", mode);
    sscanf(tmpstr, "%o", &mode);
    return mode;
}

/*   Get the object name from the end of a full or partial pathname.
    The GetFilenameFromPathname function gets the file (or directory) name
    from the end of a full or partial pathname. Returns The file (or directory)
    name: pointer to part of all original pathname.

*/
char    *GetFilenameFromPathname(const char* pathname)
{
    char *fname = strrchr(pathname,PATH_DELIM);
    if(fname)
        fname++;
    else
        return (char*)pathname;
    return fname;
}


/*  Get the object name from the end of a full or partial pathname (OS-independed).
    This function gets the file (or directory) name from the end of a full
    or partial pathname for any path style: UNIX, DOS or mixed (mixed style
    may be used in Windows NT OS family).
    Returns the file (or directory) name: pointer to part of all original pathname.
*/
char *OS_independed_basename(const char *pathname)
{
    register char *fname=NULL, *pname=(char*)pathname;

    /* Process Unix-style, result to pathname */
    fname = strrchr(pname,'/');
    if( fname != NULL ) pname = ++fname;

    /* Process DOS-style */
    fname = strrchr(pname,'\\');
    if( fname != NULL ) ++fname;
    else fname = pname;

    return fname;
}

/* Return directory part of pathname (without filename, '/' or '\\' present at end)
 * Return value is pointer to malloc'ed string;
 * if pathname is filenfme without directory return current directory (./ or .\)
 */
char    *GetDirnameFromPathname(const char* pathname)
{
    char *sp=NULL, *rp=NULL;
    register int lll;

    sp = strrchr(pathname,PATH_DELIM);
    if( sp )
    {
        sp++;
        lll = sp-pathname;
        rp = scalloc(lll+1,sizeof(char));
        sstrncpy(rp, pathname, lll);
    }
    else
#if PATH_DELIM=='/'
        rp = sstrdup("./");
#else
        rp = sstrdup(".\\");
#endif

    return rp;
}

void fillCmdStatement(char *cmd, const char *call, const char *archiv, const char *file, const char *path)
{
    const char *start, *tmp, *add;
#ifdef __WIN32__
    char *p;
#endif
    char fullarch[256];
    char fullpath[256];

#ifdef __WIN32__
    GetFullPathName(archiv, sizeof(fullarch), fullarch, &p);
    if(*path)
        GetFullPathName(path, sizeof(fullpath), fullpath, &p);
#else
    strnzcpy(fullpath,path, 255);
    strnzcpy(fullarch,archiv, 255);
#endif

    *cmd = '\0';
    start = NULL;
    for (tmp = call; (start = strchr(tmp, '$')) != NULL; tmp = start + 2)
    {
        switch(*(start + 1))
        {
        case 'a':
            add = fullarch;
            break;
        case 'p':
            add = fullpath;
            break;
        case 'f':
            add = file;
            break;
        default:
            strncat(cmd, tmp, (size_t) (start - tmp + 1));
            start--;
            continue;
        };
        strnzcat(cmd, tmp, (size_t) (start - tmp + 1));
        strcat(cmd, add);
    };
    strcat(cmd, tmp);
}
