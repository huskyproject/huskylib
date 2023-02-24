/* $Id$ */
#ifndef HUSKY_TYPESIZE_H
#define HUSKY_TYPESIZE_H
/*  */
/*   typesize.h                         written by tom schlangen */
/*   ----------                         modified by TJW */
/*  */
/*   this is an attempt to somewhat reduce problems by unifying */
/*   compiler dependend type sizes. */
/*  */
/*   the basic set/list of unified types required to be present */
/*   for each compiler is: */
/*  */
/*   --------+-----------------+-------+----------------------- */
/*   name    | description     | bytes | range */
/*   --------+-----------------+-------+----------------------- */
/*   CHAR    | signed char     | 1     | -128..127 */
/*   UCHAR   | unsigned char   | 1     | 0..255 */
/*   INT16   | signed word     | 2     | -32768..32767 */
/*   UINT16  | unsigned word   | 2     | 0..65535 */
/*   INT32   | signed dword    | 4     | -2147483648..2147483647 */
/*   UINT32  | unsigned dword  | 4     | 0..4294967295 */
/*   --------+-----------------+-------+----------------------- */
/*  */
/*   besides that, there are some further general purpose types */
/*   with guaranteed (either by ANSI C or by us) sizes/ranges. */
/*   these should be used with care, since beneath their */
/*   guaranteed size they are strictly compiler specific. so if */
/*   you use them, make sure you do so only within the */
/*   guaranteed range. also take care not to use them in byte- */
/*   aligned (`packed') structures, since the size of the re- */
/*   resulting structures may vary from compiler to compiler, */
/*   which may cause hazzard on in certain cases. */
/*  */
/*   --------+------------------------------------------------- */
/*   name    | description */
/*   --------+------------------------------------------------- */
/*   INT     | general purpose compiler specific `signed int'. */
/*           | ANSI C guarantees at least 2 bytes, */
/*           | range -32768..32767 for this type. */
/*   UINT    | general purpose compiler specific `unsigned int', */
/*           | we guarantee at least 2 bytes, */
/*           | range 0..65535 for this type. */
/*   LONG    | general purpose compiler specific `signed long'. */
/*           | ANSI C * guarantees at least 4 bytes, */
/*           | range -2147483648..2147483647 for this type. */
/*   ULONG   | general purpose compiler specific `unsigned long'. */
/*           | we guarantee at least 4 bytes, */
/*           | range 0..4294967295 for this type. */
/*   --------+------------------------------------------------- */
/*  */
/*   the following definition blocks are in alphabetical order */
/*   of the various compilers identification defines. please add */
/*   the definitions for your compiler, if not already present. */
/*  */

#include "compiler.h"

#ifdef __OS2__
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
    #include <os2.h>
#endif

#if defined (__NT__)
#define WIN32_LEAN_AND_MEAN
#define NOMSG
#define NOGDI
    #if !defined (__EMX__)
        #include <windows.h>
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined (USE_PSTDINT_H) && !defined (HAS_STDINT_H)
/* Fall back to legacy code */
/* Obsolete. Check if your platform can benefit from using stdint.h/pstdint.h
 * and correct compiler.h accordingly */

#if defined (__NT__)
typedef   signed short INT16;                   /*  2 byte */
typedef unsigned short UINT16;                  /*  2 byte */
#endif
/*   the EMX/GNU 32bit compilers */

#if defined (__EMX__)
#if !defined (__OS2_H__) && !defined (_OS2_H) && !defined (_WINNT_H)   /*  os2.h defines it
                                                                          already */
typedef   signed char CHAR;                     /*  1 byte */
typedef unsigned char UCHAR;                    /*  1 byte */
#endif
typedef   signed short INT16;                   /*  2 byte */
typedef unsigned short UINT16;                  /*  2 byte */
typedef   signed int   INT32;                   /*  4 byte */
typedef unsigned int   UINT32;                  /*  4 byte */
/*  -------------------------------------------------------------------------- */
#if !defined (__OS2_H__) && !defined (_OS2_H) && !defined (_WINNT_H)   /*  os2.h defines it
                                                                          already */
typedef   signed int  INT;                      /*  4 byte */
typedef unsigned int  UINT;                     /*  4 byte */
typedef   signed long LONG;                     /*  4 byte */
typedef unsigned long ULONG;                    /*  4 byte */
typedef          void VOID;
#endif
#endif /*  #if defined(__EMX__) */

#if (defined (__linux__) && !(defined (__alpha) || defined (__X86_64__))) || \
    (defined (__FreeBSD__) && !(defined (__alpha) || defined (__X86_64__))) || \
    defined (__NetBSD__) || \
    defined (__DJGPP__) || defined (__BEOS__) || defined (__OpenBSD__) || defined (__CYGWIN__) || \
    defined (__QNXNTO__)
