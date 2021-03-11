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
/* standard headers */
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
/* huskylib: compiler.h */
#include <compiler.h>
/* compiler-dependent headers */
/* huskylib headers */
#define DLLEXPORT
#include <huskyext.h>
/* huskylib headers */
#include <huskylib.h>
#define SYSLOG_NAMES
#include <syslogp.h>
/***  Declarations & defines  ***********************************************/
static s_log * husky_log = NULL;
static char * logFileDir = NULL;
static int logEchoToScreen;
static char * logLevels       = NULL;
static char * screenLogLevels = NULL;
static char * logDateFormat   = NULL;

#ifdef HAVE_SYSLOG
static int syslogFacility     = 0;
#endif

/***  Implementation  *******************************************************/
void initLogSyslog(char * ext_program,
                   char * ext_logFileDir,
                   int ext_syslogFacility,
                   int ext_logEchoToScreen,
                   char * ext_logLevels,
                   char * ext_screenLogLevels)
{
#ifdef HAVE_SYSLOG

    if(ext_program && ext_syslogFacility)
    {
        syslogFacility  = ext_syslogFacility;
        logLevels       = ext_logLevels;
        screenLogLevels = ext_screenLogLevels;
        logEchoToScreen = ext_logEchoToScreen;
        openlog(ext_program,
                LOG_PID | LOG_ODELAY | (ext_logEchoToScreen ? LOG_PERROR : 0),
                syslogFacility);
    }

#else
    unused(ext_syslogFacility);
    unused(ext_program);
#endif

    if(ext_logFileDir)
    {
        initLog(ext_logFileDir, ext_logEchoToScreen, ext_logLevels, ext_screenLogLevels);
    }
}

void initLog(char * ext_logFileDir,
             int ext_logEchoToScreen,
             char * ext_logLevels,
             char * ext_screenLogLevels)
{
    logFileDir      = ext_logFileDir;
    logEchoToScreen = ext_logEchoToScreen;
    logLevels       = ext_logLevels;
    screenLogLevels = ext_screenLogLevels;
    return;
}

void setLogDateFormat(char * _logDateFormat)
{
    logDateFormat = _logDateFormat;
}

s_log * openLog(char * fileName, char * appN)
{
    time_t currentTime;
    struct tm * locTime;
    char * pathname = NULL;
    int error       = 0;

    if(!fileName || !fileName[0])
    {
        fprintf(stderr, "Logfile not defined, log into screen instead\n");
        return NULL;
    }

    if(strchr(fileName, '\\') || strchr(fileName, '/'))
    {
        pathname = fileName;
    }
    else
    {
        /* filename without path, construct full pathname  */
        if(logFileDir && logFileDir[0])
        {
            xstrscat(&pathname, logFileDir, fileName, (char *)NULLP);
        }
        else
        {
            fprintf(stderr, "LogFileDir not defined, log into screen instead\n");
            return NULL;
        }
    }

    husky_log = (s_log *)smalloc(sizeof(s_log));
    memset(husky_log, '\0', sizeof(s_log));
    husky_log->logFile = fopen(pathname, "a");

    if(NULL == husky_log->logFile)
    {
        fprintf(stderr, "Cannot open log '%s': %s\n", fileName, strerror(errno));
        nfree(husky_log);

        if(pathname != fileName)
        {
            nfree(pathname);
        }

        return NULL;
    } /* endif */

    husky_log->isopen = 1;
    /* copy all informations */
    xstrcat(&husky_log->appName, appN);

    if(logLevels != NULL)
    {
        xstrcat(&husky_log->keysAllowed, logLevels);
    }
    else
    {
        xstrcat(&husky_log->keysAllowed, DefaultLogLevels);
    }

    if(logEchoToScreen)
    {
        if(screenLogLevels != NULL)
        {
            xstrcat(&husky_log->keysPrinted, screenLogLevels);
        }
        else
        {
            xstrcat(&husky_log->keysPrinted, DefaultScreenLogLevels);
        }
    } /* else: quiet mode, keysPrinted is empty */

    husky_log->logEcho = logEchoToScreen;
    /* make first line of log */
    currentTime = time(NULL);
    locTime     = localtime(&currentTime);

    if(logDateFormat)
    {
        char logDate[64];
        int i, logDateLen = (int)strftime(logDate, 63, logDateFormat, locTime);

        for(i = 0; i < logDateLen; i++)
        {
            logDate[i] = '-';
        }
        logDate[i] = '\0';
        error      = (fputs("--", husky_log->logFile) == EOF);

        if(!error)
        {
            error = (fputs(logDate, husky_log->logFile) == EOF);

            if(!error)
            {
                error = (fputs(" ", husky_log->logFile) == EOF);
            }
        }
    }
    else
    {
        error = (fputs("----------  ", husky_log->logFile) == EOF);
    }

    if(!error)
    {
        error =
            (fprintf(husky_log->logFile, "%3s %02u %3s %04u, %s\n",
                     weekday_ab[(size_t)(locTime->tm_wday)],
                     (unsigned)(locTime->tm_mday),
                     months_ab[(size_t)(locTime->tm_mon)],
                     (unsigned)(locTime->tm_year + 1900),
                     husky_log->appName) == EOF);
    }

    if(error)
    {
        fprintf(stderr, "Can't write to log file \"%s\": %s", pathname, strerror(errno));
    }

    if(pathname != fileName)
    {
        nfree(pathname);
    }

    return husky_log;
} /* openLog */

