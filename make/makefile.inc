# $Id$
#
# Common makefiles part (include-file)

VERH =  0.1
VER  =  $(VERH).0

OBJS	= patmat$(OBJ) fexist$(OBJ) ffind$(OBJ) genmsgid$(OBJ) parsenn$(OBJ) strext$(OBJ) strftime$(OBJ) tdelay$(OBJ) calendar$(OBJ) cvtdate$(OBJ) memory$(OBJ) setfsize$(OBJ)

HEADERS = compiler.h ffind.h patmat.h typedefs.h vixie.h calendar.h parsenn.h strext.h huskylib.h cvtdate.h unused.h

PROGRAMS= 

LIBNAME = huskylib

SRC_DIR=../src
H_DIR  =../h