typedef   signed char  CHAR;                    /*  1 byte */
typedef unsigned char  UCHAR;                   /*  1 byte */
typedef   signed int   INT32;                   /*  4 byte */
typedef unsigned int   UINT32;                  /*  4 byte */
typedef   signed int   INT;                     /*  4 byte */
typedef unsigned int   UINT;                    /*  4 byte */
typedef   signed long  LONG;                    /*  4 byte */
typedef unsigned long  ULONG;                   /*  4 byte */
typedef          void  VOID;
typedef   signed short INT16;                   /*  2 byte */
typedef unsigned short UINT16;                  /*  2 byte */
/*  -------------------------------------------------------------------------- */
#endif
/* Alpha AXP running Digital Unix (and possibly also Linux/Alpha?)
   This is a 64 bit architecture (AMD64, EM64T) */
#if defined (__alpha) || defined (__X86_64__)
typedef   signed char  CHAR;                    /*  1 byte */
typedef unsigned char  UCHAR;                   /*  1 byte */
typedef   signed short INT16;                   /*  2 byte */
typedef unsigned short UINT16;                  /*  2 byte */
typedef   signed int   INT32;                   /*  4 byte */
typedef unsigned int   UINT32;                  /*  4 byte */
/*  -------------------------------------------------------------------------- */
typedef   signed int  INT;                      /*  4 byte */
typedef unsigned int  UINT;                     /*  4 byte */
typedef   signed long LONG;                     /*  8 byte */
typedef unsigned long ULONG;                    /*  8 byte */
typedef          void VOID;
#endif
/* AIX using xlc or gcc. Unfortunately, the _AIX constant is also defined
   on 64 bit RS/6000 machines. As I do not have such a thing available,
   I do not know how to discern it from a 32 bit one, so the following will
   probably not work there. */

#if defined (_AIX)
typedef   signed char  CHAR;                    /*  1 byte */
typedef unsigned char  UCHAR;                   /*  1 byte */
typedef   signed short INT16;                   /*  2 byte */
typedef unsigned short UINT16;                  /*  2 byte */
typedef   signed int   INT32;                   /*  4 byte */
typedef unsigned int   UINT32;                  /*  4 byte */
/*  -------------------------------------------------------------------------- */
typedef   signed int  INT;                      /*  4 byte */
typedef unsigned int  UINT;                     /*  4 byte */
typedef   signed long LONG;                     /*  4 byte */
typedef unsigned long ULONG;                    /*  4 byte */
typedef          void VOID;
#endif

#if defined (__sun__)
typedef   signed char  CHAR;                    /*  1 byte */
typedef unsigned char  UCHAR;                   /*  1 byte */
typedef   signed short INT16;                   /*  2 byte */
typedef unsigned short UINT16;                  /*  2 byte */
typedef   signed int   INT32;                   /*  4 byte */
typedef unsigned int   UINT32;                  /*  4 byte */
/*  -------------------------------------------------------------------------- */
typedef   signed int  INT;                      /*  4 byte */
typedef unsigned int  UINT;                     /*  4 byte */
typedef   signed long LONG;                     /*  4 byte */
typedef unsigned long ULONG;                    /*  4 byte */
typedef          void VOID;
#endif

#if defined (__hpux)
typedef   signed char  CHAR;                    /*  1 byte */
typedef unsigned char  UCHAR;                   /*  1 byte */
typedef   signed short INT16;                   /*  2 byte */
typedef unsigned short UINT16;                  /*  2 byte */
typedef   signed int   INT32;                   /*  4 byte */
typedef unsigned int   UINT32;                  /*  4 byte */
/*  -------------------------------------------------------------------------- */
typedef   signed int  INT;                      /*  4 byte */
typedef unsigned int  UINT;                     /*  4 byte */
typedef   signed long LONG;                     /*  4 byte */
typedef unsigned long ULONG;                    /*  4 byte */
typedef          void VOID;
#endif
/*  the Borland compiler family - valid for DOS, OS/2 and Win32 versions */

#if defined (__BORLANDC__)
#ifndef __OS2_H__
typedef signed char   CHAR;                     /*  1 byte */
typedef unsigned char UCHAR;                    /*  1 byte */
#endif
typedef signed short   INT16;                   /*  2 byte */
typedef unsigned short UINT16;                  /*  2 byte */
typedef signed long    INT32;                   /*  4 byte */
typedef unsigned long  UINT32;                  /*  4 byte */
/*  -------------------------------------------------------------------------- */
#ifndef __OS2_H__
typedef signed int    INT;                      /*  2/4 byte */
typedef unsigned int  UINT;                     /*  2/4 byte */
typedef signed long   LONG;                     /*  4 byte */
typedef unsigned long ULONG;                    /*  4 byte */
#if !defined (__NT__)
typedef          void VOID;
#endif
#endif
#endif /*  #if defined(__BORLANDC__) */
/*   the IBM 32bit CSet/VAC++ compilers */

