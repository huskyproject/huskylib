
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "fscmn.h"
#include "fidoconf/common.h"
#include <smapi/compiler.h>

/*
#ifdef _MAKE_DLL_MVC_
#	include <Winbase.h>
#	undef hs_addr
#endif
*/  

#ifdef UNIX
#define MOVE_FILE_BUFFER_SIZE 128000
#else
#define MOVE_FILE_BUFFER_SIZE 16384
#endif

int move_file(const char *from, const char *to)
{
#if !(defined(USE_SYSTEM_COPY) && (defined(__NT__) || defined(OS2)))
    int rc;
    char *buffer;
    size_t read;
    FILE *fin, *fout;
	

    rc = rename(from, to);
    if (!rc) {               /* rename succeeded. fine! */
#elif defined(__NT__) && defined(USE_SYSTEM_COPY)
    int rc;
    rc = MoveFile(from, to);
    if (rc == TRUE) {
#elif defined(OS2) && defined(USE_SYSTEM_COPY)
    USHORT rc;
    rc = DosMove((PSZ)from, (PSZ)to);
    if (!rc) {
#endif
      return 0;
    }

#if !(defined(USE_SYSTEM_COPY) && (defined(__NT__) || defined(OS2)))
    /* Rename did not succeed, probably because the move is accross
       file system boundaries. We have to copy the file. */

    buffer = malloc(MOVE_FILE_BUFFER_SIZE);
    if (buffer == NULL)	return -1;

    fin = fopen(from, "rb");
    if (fin == NULL) { nfree(buffer); return -1; }

    fout = fopen(to, "wb");
    if (fout == NULL) { nfree(buffer); fclose(fin); return -1; }

    while ((read = fread(buffer, 1, MOVE_FILE_BUFFER_SIZE, fin)) > 0)
    {
	if (fwrite(buffer, 1, read, fout) != read)
	{
	    fclose(fout); fclose(fin); remove(to); nfree(buffer);
	    return -1;
	}
    }

    if (ferror(fout) || ferror(fin))
    {
	fclose(fout);
	fclose(fin);
	nfree(buffer);
	remove(to);
	return -1;
    }

    fclose(fout);
    fclose(fin);
    nfree(buffer);
#elif defined (__NT__) && defined(USE_SYSTEM_COPY)
    rc = CopyFile(from, to, FALSE);
    if (rc == FALSE) {
      remove(to);
      return -1;
    }       
#elif defined (OS2) && defined(USE_SYSTEM_COPY)
    rc = DosCopy((PSZ)from, (PSZ)to, 1);
    if (rc) {
      remove(to);
      return -1;
    }       
#endif
    remove(from);
    return 0;
}

	
int copy_file(const char *from, const char *to)
{
#if !(defined(USE_SYSTEM_COPY) && (defined(__NT__) || defined(OS2)))
    char *buffer;
    size_t read;
    FILE *fin, *fout;
	

    /* Rename did not succeed, probably because the move is accross
       file system boundaries. We have to copy the file. */

    buffer = malloc(MOVE_FILE_BUFFER_SIZE);
    if (buffer == NULL)	return -1;

    fin = fopen(from, "rb");
    if (fin == NULL) { nfree(buffer); return -1; }

    fout = fopen(to, "wb");
    if (fout == NULL) { nfree(buffer); fclose(fin); return -1; }

    while ((read = fread(buffer, 1, MOVE_FILE_BUFFER_SIZE, fin)) > 0)
    {
	if (fwrite(buffer, 1, read, fout) != read)
	{
	    fclose(fout); fclose(fin); remove(to); nfree(buffer);
	    return -1;
	}
    }

    if (ferror(fout) || ferror(fin))
    {
	fclose(fout);
	fclose(fin);
	nfree(buffer);
        remove(to);
	return -1;
    }

    fclose(fout);
    fclose(fin);
    nfree(buffer);
#elif defined (__NT__) && defined(USE_SYSTEM_COPY)
    int rc = CopyFile(from, to, FALSE);
    if (rc == FALSE) {
      remove(to);
      return -1;
    }       
#elif defined (OS2) && defined(USE_SYSTEM_COPY)
    USHORT rc = DosCopy((PSZ)from, (PSZ)to, 1);
    if (rc) {
      remove(to);
      return -1;
    }       
#endif
    return 0;
}

char    *GetFilenameFromPathname(char* pathname)
{
    char *fname = strrchr(pathname,PATH_DELIM);
    if(fname)
        fname++;
    else
        fname = pathname;
    return fname;
}

char *makeMsgbFileName(ps_fidoconfig config, char *s) {
    // allowed symbols: 0..9, a..z, A..Z, ".,!@#$^()~-_{}[]"
    static char defstr[]="\"*/:;<=>?\\|%`'&+"; // not allowed
    char *name=NULL, *str;

    if (config->notValidFNChars) str = config->notValidFNChars;
    else str = defstr;

    while (*s) {
	if (strchr(str,*s)) xscatprintf(&name,"%%%x", *s);
	else xscatprintf(&name, "%c", *s);
	s++;
    }

    return name;
}

int NCreateOutboundFileName(ps_fidoconfig config, s_link *link, e_flavour prio, e_pollType typ)
{
   int fd; // bsy file for current link
   int nRet = 0;
   char *name=NULL, *sepDir=NULL, limiter=PATH_DELIM, *tmpPtr;
   e_bundleFileNameStyle bundleNameStyle = eUndef;

   if (link->linkBundleNameStyle!=eUndef) bundleNameStyle=link->linkBundleNameStyle;
   else if (config->bundleNameStyle!=eUndef) bundleNameStyle=config->bundleNameStyle;
   
   if (bundleNameStyle != eAmiga) {
	   if (link->hisAka.point) xscatprintf(&name, "%08x.", link->hisAka.point);
	   else xscatprintf(&name, "%04x%04x.", link->hisAka.net, link->hisAka.node);
   } else {
	   xscatprintf(&name, "%u.%u.%u.%u.", link->hisAka.zone,
				   link->hisAka.net, link->hisAka.node, link->hisAka.point);
   }

   if (typ != REQUEST) {
	   switch (prio) {
	   case crash :     xstrcat(&name, "c");
		   break;
	   case hold :      xstrcat(&name, "h");
		   break;
	   case direct :    xstrcat(&name, "d");
		   break;
	   case immediate : xstrcat(&name, "i");
		   break;
	   case normal :    xstrcat(&name, (typ==PKT) ? "o" : "f");
		   break;
	   }
   } else xstrcat(&name, "req");

   switch (typ) {
   case PKT :     xstrcat(&name, "ut");
	   break;
   case FLOFILE : xstrcat(&name, "lo");
	   break;
   case REQUEST :
	   break;
   }

   // create floFile
   xstrcat(&link->floFile, config->outbound);

   // add suffix for other zones
   if (link->hisAka.zone != config->addr[0].zone && bundleNameStyle != eAmiga) {
	   link->floFile[strlen(link->floFile)-1]='\0';
	   xscatprintf(&link->floFile, ".%03x%c", link->hisAka.zone, limiter);
   }

   if (link->hisAka.point && bundleNameStyle != eAmiga)
	   xscatprintf(&link->floFile, "%04x%04x.pnt%c",
				   link->hisAka.net, link->hisAka.node, limiter);
   
   _createDirectoryTree(link->floFile); // create directoryTree if necessary
   xstrcat(&link->bsyFile, link->floFile);
   xstrcat(&link->floFile, name);

   // separate bundles

   if (config->separateBundles && (bundleNameStyle!=eAmiga || (bundleNameStyle==eAmiga && link->packerDef==NULL))) {

       xstrcat(&sepDir, link->bsyFile);
       if (bundleNameStyle==eAmiga) 
	   xscatprintf(&sepDir, "%u.%u.%u.%u.sep%c", 
		       link->hisAka.zone, link->hisAka.net,
		       link->hisAka.node ,link->hisAka.point, limiter);
       else if (link->hisAka.point) xscatprintf(&sepDir, "%08x.sep%c", 
						link->hisAka.point, limiter);
       else xscatprintf(&sepDir, "%04x%04x.sep%c", link->hisAka.net,
			link->hisAka.node, limiter);

       _createDirectoryTree(sepDir);
       nfree(sepDir);
   }

   // create bsyFile
   if ((tmpPtr=strrchr(name, '.')) != NULL) *tmpPtr = '\0';
   xstrscat(&link->bsyFile, name, ".bsy", NULL);
   nfree(name);

   // maybe we have session with this link?
   if ( (fd=open(link->bsyFile, O_CREAT | O_RDWR | O_EXCL, S_IREAD | S_IWRITE)) < 0 ) {
#if !defined(__WATCOMC__)	   
	   int save_errno = errno;
	   
	   if (save_errno != EEXIST) {
		   w_log('7', "cannot create *.bsy file \"%s\" for %s (errno %d)\n", link->bsyFile, link->name, (int)save_errno);
         nRet = -1;
		   
	   } else {
#endif
		   w_log('7', "link %s is busy.", aka2str(link->hisAka));
		   nfree(link->floFile);
		   nfree(link->bsyFile);
		   nRet = 1;
#if !defined(__WATCOMC__)	   
	   }
#endif
   } else  {
      close(fd);
      nRet = 0;
   }
   return nRet;
}

int needUseFileBoxForLink (ps_fidoconfig config, s_link *link)
{
    char limiter=PATH_DELIM;
    char *bsyFile = NULL;
    e_bundleFileNameStyle bundleNameStyle = eUndef;

    /* link->useFileBox means:
     * 0 - unknown, need to check
     * 1 - don't use
     * 2 - use box
     */

    if (link->useFileBox == 1) return 0; // Don't use
    if (link->useFileBox == 2) return 1; // Use

    // link->useFileBox == 0 -> still don't know

    if ( (link->fileBox==NULL && config->fileBoxesDir==NULL) ||
         (theApp.module == M_HTICK  && !link->tickerPackToBox)
       )
    {
        link->useFileBox = 1;
        return 0;
    }

    if (link->fileBoxAlways) {
	link->useFileBox = 2;
	return 1;
    }

    // check if can we use outbound
    xstrcat(&bsyFile, config->outbound);

    // add suffix for other zones
    if (link->hisAka.zone != config->addr[0].zone && bundleNameStyle != eAmiga) {
	bsyFile[strlen(bsyFile)-1]='\0';
	xscatprintf(&bsyFile, ".%03x%c", link->hisAka.zone, limiter);
    }

    if (link->hisAka.point && bundleNameStyle != eAmiga)
	xscatprintf(&bsyFile, "%04x%04x.pnt%c",
		    link->hisAka.net, link->hisAka.node, limiter);
   
    _createDirectoryTree(bsyFile); // create directoryTree if necessary

    if (link->linkBundleNameStyle!=eUndef) bundleNameStyle=link->linkBundleNameStyle;
    else if (config->bundleNameStyle!=eUndef) bundleNameStyle=config->bundleNameStyle;
   
    if (bundleNameStyle != eAmiga) {
	if (link->hisAka.point) xscatprintf(&bsyFile, "%08x", link->hisAka.point);
	else xscatprintf(&bsyFile, "%04x%04x", link->hisAka.net, link->hisAka.node);
    } else {
	xscatprintf(&bsyFile, "%u.%u.%u.%u", link->hisAka.zone,
		    link->hisAka.net, link->hisAka.node, link->hisAka.point);
    }

    xstrscat(&bsyFile, ".bsy", NULL);
    
    if (fexist(bsyFile)) {
	link->useFileBox = 2;
    } else {
	// link is not busy, use outrbound
	link->useFileBox = 1;
    }

    nfree (bsyFile);

    return link->useFileBox - 1;
}

char *makeFileBoxName (ps_fidoconfig config, s_link *link)
{
    char *name=NULL;

    xscatprintf (&name, "%s%d.%d.%d.%d%s%c",
		 config->fileBoxesDir,
		 link->hisAka.zone,
		 link->hisAka.net,
		 link->hisAka.node,
		 link->hisAka.point,
		 (link->echoMailFlavour==hold) ? ".h" : "",
		 PATH_DELIM);
    return name;
}

void fillCmdStatement(char *cmd, const char *call, const char *archiv, const char *file, const char *path)
{
    const char *start, *tmp, *add;
    
    *cmd = '\0';  start = NULL;
    for (tmp = call; (start = strchr(tmp, '$')) != NULL; tmp = start + 2) {
        switch(*(start + 1)) {
        case 'a': add = archiv; break;
        case 'p': add = path; break;
        case 'f': add = file; break;
        default:
            strncat(cmd, tmp, (size_t) (start - tmp + 1));
            start--; continue;
        };
        strncat(cmd, tmp, (size_t) (start - tmp));
        strcat(cmd, add);
    };
    strcat(cmd, tmp);
}

char *changeFileSuffix(char *fileName, char *newSuffix) {

    int   i = 1;
    char  buff[255];

    char *beginOfSuffix = strrchr(fileName, '.')+1;
    char *newFileName;
    int  length = strlen(fileName)-strlen(beginOfSuffix)+strlen(newSuffix);

    newFileName = (char *) smalloc((size_t) length+1+2);
    memset(newFileName, '\0',length+1+2);
    strncpy(newFileName, fileName, length-strlen(newSuffix));
    strcat(newFileName, newSuffix);

#ifdef DEBUG_HPT
    printf("old: %s      new: %s\n", fileName, newFileName);
#endif

    while (fexist(newFileName) && (i<255)) {
	sprintf(buff, "%02x", i);
	beginOfSuffix = strrchr(newFileName, '.')+1;
	strncpy(beginOfSuffix+1, buff, 2);
	i++;
    }

    if (!fexist(newFileName)) {
	rename(fileName, newFileName);
	return newFileName;
    } else {
	w_log('9', "Could not change suffix for %s. File already there and the 255 files after", fileName);
	nfree(newFileName);
	return NULL;
    }
}
