# $Id$
#
# Common makefiles part (include-file) for huskylib

# Use defines:
# _OBJ	object files suffix begins with point (".o", ".obj", ... )
# _LIB	library files suffix begins with point (".a", ".lib", ... )
# _DLL	dinamic linking library files suffix begins with point (".dll", ...)
# LIBPREFIX  library filename prefix ("lib", "", ...)
# LIBSUFFIX  library filename suffix (compiler ID, three characters, somewhere empty)
# DLLPREFIX  dinamic linking library prefix (usually empty)
# DLLSUFFIX  dinamic linking library suffix (compiler ID or empty)


VERH	= 0.1
VER	= $(VERH).0

OBJS1	= calendar$(_OBJ) cvtdate$(_OBJ) fexist$(_OBJ) ffind$(_OBJ) genmsgid$(_OBJ)
OBJS2	= memory$(_OBJ) parsenn$(_OBJ) patmat$(_OBJ) setfsize$(_OBJ) strext$(_OBJ)
OBJS3	= strftime$(_OBJ) tdelay$(_OBJ)

HEADERS1= calendar.h compiler.h cvtdate.h ffind.h huskylib.h patmat.h
HEADERS2= parsenn.h strext.h typedefs.h unused.h vixie.h

HEADERS	= $(HEADERS1) $(HEADERS2)

OBJS	= $(OBJS1) $(OBJS2) $(OBJS3)

PROGRAMS=

# HUSKY libraries required to build binary file(s)
# Usually compose from LIBPREFIX, LIBSUFFIX and LIB.
LIBS	=

LIBNAME	= husky

SRC_DIR	=../src/
H_DIR	=../h/

TARGETLIB = $(LIBPREFIX)$(LIBNAME)$(LIBSUFFIX)$(_LIB)
TARGETDLL = $(DLLPREFIX)$(LIBNAME)$(DLLSUFFIX)$(_DLL)