#if defined (__IBMC__) || defined (__IBMCPP__)
#ifndef __OS2_H__              /*  os2.h defines it already */
typedef   signed char CHAR;    /*  1 byte */
typedef unsigned char UCHAR;   /*  1 byte */
#endif
typedef   signed short INT16;  /*  2 byte */
typedef unsigned short UINT16; /*  2 byte */
typedef   signed int   INT32;  /*  4 byte */
typedef unsigned int   UINT32; /*  4 byte */
/*  -------------------------------------------------------------------------- */
#ifndef __OS2_H__              /*  os2.h defines it already */
typedef   signed int  INT;     /*  4 byte */
typedef unsigned int  UINT;    /*  4 byte */
typedef   signed long LONG;    /*  4 byte */
typedef unsigned long ULONG;   /*  4 byte */
typedef          void VOID;
#endif
#endif /*  #if defined(__IBMC(PP)__) */
/*   the uSoft 16bit compiler family for DOS */

#if defined (__MSC__)
typedef   signed char CHAR;                     /*  1 byte */
typedef unsigned char UCHAR;                    /*  1 byte */
typedef   signed int  INT16;                    /*  2 byte */
typedef unsigned int  UINT16;                   /*  2 byte */
typedef   signed long INT32;                    /*  4 byte */
typedef unsigned long UINT32;                   /*  4 byte */
/*  -------------------------------------------------------------------------- */
typedef   signed int  INT;                      /*  2 byte */
typedef unsigned int  UINT;                     /*  2 byte */
typedef   signed long LONG;                     /*  4 byte */
typedef unsigned long ULONG;                    /*  4 byte */
typedef          void VOID;
#endif /*  #if defined(_MSC_VER) */
/*   the Watcom 16/32bit compilers */

#if defined (__WATCOMC__)
#if !defined (__OS2_H__) && !defined (__NT__)  /*  os2.h and windows.h defines it already */
typedef signed char   CHAR;                     /*  1 byte */
typedef unsigned char UCHAR;                    /*  1 byte */
#endif
#if !defined (__NT__)  /* windows.h defines its already */
typedef signed short int   INT16;               /*  2 byte */
typedef unsigned short int UINT16;              /*  2 byte */
#endif
/*#if !defined(__OS2_H__)*/
typedef signed int   INT32;                     /*  4 byte */
typedef unsigned int UINT32;                    /*  4 byte */
/*#endif*/
/*  -------------------------------------------------------------------------- */
#if !defined (__OS2_H__) && !defined (__NT__)  /*  os2.h and windows.h defines it already */
typedef signed int    INT;                      /*  2/4 byte */
typedef unsigned int  UINT;                     /*  2/4 byte */
typedef signed long   LONG;                     /*  4 byte */
typedef unsigned long ULONG;                    /*  4 byte */
typedef          void VOID;
#endif

#endif /*  #if defined(__WATCOMC__) */
/*  MacOS X with gcc */
#if defined (__APPLE__) && defined (__MACH__)
typedef   signed char  CHAR;                    /*  1 byte */
typedef unsigned char  UCHAR;                   /*  1 byte */
typedef   signed short INT16;                   /*  2 byte */
typedef unsigned short UINT16;                  /*  2 byte */
typedef   signed int   INT32;                   /*  4 byte */
typedef unsigned int   UINT32;                  /*  4 byte */
/*  -------------------------------------------------------------------------- */
typedef   signed int  INT;                      /*  4 byte */
typedef unsigned int  UINT;                     /*  4 byte */
typedef   signed long LONG;                     /*  4 byte */
typedef unsigned long ULONG;                    /*  4 byte */
typedef void          VOID;
#endif

#else /* pstdint.h/stdint.h work for this platform */
/* Compile with --std=gnu99 if you can or
 * define HAS_STDINT_H if you have one in your system or
 * define USE_PSTDINT_H if it works as replacement */
#include "pstdint.h"
/* typedef   int8_t  CHAR; */                        /*  1 byte */
typedef  uint8_t  UCHAR;                        /*  1 byte */
typedef   int16_t INT16;                        /*  2 byte */
typedef  uint16_t UINT16;                       /*  2 byte */
#ifndef __NT__ /* these are overlapped with windows' types */
typedef   int32_t INT32;                        /*  4 byte */
typedef  uint32_t UINT32;                       /*  4 byte */
/*  -------------------------------------------------------------------------- */
typedef   signed int  INT;                      /*  4 byte */
typedef unsigned int  UINT;                     /*  4 byte */
typedef   signed long LONG;                     /*  4 byte */
typedef unsigned long ULONG;                    /*  4 byte */
typedef          void VOID;
#endif
#endif // if !defined (USE_PSTDINT_H) && !defined (HAS_STDINT_H)

#ifdef __cplusplus
}
#endif

#endif /*  #if !defined(_TYPESIZE_H) */
