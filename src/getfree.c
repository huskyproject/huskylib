/* $Id$
 *  Provides compiler-independent function for take free disk space value.
 *
 *  This code taken from BinkD and modified a bit for Husky by Max Levenkov
 *  at 2001
 *  Copyright (c) 1997 by Fydodor Ustinov
 *                       FIDONet 2:5020/79
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
#include <limits.h>
#include <ctype.h>
#include <sys/types.h>


/* huskylib: compiler.h */
#include <compiler.h>


/* compiler-dependent headers */
#ifdef HAS_SYS_PARAM_H
    #include <sys/param.h>
#endif
#ifdef HAS_SYS_MOUNT_H
    #include <sys/mount.h>
#endif
#ifdef HAS_SYS_STATFS_H
    #include <sys/statfs.h>
#endif
#ifdef HAS_SYS_STATVFS_H
    #include <sys/statvfs.h>
#endif
#ifdef HAS_SYS_VFS_H
    #include <sys/vfs.h>
#endif
#ifdef HAS_DOS_H
    #include <dos.h>
#endif

/* huskylib headers */
#define DLLEXPORT
#include <huskylib.h>



/***  Declarations & defines  ***********************************************/

#ifdef __WATCOMC__NT__
    #ifndef MAXPATHLEN
        #define MAXPATHLEN NAME_MAX
    #endif
#endif

#ifdef __MSVC__
    #ifndef MAXPATHLEN
        #define MAXPATHLEN _MAX_PATH
    #endif
#endif

#ifndef MAXPATHLEN
    #define MAXPATHLEN 255
#endif

/***  Implementation  *******************************************************/

#ifdef __WIN32__

#include <windows.h>

/* Return value is free space in kiB */
/* Note: if function can't get actual value of free space is assumes maximum possible value */
unsigned long husky_GetDiskFreeSpace (const char *path)
{
    FARPROC pGetDiskFreeSpaceEx = NULL;
    BOOL rc;
    unsigned long freeSpace = unsigned_long_max;


    pGetDiskFreeSpaceEx = GetProcAddress( GetModuleHandle("kernel32.dll"),
                                          "GetDiskFreeSpaceExA");

    if (pGetDiskFreeSpaceEx)
    {
        ULARGE_INTEGER i64FreeBytesToCaller,i64TotalBytes,i64FreeBytes;
        rc = pGetDiskFreeSpaceEx (path,
                                  (PULARGE_INTEGER)&i64FreeBytesToCaller,
                                  (PULARGE_INTEGER)&i64TotalBytes,
                                  (PULARGE_INTEGER)&i64FreeBytes);
        if (rc != TRUE)
        {
            w_log (LL_ERR, "GetDiskFreeSpace error: return code = %lu", GetLastError());
            /* return freeSpace;		    Assume enough disk space */
        }
        else
        {
            i64FreeBytesToCaller.QuadPart /= 1024;
            if( i64FreeBytesToCaller.QuadPart < unsigned_long_max )
                freeSpace = (unsigned long)i64FreeBytesToCaller.QuadPart;
            /*  Process GetDiskFreeSpaceEx results. */
        }
    }
    else
    {
        DWORD SPC;				/*  sectors per cluster */
        DWORD BPS;				/*  bytes per sector */
        DWORD FC;				/*  number of free clusters */
        DWORD TNC;				/*  total number of clusters */
        char RPN[MAXPATHLEN];	/*  root path */
        char *pRPN;             /*  Pointer to Root path */
        pRPN = RPN;
        if (isalpha(path[0]) && path[1] == ':' )
        {
            /*  Drive letter */
            RPN[0] = path[0];
            RPN[1] = ':';
            RPN[2] = '\\';
            RPN[3] = '\0';
        }
        else if (path[0] == '\\' && path[1] == '\\')
        {
            /*  UNC path */
            int i;
            RPN[0] = '\\';
            RPN[1] = '\\';
            i = 2;
            /*  copy server name.... */
            do
            {
                RPN[i] = path[i];
            }
            while (path[i++] != '\\');
            /*  .... and share name */
            do
            {
                RPN[i] = path[i];
            }
            while (path[i++] != '\\');

            RPN[i] = '\0';

        }
        else
        {
            /*  Current Drive */
            pRPN = NULL;
        }


        rc = GetDiskFreeSpace (pRPN,&SPC,&BPS,&FC,&TNC);

        if (rc != TRUE)
        {
            w_log (LL_ERR, "GetDiskFreeSpace error: return code = %lu", GetLastError());
            /* return freeSpace;		    Assume enough disk space */
        }
        else
        {
            /* return (unsigned long) (BPS * SPC * FC / 1024); */
            if (BPS * SPC >= 1024)
                /* Note: 4 TiB barrier */
                freeSpace = ((BPS * SPC / 1024l) * FC);
            else
                freeSpace = (FC / (1024 / (BPS * SPC)));
        }
    }
    return freeSpace;
}
#elif defined(__OS2__) && !defined(__EMX__)

