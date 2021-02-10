/* $Id$
 *  Provides log file maintnance routines
 *
 *  Compiled from hpt/log & htick/log
 *  by Stas Degteff <g@grumbler.org>, 2:5080/102@fidonet
 *
 *  Portions copyright (C) Matthias Tichy
 *                         Fido:     2:2433/1245 2:2433/1247 2:2432/605.14
 *                         Internet: mtt@tichy.de
 *  Portions copyright (C) Max Levenkov
 *                         Fido:     2:5000/117
 *                         Internet: sackett@mail.ru
 *  Portions copyright (C) Gabriel Plutzar
 *                         Fido:     2:31/1
 *                         Internet: gabriel@hit.priv.at
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

#ifndef HUSKY_LOG_H
#define HUSKY_LOG_H
/* standard headers */
#include <stdio.h>
/* huskylib: compiler.h */
#include "compiler.h"
/* huskylib headers */
#include "huskyext.h"
/***  Declarations & defines  ***********************************************/

#define DefaultLogLevels "1234567890ABCDEF"
#define DefaultScreenLogLevels DefaultLogLevels

#define LL_START '1'         /* Program start */
#define LL_STOP '1'          /* Program end */
#define LL_DUPE '2'          /* Dupecheck */
#define LL_DUPECHECK LL_DUPE
#define LL_LINKING '3'       /* Link messagebase */
#define LL_SCANNING '4'      /* Scan messagebase */
#define LL_POSTING '5'       /* Post message */
#define LL_CARBON '5'        /* Carbon operations */
#define LL_EXEC '6'          /* system() & exec() */
#define LL_BUNDLE '7'        /* Bundle operations */
#define LL_OUTBOUND '7'      /* Outbound operations */
#define LL_FLO '7'           /* ?LO file operations */
#define LL_UT '7'            /* ?UT file operations */
#define LL_PKT '7'           /* PKT file operations */
#define LL_LINK '7'          /* Link operation */
#define LL_LINKBEGIN LL_LINK     /* Link operations begin */
#define LL_LINKEND LL_LINK       /* Link operations end */
#define LL_FREQ '7'          /* FREQ processing */
#define LL_ROUTE '7'         /* Routing */
#define LL_FROUTE '7'        /* File routing */
#define LL_ATTACH '7'        /* File attach */
#define LL_TIC '7'           /* TIC file parse/create/etc. */
#define LL_MSGPACK '7'       /* Message packing */
#define LL_AREAFIX '8'       /* areafix & filefix operations */
#define LL_RELINK '8'        /* send relink message */
#define LL_AUTOCREATE '8'    /* area auto cleate */
#define LL_CRIT '9'          /* Critical error: exit */
#define LL_FLAG '0'          /* Create/remove/test flag */
#define LL_BUSY '0'          /* .BSY exist */
#define LL_LINKBUSY LL_BUSY
#define LL_BSY LL_BUSY

#define LL_ERROR 'A'         /* Trivial error: continue */
#define LL_ERR LL_ERROR
#define LL_WARN 'B'          /* Warning */
#define LL_WARNING LL_WARN
#define LL_ALERT 'B'         /* Alert */
#define LL_SECURITY 'B'      /* Security alert */
#define LL_INFO 'C'          /* Information messages */
#define LL_STAT 'D'          /* Statistics */
#define LL_SUMMARY 'E'       /* Summary */
#define LL_PRG 'F'           /* Program name */
#define LL_SENT 'G'          /* Message sent */
#define LL_ENCODE 'H'        /* Encode file/message */
#define LL_DECODE 'H'        /* Decode file/message */
#define LL_RECODE 'H'        /* Recoding tables (codepage translations) */

