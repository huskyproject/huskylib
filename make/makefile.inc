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
LIBNAME	= huskylib

# Version
VERMAJOR= 0
VERMINOR= 1
VERH	= $(VERMAJOR).$(VERMINOR)
VER	= $(VERH).0

# Object files
OBJS1	= calendar$(_OBJ) cvtdate$(_OBJ) fexist$(_OBJ) ffind$(_OBJ) genmsgid$(_OBJ)
OBJS2	= memory$(_OBJ) parsenn$(_OBJ) patmat$(_OBJ) setfsize$(_OBJ) strext$(_OBJ)
OBJS3	= strftime$(_OBJ) tdelay$(_OBJ)
OBJS	= $(OBJS1) $(OBJS2) $(OBJS3)

# Header files
HEADERS1= calendar.h compiler.h cvtdate.h ffind.h huskylib.h patmat.h
HEADERS2= parsenn.h strext.h unused.h vixie.h
HEADERS3= BCD.h BCO.h BCW.h BEOS5.h BORLANDC.h DJGPP.h EMX.h HIGHC.h IBMC_OS2.h
HEADERS4= MINGW32.h MSC.h MSVC.h SASC.h UNIX.h WATCOMC.h WCD.h WCO.h WCW.h WCX.h
HEADERS	= $(HEADERS1) $(HEADERS2) $(HEADERS3) $(HEADERS4)

# Binary file(s) to build from sources
PROGRAMS= gnmsgid$(_EXE)

# HUSKY libraries required to build binary file(s)
# Usually compose from LIBPREFIX, LIBSUFFIX and LIB.
LIBS	= -L. -l$(LIBNAME)

SRC_DIR	=../src/
H_DIR	=../huskylib/

TARGETLIB = $(LIBPREFIX)$(LIBNAME)$(LIBSUFFIX)$(_LIB)
TARGETDLL = $(DLLPREFIX)$(LIBNAME)$(DLLSUFFIX)$(_DLL)
TARGET_H  = *.h