#ifdef __WATCOMC__
    #define __IBMC__ 0
    #define __IBMCPP__ 0
#endif

#define INCL_DOS
#include <os2.h>


unsigned long husky_GetDiskFreeSpace (const char *path)
{
    FSALLOCATE fsa;
    unsigned long disknum = 0;
    APIRET rc;

    if (isalpha (path[0]) && path[1] == ':')
        disknum = toupper (path[0]) - 'A' + 1;

    rc = DosQueryFSInfo (disknum, /* Drive number            */
                         FSIL_ALLOC,	    /* Level 1 allocation info */
                         (PVOID) & fsa,	/* Buffer                  */
                         sizeof (fsa));	/* Size of buffer          */

    if (rc)
    {
        w_log (LL_ERR, "DosQueryFSInfo error: return code = %u", rc);
        return unsigned_long_max;		    /* Assume enough disk space */
    }
    else
    {
        /* return fsa.cSectorUnit * fsa.cUnitAvail * fsa.cbSector; */
        if (fsa.cSectorUnit * fsa.cbSector >= 1024)
            return fsa.cUnitAvail * (fsa.cSectorUnit * fsa.cbSector / 1024);
        else
            return fsa.cUnitAvail / (1024 / (fsa.cSectorUnit * fsa.cbSector));
    }
}

#elif defined(__UNIX__) || defined(__EMX__)
/*
   This was taken from ifmail, and modified a bit for binkd -- mff, 1997

   Copyright (c) 1993-1995 Eugene G. Crosser

   ifcico is a FidoNet(r) compatible mailer for U*IX platforms.

   You may do virtually what you wish with this software, as long as the
   explicit reference to its original author is retained:

   Eugene G. Crosser <crosser@pccross.msk.su>, 2:5020/230@FidoNet

   THIS SOFTWARE IS PROVIDED AS IS AND COME WITH NO WARRANTY OF ANY KIND,
   EITHER EXPRESSED OR IMPLIED.  IN NO EVENT WILL THE COPYRIGHT HOLDER BE
   LIABLE FOR ANY DAMAGES RESULTING FROM THE USE OF THIS SOFTWARE.
 */


#if defined(HAS_SYS_STATFS_H) || defined(HAS_SYS_STATVFS_H) || defined(HAS_SYS_VFS_H) || defined(HAS_SYS_MOUNT_H)
unsigned long husky_GetDiskFreeSpace (const char *path)
{
#if defined(HAS_SYS_STATVFS_H) || defined(HAS_SYS_VFS_H)
    struct statvfs sfs;
    /* f_bavail is in f_frsize units in statvfs */
#define B_SIZE f_frsize

    if (statvfs (path, &sfs) != 0)
#else
    struct statfs sfs;
#define B_SIZE f_bsize

    if (statfs (path, &sfs) != 0)
#endif
    {
        w_log (LL_ERR, "cannot statfs \"%s\", assume enough space", path);
        return unsigned_long_max;
    }
    else
    {
        if (sfs.B_SIZE >= 1024)
            return ((sfs.B_SIZE / 1024l) * sfs.f_bavail);
        else
            return (sfs.f_bavail / (1024l / sfs.B_SIZE));
    }
}

#else
unsigned long husky_GetDiskFreeSpace (const char *path)
{
    w_log (LL_WARN, "warning: free space doesn't checked in %s",path);
    return unsigned_long_max;
}

#endif


#elif defined(__MSC__) || defined(__DJGPP__) /* alternate variand for DJGPP with DOS Fn's error check */