void closeLog()
{
    if(husky_log != NULL)
    {
        if(husky_log->isopen)
        {
            fprintf(husky_log->logFile, "\n");
            fclose(husky_log->logFile);
        } /* endif */

        nfree(husky_log->appName);
        nfree(husky_log->keysAllowed);
        nfree(husky_log->keysPrinted);
        nfree(husky_log);
    }

#ifdef HAVE_SYSLOG
    closelog();
#endif
}

void w_log(char key, char * logString, ...)
{
    time_t currentTime;
    struct tm * locTime;
    va_list ap;
    register char log = 0, screen = 0;

    if(husky_log)
    {
        if(husky_log->isopen && strchr(husky_log->keysAllowed, key))
        {
            log = 1;
        }

        if(husky_log->logEcho && strchr(husky_log->keysPrinted, key))
        {
            screen = 1;
        }

        if(!husky_log->isopen && key == LL_CRIT)
        {
            screen = 1;                                   /* Critical error to stderr if not
                                                             logged */
        }
    }
    else
    {
        screen = 1;
    }

    if(log || screen)
    {
        char logDate[64];
        size_t logDateLen;
        currentTime = time(NULL);
#if 1
        locTime    = localtime(&currentTime);
        logDateLen = strftime(logDate,
                              63,
                              logDateFormat ? logDateFormat : "%H:%M:%S ",
                              locTime);
        logDate[logDateLen] = '\0';
#endif

        if(log)
        {
            register int error = 0;
#if 1
            error = (EOF == fprintf(husky_log->logFile, "%c %s ", key, logDate));
#else
            error =
                (EOF ==
                 fprintf(husky_log->logFile, "%c %02u:%02u:%02u  ", key, locTime->tm_hour,
                         locTime->tm_min, locTime->tm_sec));
#endif

            if(!error)
            {
                va_start(ap, logString);
                error = (0 >= vfprintf(husky_log->logFile, logString, ap));
                va_end(ap);

                if(!error)
                {
                    error = (EOF == putc('\n', husky_log->logFile));
                }

                if(!error)
                {
                    error = (EOF == fflush(husky_log->logFile));
                }
            }

            if(error)
            {
                fprintf(stderr, "!!! Log writing attempt has caused an error: %s",
                        strerror(errno));
                screen = 1;
            }
        }

        if(screen)
        {
#if 1
            printf("%c %s  ", key, logDate);
#else
            printf("%c %02u:%02u:%02u  ", key, locTime->tm_hour, locTime->tm_min, locTime->tm_sec);
#endif
            va_start(ap, logString);
            vprintf(logString, ap);
            va_end(ap);
            putchar('\n');
        }
    }
} /* w_log */

#ifdef __NT__

LONG WINAPI UExceptionFilter(struct _EXCEPTION_POINTERS * ExceptionInfo)
{
    char * ErrorMsg;

    /* avoid recursive call of the exception filter */
    SetUnhandledExceptionFilter(UnhandledExceptionFilter);

    switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
    {
        case EXCEPTION_ACCESS_VIOLATION:
            ErrorMsg = "Access violation";
            break;

        case EXCEPTION_DATATYPE_MISALIGNMENT:
            ErrorMsg = "Datatype misalignment";
            break;

        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            ErrorMsg = "Array bound exceeded";
            break;

        case EXCEPTION_FLT_DENORMAL_OPERAND:
            ErrorMsg = "Float: denormal operand";
            break;

        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            ErrorMsg = "Float: divide by zero";
            break;

        case EXCEPTION_FLT_INEXACT_RESULT:
            ErrorMsg = "Float: inexact result";
            break;

        case EXCEPTION_FLT_INVALID_OPERATION:
            ErrorMsg = "Float: invalid operation";
            break;

        case EXCEPTION_FLT_OVERFLOW:
            ErrorMsg = "Float: overflow";
            break;

        case EXCEPTION_FLT_STACK_CHECK:
            ErrorMsg = "Float: stack check";
            break;

        case EXCEPTION_FLT_UNDERFLOW:
            ErrorMsg = "Float: underflow";
            break;

        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            ErrorMsg = "Divide by zero";
            break;

        case EXCEPTION_INT_OVERFLOW:
            ErrorMsg = "Overflow";
            break;

        case EXCEPTION_PRIV_INSTRUCTION:
            ErrorMsg = "Priveleged instruction";
            break;

        case EXCEPTION_IN_PAGE_ERROR:
            ErrorMsg = "Page error";
            break;

        case EXCEPTION_ILLEGAL_INSTRUCTION:
            ErrorMsg = "Illegal instruction";
            break;

        case EXCEPTION_STACK_OVERFLOW:
            ErrorMsg = "Stack overflow";
            break;

        case EXCEPTION_INVALID_DISPOSITION:
            ErrorMsg = "Invalid disposition";
            break;

        case EXCEPTION_GUARD_PAGE:
            ErrorMsg = "Guard page";
            break;

            /* gcc 2.95.2 in Mingw32 knows nothing about this */
#ifndef __MINGW32__
        case EXCEPTION_INVALID_HANDLE:
            ErrorMsg = "Invalid handle";
            break;

#endif
        default:
            ErrorMsg = "Unknown error";
    } /* switch */
    {
        char sCode[11];
        sprintf((char* const)sCode, "0x%08x",
                ExceptionInfo->ExceptionRecord->ExceptionCode);
        w_log(LL_CRIT,
              "Exception %s (%s) at address %p",
              (char *)sCode,
              ErrorMsg,
              ExceptionInfo->ExceptionRecord->ExceptionAddress);
    }
    exit(1);
 /*   return 0;  compiler paranoia */
} /* UExceptionFilter */

#endif /* ifdef __NT__ */
