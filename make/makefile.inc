# $Id$
#
# Common makefiles part (include-file) for huskylib

# Use defines:
# OBJ	object files suffix begins with point (".o", ".obj", ... )
# LIB	library files suffix begins with point (".a", ".lib", ... )
# DLL	dinamic linking library files suffix begins with point (".dll", ...)
# LIBPREFIX  library filename prefix ("lib", "", ...)
# LIBSUFFIX  library filename suffix (compiler ID, three characters, somewhere empty)
# DLLPREFIX  dinamic linking library prefix (usually empty)
# DLLSUFFIX  dinamic linking library suffix (compiler ID or empty)


VERH =  0.1
VER  =  $(VERH).0

OBJS	=  calendar$(OBJ) cvtdate$(OBJ) fexist$(OBJ) ffind$(OBJ) genmsgid$(OBJ)
OBJS	+= memory$(OBJ) parsenn$(OBJ) patmat$(OBJ) setfsize$(OBJ) strext$(OBJ)
OBJS	+= strftime$(OBJ) tdelay$(OBJ)

HEADERS	= calendar.h compiler.h cvtdate.h ffind.h huskylib.h patmat.h parsenn.h
HEADERS	+=strext.h typedefs.h unused.h vixie.h

PROGRAMS=

# HUSKY libraries required to build binary file(s)
# Usually compose from LIBPREFIX, LIBSUFFIX and LIB.
LIBS=

LIBNAME = husky

SRC_DIR	=../src/
H_DIR	=../h/

TARGETLIB	= $(LIBPREFIX)$(LIBNAME)$(LIBSUFFIX)$(LIB)
TARGETDLL	= $(DLLPREFIX)$(LIBNAME)$(DLLSUFFIX)$(DLL)