unsigned long husky_GetDiskFreeSpace (const char *path)
{
    int diskno;
    struct _diskfree_t df;

    if(!path || !*path) return 0l;

    diskno = toupper(*path) - '@';     /* 1="A:", 2="B:", 3="C:" ...*/
    if(diskno<0 || diskno >25) return 0;        /* illegal diskno */
    if(_dos_getdiskfree(diskno, &df)) return 0; /* invalid disk */

#ifdef TEST
    printf("diskno=%i\n",diskno);
    printf("df.avail_clusters=%x\n", df.avail_clusters);
    printf("df.sectors_per_cluster=%x\n",   df.sectors_per_cluster);
    printf("df.bytes_per_sector=%x\n",  df.bytes_per_sector);
#endif
    if (df.sectors_per_cluster * df.bytes_per_sector >= 1024)
        return ((df.sectors_per_cluster * df.bytes_per_sector / 1024l) * df.avail_clusters);
    else
        return (df.avail_clusters / (1024l / (df.sectors_per_cluster * df.bytes_per_sector)));
}

#elif defined(__DJGPP__) /* without DOS Fn's error ckeck */

unsigned long husky_GetDiskFreeSpace (const char *path)
{
    int diskno;
    struct dfree df;

    if(!path || !*path) return 0l;

    diskno = toupper(*path) - '@';     /* 1="A:", 2="B:", 3="C:" ...*/
    if(diskno<0 || diskno >25) return 0;        /* illegal diskno */
    getdfree(diskno, &df);

#ifdef TEST
    printf("diskno=%i\n",diskno);
    printf("df.df_avail=%x\n", df.df_avail);
    printf("df.df_sclus=%x\n",   df.df_sclus);
    printf("df.df_bsec=%x\n",  df.df_bsec);
#endif
    if (df.df_bsec * df.df_sclus >= 1024)
        return ((df.df_bsec * df.df_sclus / 1024l) * df.df_avail);
    else
        return (df.df_avail / (1024l / (df.df_bsec * df.df_sclus)));
}

#elif defined(__DOS__) /* call int 0x21 DOS Fn 0x36 */

unsigned long husky_GetDiskFreeSpace (const char *path)
{
    int diskno;
    union REGS in, out;

    if(!path || !*path) return 0l;

    diskno = toupper(*path) - '@';     /* 1="A:", 2="B:", 3="C:" ...*/
    if(diskno<1 || diskno >25) return 0;        /* illegal diskno */

    /* [DOS Fn 36H: Get Disk Free Space]
     *  Returns: AX    ffffH   if DL was an invalid drive number
     *                 else    sectors per cluster if no error
     *           BX    available clusters (allocation units)
     *           CX    bytes per sector (usually 512)
     *           DX    total clusters on the disk
     */

    in.h.ah=0x36;   /* DOS Fn 36H: Get Disk Free Space */
    in.h.dl=diskno;
#if defined(__DPMI__) && !defined(__DJGPP__)
    int386(0x21, &in, &out);
#ifdef TEST
    printf("out.x.eax=%lx available clusters\n",out.x.eax);
    printf("out.x.ebx=%lx sectors per cluster (0xffff = error)\n",out.x.ebx);
    printf("out.x.ecx=%lx bytes per sector\n",out.x.ecx);
#endif
    if((out.x.eax & 0xffff) == 0xffff )
        return 0; /* bad drive number in DL */
    return (out.x.eax & 0xffff) * (out.x.ebx & 0xffff) * (out.x.ecx & 0xffff);
#else
    int86(0x21,&in,&out);
#ifdef TEST
#ifdef __DJGPP__
    printf("out.x.bx=%lx available clusters\n",out.x.bx);
    printf("out.x.ax=%lx sectors per cluster (0xffff = error)\n",out.x.ax);
    printf("out.x.cx=%lx bytes per sector\n",out.x.cx);
#else
    printf("out.x.bx=%x available clusters\n",out.x.bx);
    printf("out.x.ax=%x sectors per cluster (0xffff = error)\n",out.x.ax);
    printf("out.x.cx=%x bytes per sector\n",out.x.cx);
#endif
#endif
    if((out.x.ax & 0xffff) == 0xffff )
        return 0; /* bad drive number in DL */
    return (out.x.ax & 0xffff) * out.x.bx * out.x.cx / 1024; /* OK */
#endif

}
#else

#error "unknown system!"

#endif

#ifdef TEST

int main(int argc, char**argv)
{
    unsigned long f;
    char *sdisk="c:\\";

    if(argc>1) *sdisk = argv[1][0];

    f = husky_GetDiskFreeSpace(sdisk);
    printf("Free space on %s is  %lu\n", sdisk, f);

    return 0;
}
#endif