#define LL_MSGID 'I'         /* Generate/check MSGID */
#define LL_ECHOMAIL 'J'      /* Echomail phase */
#define LL_FILEBOX 'K'       /* Filebox phase/operations */
#define LL_BOX LL_FILEBOX
#define LL_NETMAIL 'L'       /* Netmail phase */
#define LL_CREAT 'M'         /* Create file */
#define LL_DEL 'N'           /* Delete file */
#define LL_FILE 'O'          /* Other file operations (read, write, seek, ...) */
#define LL_DIR 'P'           /* Directory create/delete/etc */
#define LL_PATH LL_DIR
#define LL_EMAIL 'Q'         /* Email parsing or construct */
#define LL_TRUNC 'R'         /* Truncate file */
#define LL_DELETE LL_DEL
#define LL_TRUNCATE LL_TRUNC
#define LL_FILESENT 'S'      /* File sent */
#define LL_FILETEST 'T'      /* Test files (exist, permittions) */
#define LL_FILENAME 'X'      /* Filenames construct */
#define LL_FUNC 'U'          /* Functions calls */
#define LL_LINKPASS 'Y'      /* Link messagebase pass (some phase) */
#define LL_SRCLINE 'Z'       /* Source lines numbers */
#define LL_DEBUG 'a'         /* Debug output */
#define LL_DEBUGA 'a'        /* Debug output */
#define LL_DEBUGB 'b'        /* Debug output */
#define LL_DEBUGC 'c'        /* Debug output */
#define LL_DEBUGD 'd'        /* Debug output */
#define LL_DEBUGE 'e'        /* Debug output */
#define LL_DEBUGF 'f'        /* Debug output */
#define LL_DEBUGG 'g'        /* Debug output */
#define LL_DEBUGH 'h'        /* Debug output */
#define LL_DEBUGI 'i'        /* Debug output */
#define LL_DEBUGJ 'j'        /* Debug output */
#define LL_DEBUGK 'k'        /* Debug output */
#define LL_DEBUGL 'l'        /* Debug output */
#define LL_DEBUGM 'm'        /* Debug output */
#define LL_DEBUGN 'n'        /* Debug output */
#define LL_DEBUGO 'o'        /* Debug output */
#define LL_DEBUGP 'p'        /* Debug output */
#define LL_DEBUGQ 'q'        /* Debug output */
#define LL_DEBUGR 'r'        /* Debug output */
#define LL_DEBUGS 's'        /* Debug output */
#define LL_DEBUGT 't'        /* Debug output */
#define LL_DEBUGU 'u'        /* Debug output */
#define LL_DEBUGV 'v'        /* Debug output */
#define LL_DEBUGW 'w'        /* Debug output */
#define LL_DEBUGX 'x'        /* Debug output */
#define LL_DEBUGY 'y'        /* Debug output */
#define LL_DEBUGZ 'z'        /* Debug output */
#define LL_DEBUGz 'z'        /* Debug output */

#ifdef __cplusplus
extern "C" {
#endif

struct _log
{
    char *        keysAllowed; /*  only log-data with one of these keys will be stored */
    char *        keysPrinted; /*  only log-data with these keys will be printed to screen */
    char *        appName; /*  name of the application which has created this log entry */
    FILE *        logFile; /*  in this logFile */
    unsigned char isopen;  /*  is the log-file open? */
    unsigned int  logEcho; /*  echo log to screen? */
};

typedef struct _log s_log;
HUSKYEXT void initLog(char * ext_logFileDir,
                      int ext_logEchoToScreen,
                      char * ext_logLevels,
                      char * ext_screenLogLevels);

/*DOC
   Input: fidoconfig variables
   Output: none;
   FZ: copy fidoconfig values into log processing
 */
HUSKYEXT void setLogDateFormat(char * _logDateFormat);

/*DOC
   Input: log date format as strftime() third argument
   Output: none;
   FZ: copy fidoconfig value logDateFormat into log processing
 */
HUSKYEXT s_log * openLog(char * fileName, char * appN);

/*DOC
   Input:  fileName is a valid name for a file.
          appN contains the name of the application.
   Output: openLog returns a pointer to an s_log struct.
   FZ:     openLog fills the s_log struct, opens the logfile and returns the struct
 */
HUSKYEXT void closeLog();

/*DOC
   Input:  log is a pointer to a s_log
   Output: ./.
   FZ:     closes the logFile and frees all mem use by log.
 */
HUSKYEXT void w_log(char key, char * logString, ...);

/*DOC
   Input:  key is the key under which the log-entry will be stored
          logString is the logEntry
   Output: ./.
   FZ:     if the key is in keysAllowed the logString will be written to the log.
 */

#ifdef DEBUG
# define w_dbglog w_log
#else
/* if your compiler supports variadic macros add its recognition here */
/*    vs2005 */
# if (_MSC_VER >= 1400)
#  define w_dbglog(...) ((void)0)
# elif (__WATCOMC__ > 1200)
#  define w_dbglog(...) ((void)0)
# elif (__GNUC__ >= 2)
#  define w_dbglog(args ...) ((void)0)
# else
#  define w_dbglog(exp) ((void)0)
# endif

#endif

#ifdef __NT__
#include <windows.h>
#include <winbase.h>
HUSKYEXT LONG WINAPI UExceptionFilter(struct _EXCEPTION_POINTERS * ExceptionInfo);

#endif

#ifdef __cplusplus
}
#endif

#endif // ifndef HUSKY_LOG_H
