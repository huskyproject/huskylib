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

# Library name
LIBNAME	= husky

# Version
VERMAJOR= 1
VERMINOR= 9
VERH	= $(VERMAJOR).$(VERMINOR)
VER	= $(VERH).0

# Object files (please sort list to easy check by human)

OBJS1	= adcase$(_OBJ) calendar$(_OBJ) crc$(_OBJ) cvtdate$(_OBJ)
OBJS2	= dirlayer$(_OBJ) fexist$(_OBJ) ffind$(_OBJ) genmsgid$(_OBJ)
OBJS3	= getfree$(_OBJ) ioutil$(_OBJ) log$(_OBJ) locking$(_OBJ)
OBJS4	= mapfile$(_OBJ) memory$(_OBJ) parsenn$(_OBJ) patmat$(_OBJ)
OBJS5   = recode$(_OBJ) setfsize$(_OBJ) strext$(_OBJ) strftime$(_OBJ)
OBJS6   = temp$(_OBJ) tdelay$(_OBJ) tree$(_OBJ) xstr$(_OBJ)

OBJS	= $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6)

# Header files (please sort list to easy check by human)

HEADERS1= BCD.h BCO.h BCW.h BEOS5.h BORLANDC.h DJGPP.h EMX.h HIGHC.h IBMC_OS2.h
HEADERS2= MINGW32.h MSC.h MSVC.h SASC.h UNIX.h WATCOMC.h WCD.h WCO.h WCW.h WCX.h
HEADERS3= calendar.h compiler.h crc.h cvtdate.h dirlayer.h
HEADERS4= fexist.h ffind.h huskyext.h huskylib.h log.h locking.h memory.h
HEADERS5= parsenn.h recode.h strext.h temp.h tree.h typesize.h
HEADERS6= unused.h vixie.h xstr.h

HEADERS	= $(HEADERS1) $(HEADERS2) $(HEADERS3) $(HEADERS4) $(HEADERS5) $(HEADERS6)

# Binary file(s) to build from sources
PROGRAMS= gnmsgid$(_EXE)

SRC_DIR	=../src/
H_DIR	=../huskylib/

# Usually compose from LIBPREFIX, LIBSUFFIX and LIB.
TARGETLIB = $(LIBPREFIX)$(LIBNAME)$(LIBSUFFIX)$(_LIB)
TARGETDLL = $(DLLPREFIX)$(LIBNAME)$(DLLSUFFIX)$(_DLL)
TARGET_H  = *.h

# List of HUSKY libraries required to build binary file(s)
LIBS	=
