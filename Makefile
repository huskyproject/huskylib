# Makefile for the Husky build environment

# include Husky-Makefile-Config
ifeq ($(DEBIAN), 1)
# Every Debian-Source-Paket has one included.
include /usr/share/husky/huskymak.cfg
else ifdef RPM_BUILD_ROOT
# RPM build requires all files to be in the source directory
include huskymak.cfg
else
include ../huskymak.cfg
endif

ifeq ($(OSTYPE), UNIX)
  LIBPREFIX=lib
  DLLPREFIX=lib
endif

include make/makefile.inc

ifeq ($(DEBUG), 1)
  CFLAGS+=$(WARNFLAGS) $(DEBCFLAGS)
  LFLAGS=$(DEBLFLAGS)
else
  CFLAGS+=$(WARNFLAGS) $(OPTCFLAGS)
  LFLAGS=$(OPTLFLAGS)
endif

CDEFS=-D$(OSTYPE) $(ADDCDEFS) -I$(_H_DIR)
LIBS=
#-L. -l$(LIBNAME)

ifeq ($(DYNLIBS), 1)
all: $(TARGETLIB) $(TARGETDLL).$(VER) $(PROGRAMS)
else
all: $(TARGETLIB) $(PROGRAMS)
endif

_SRC_DIR = src/
_H_DIR   = huskylib

%$(_OBJ): $(_SRC_DIR)%.c
	$(CC) $(CFLAGS) $(CDEFS) $(_SRC_DIR)$*.c

$(TARGETLIB): $(OBJS)
	$(AR) $(AR_R) $(TARGETLIB) $?
ifdef RANLIB
	$(RANLIB) $(TARGETLIB)
endif

ifeq ($(DYNLIBS), 1)
  ifeq (~$(MKSHARED)~,~ld~)
$(TARGETDLL).$(VER): $(OBJS)
	$(LD) $(LFLAGS) -o $(TARGETDLL).$(VER) $(OBJS)
  else
$(TARGETDLL).$(VER): $(OBJS)
	$(CC) -shared -Wl,-soname,$(TARGETDLL).$(VERH) \
          -o $(TARGETDLL).$(VER) $(OBJS)
  endif
	$(LN) $(LNOPT) $(TARGETDLL).$(VER) $(TARGETDLL).$(VERH) ;\
	$(LN) $(LNOPT) $(TARGETDLL).$(VER) $(TARGETDLL)

install-dynlib: $(TARGETLIB) $(TARGETDLL).$(VER)
	-$(MKDIR) $(MKDIROPT) $(DESTDIR)$(LIBDIR)
	$(INSTALL) $(ILOPT) $(TARGETDLL).$(VER) $(DESTDIR)$(LIBDIR)
	-$(RM) $(RMOPT) $(DESTDIR)$(LIBDIR)$(DIRSEP)$(TARGETDLL).$(VERH)
	-$(RM) $(RMOPT) $(DESTDIR)$(LIBDIR)$(DIRSEP)$(TARGETDLL)
# Changed the symlinks from symlinks with full path to just symlinks.
# Better so :)
	cd $(DESTDIR)$(LIBDIR) ;\
	$(LN) $(LNOPT) $(TARGETDLL).$(VER) $(TARGETDLL).$(VERH) ;\
	$(LN) $(LNOPT) $(TARGETDLL).$(VER) $(TARGETDLL)
ifneq (~$(LDCONFIG)~, ~~)
	$(LDCONFIG)
endif

else
install-dynlib:

endif

#ifeq ($(DYNLIBS), 1)
#$(PROGRAMS): $(TARGETDLL).$(VER)
#	$(CC) $(CFLAGS) $(CDEFS) $(COPT) $(_SRC_DIR)$@.c
#	$(CC) $(LFLAGS) $(EXENAMEFLAG) $@ $@$(_OBJ) $(LIBS) $<
#else
#   Build programs static linked only
$(PROGRAMS): $(TARGETLIB)
	$(CC) $(CFLAGS) $(CDEFS) $(COPT) $(_SRC_DIR)$(basename $@).c
	$(CC) $(LFLAGS) $(EXENAMEFLAG) $@ $(basename $@)$(_OBJ) $(TARGETLIB)
#endif


FORCE:

install-h-dir: FORCE
	-$(MKDIR) $(MKDIROPT) $(DESTDIR)$(INCDIR)$(DIRSEP)$(_H_DIR)

%.h: FORCE
	-$(INSTALL) $(IIOPT) $(_H_DIR)$(DIRSEP)$@ $(DESTDIR)$(INCDIR)$(DIRSEP)$(_H_DIR)

install-h: install-h-dir $(HEADERS)

install-bindir:
	-$(MKDIR) $(MKDIROPT) $(DESTDIR)$(BINDIR)

install-programs: $(PROGRAMS) install-bindir
	$(INSTALL) $(IBOPT) $< $(DESTDIR)$(BINDIR)

install-lib: $(TARGETLIB)
	-$(MKDIR) $(MKDIROPT) $(DESTDIR)$(LIBDIR)
	$(INSTALL) $(ISLOPT) $(TARGETLIB) $(DESTDIR)$(LIBDIR)

install-man:
	(cd man && $(MAKE) install)

install: install-dynlib install-lib install-programs install-h
#	@list='$(PROGRAMS)'; for p in $$list; do \
#	    $(INSTALL) $(IBOPT) $$p $(DESTDIR)$(BINDIR); \
#	done
	@echo
	@echo "*** For install man pages run 'gmake install-man' (unixes only)"
	@echo


uninstall:
	-cd $(DESTDIR)$(INCDIR)$(DIRSEP)$(_H_DIR) ;\
	$(RM) $(RMOPT) $(HEADERS)
	-$(RM) $(RMOPT) $(LIBDIR)$(DIRSEP)$(TARGETLIB)
	-$(RM) $(RMOPT) $(LIBDIR)$(DIRSEP)$(TARGETDLL)*
	-cd $(DESTDIR)$(BINDIR) ;\
	$(RM) $(RMOPT) $(PROGRAMS)

clean:
	-$(RM) $(RMOPT) *$(_OBJ)
	-$(RM) $(RMOPT) $(TARGETDLL).$(VERH)
	-$(RM) $(RMOPT) $(TARGETDLL)

distclean: clean
	-$(RM) $(RMOPT) $(TARGETLIB)
	-$(RM) $(RMOPT) $(TARGETDLL).$(VER)
	-$(RM) $(RMOPT) $(PROGRAMS)
