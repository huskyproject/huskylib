# Makefile for the Husky build environment

# include Husky-Makefile-Config
ifeq ($(DEBIAN), 1)
# Every Debian-Source-Paket has one included.
include /usr/share/husky/huskymak.cfg
else
include ../huskymak.cfg
endif

include make/makefile.inc

LIB_D = $(LIBPREFIX)$(LIBNAME).so.$(VER)
LIB_S = $(LIBPREFIX)$(LIBNAME).a

ifeq ($(DEBUG), 1)
  CFLAGS=	$(WARNFLAGS) $(DEBCFLAGS)
else
  CFLAGS=	$(WARNFLAGS) $(OPTCFLAGS)
endif

ifneq ($(OSTYPE), UNIX)
#  LIBPREFIX=
else
  LIBPREFIX=lib
endif

CDEFS=	-D$(OSTYPE) $(ADDCDEFS) -Ih

ifeq ($(DYNLIBS), 1)
all: $(LIB_S) $(LIB_D)
else
all: $(LIB_S)
endif

SRC_DIR = src/
H_DIR   = h/

%$(OBJ): $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(CDEFS)  $(SRC_DIR)$*.c

$(LIB_S): $(OBJS)
	$(AR) $(AR_R) $(LIB_S) $?
ifdef RANLIB
	$(RANLIB) $(LIB_S)
endif

ifeq ($(DYNLIBS), 1)
  ifeq (~$(MKSHARED)~,~ld~)
$(LIBPREFIX)$(LIBNAME).so.$(VER): $(OBJS)
	$(LD) $(OPTLFLAGS) \
	      -o $(LIBPREFIX)$(LIBNAME).so.$(VER) $(OBJS)
  else
$(LIBPREFIX)$(LIBNAME).so.$(VER): $(OBJS)
	$(CC) -shared -Wl,-soname,$(LIBPREFIX)$(LIBNAME).so.$(VERH) \
          -o $(LIBPREFIX)$(LIBNAME).so.$(VER) $(OBJS)
  endif

instdyn: $(LIB_S) $(LIBPREFIX)$(LIBNAME).so.$(VER)
	-$(MKDIR) $(MKDIROPT) $(LIBDIR)
	$(INSTALL) $(ILOPT) $(LIBPREFIX)$(LIBNAME).so.$(VER) $(LIBDIR)
	-$(RM) $(RMOPT) $(LIBDIR)/$(LIBPREFIX)$(LIBNAME).so.$(VERH)
	-$(RM) $(RMOPT) $(LIBDIR)/$(LIBPREFIX)$(LIBNAME).so
# Changed the symlinks from symlinks with full path to just symlinks.
# Better so :)
	cd $(LIBDIR) ;\
	$(LN) $(LNOPT) $(LIBPREFIX)$(LIBNAME).so.$(VER) $(LIBPREFIX)$(LIBNAME).so.$(VERH) ;\
	$(LN) $(LNOPT) $(LIBPREFIX)$(LIBNAME).so.$(VER) $(LIBPREFIX)$(LIBNAME).so
ifneq (~$(LDCONFIG)~, ~~)
	$(LDCONFIG)
endif

else
instdyn: $(LIB_S)

endif

FORCE:

install-h-dir: FORCE
	-$(MKDIR) $(MKDIROPT) $(INCDIR)
	-$(MKDIR) $(MKDIROPT) $(INCDIR)$(DIRSEP)huskylib

%.h: FORCE
	$(INSTALL) $(IIOPT) $(H_DIR)$@ $(INCDIR)$(DIRSEP)huskylib

install-h: install-h-dir $(HEADERS)

install: install-h instdyn
	-$(MKDIR) $(MKDIROPT) $(LIBDIR)
	$(INSTALL) $(ISLOPT) $(LIB_S) $(LIBDIR)

uninstall:
	-cd $(INCDIR)$(DIRSEP)$(LIBNAME)$(DIRSEP) ;\
	$(RM) $(RMOPT) $(HEADERS)
	-$(RM) $(RMOPT) $(LIBDIR)$(DIRSEP)$(LIB_S)
	-$(RM) $(RMOPT) $(LIBDIR)$(DIRSEP)$(LIBPREFIX)$(LIBNAME).so.$(VER)
	-$(RM) $(RMOPT) $(LIBDIR)$(DIRSEP)$(LIBPREFIX)$(LIBNAME).so.$(VERH)
	-$(RM) $(RMOPT) $(LIBDIR)$(DIRSEP)$(LIBPREFIX)$(LIBNAME).so

clean:
	-$(RM) $(RMOPT) *$(OBJ)

distclean: clean
	-$(RM) $(RMOPT) $(LIB_S)
	-$(RM) $(RMOPT) $(LIB_D)
