# $Id$
#
# Common makefiles part (include-file) for huskylib

# Use defines before including this file:
# _OBJ	object files suffix begins with point (".o", ".obj", ... )
# _LIB	library files suffix begins with point (".a", ".lib", ... )
# _DLL	dinamic linking library files suffix begins with point (".dll", ".so", ...)
# LIBPREFIX  library filename prefix ("lib", "", ...)
# LIBSUFFIX  library filename suffix (compiler ID, three characters, somewhere empty)
# DLLPREFIX  dinamic linking library prefix (usually empty)
# DLLSUFFIX  dinamic linking library suffix (compiler ID or empty)
# O, B, L, H prefixes to use with names of objects, binaries, libraries and headers respectively

# Library name
LIBNAME	= husky

# Version
VERMAJOR= 1
VERMINOR= 9
VERPATCH= 0
VERH	= $(VERMAJOR).$(VERMINOR)
VER	= $(VERH).$(VERPATCH)

# Sources directory
SRC_DIR	=../src/
# Headers directory
H_DIR	=../huskylib/

# Header files (please sort list to easy check by human)
HEADERS1= $(H)BCD.h $(H)BCO.h $(H)BCW.h $(H)BEOS5.h $(H)BORLANDC.h $(H)DJGPP.h
HEADERS2= $(H)EMX.h $(H)HIGHC.h $(H)IBMC_OS2.h $(H)MINGW32.h $(H)MSC.h $(H)MSVC.h
HEADERS3= $(H)SASC.h $(H)UNIX.h $(H)WATCOMC.h $(H)WCD.h $(H)WCO.h $(H)WCW.h
HEADERS4= $(H)WCX.h $(H)calendar.h $(H)compiler.h $(H)crc.h $(H)cvtdate.h
HEADERS5= $(H)dirlayer.h $(H)fexist.h $(H)ffind.h $(H)huskyext.h $(H)huskylib.h
HEADERS6= $(H)log.h $(H)locking.h $(H)memory.h $(H)pstdint.h $(H)recode.h
HEADERS7= $(H)strext.h $(H)syslogp.h $(H)temp.h $(H)tree.h $(H)typesize.h $(H)unused.h
HEADERS8= $(H)vixie.h $(H)xstr.h $(H)ftnaddr.h
HEADERS = $(HEADERS1) $(HEADERS2) $(HEADERS3) $(HEADERS4) $(HEADERS5) $(HEADERS6) $(HEADERS7) $(HEADERS8)

# Object files: library (please sort list to easy check by human)
OBJS1	= $(O)adcase$(_OBJ) $(O)calendar$(_OBJ) $(O)cmpfname$(_OBJ) $(O)crc$(_OBJ)
OBJS2	= $(O)cvtdate$(_OBJ) $(O)dirlayer$(_OBJ) $(O)fexist$(_OBJ) $(O)ffind$(_OBJ)
OBJS3	= $(O)genmsgid$(_OBJ) $(O)genverst$(_OBJ) $(O)getfree$(_OBJ) $(O)ioutil$(_OBJ)
OBJS4	= $(O)log$(_OBJ) $(O)locking$(_OBJ) $(O)mapfile$(_OBJ) $(O)memory$(_OBJ)
OBJS5	= $(O)qksort$(_OBJ) $(O)patmat$(_OBJ) $(O)recode$(_OBJ) $(O)ftnaddr$(_OBJ)
OBJS6	= $(O)setfsize$(_OBJ) $(O)strext$(_OBJ) $(O)strftime$(_OBJ) $(O)temp$(_OBJ)
OBJS7	= $(O)tdelay$(_OBJ) $(O)tree$(_OBJ) $(O)version$(_OBJ) $(O)xstr$(_OBJ)
OBJS8	=
OBJS	= $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) $(OBJS7) $(OBJS8)

# Object files: programs
GNMSGIDOBJ= $(O)gnmsgid$(_OBJ)
PRGOBJS	= $(GNMSGIDOBJ)

# Binary file(s) to build from sources
GNMSGID	= $(B)gnmsgid$(_EXE)
PROGRAMS= $(GNMSGID)

# Usually compose from LIBPREFIX, LIBSUFFIX and LIB.
TARGETLIB = $(L)$(LIBPREFIX)$(LIBNAME)$(LIBSUFFIX)$(_LIB)
TARGETDLL = $(B)$(DLLPREFIX)$(LIBNAME)$(DLLSUFFIX)$(_DLL)
TARGET_H  = *.h

# List of HUSKY libraries required to build binary file(s)
LIBS	=
