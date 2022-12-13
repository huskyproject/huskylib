# huskylib/Makefile
#
# This file is part of huskylib, part of the Husky fidonet software project
# Use with GNU make v.3.82 or later
# Requires: husky environment
#

ifeq ($(GNMSGID), 1)
ifdef MAN1DIR
    huskylib_MAN1PAGES := gnmsgid.1
endif
endif

huskylib_PROG_OBJ = $(O)gnmsgid$(_OBJ)
huskylib_PROG_SRC := $(huskylib_PROG_OBJ:$(_OBJ)=.c)

huskylib_OBJS := $(filter-out $(huskylib_OBJDIR)$(huskylib_PROG_OBJ),$(huskylib_ALL_OBJS))

# Executable file(s) to build from sources
ifeq ($(GNMSGID), 1)
    huskylib_PROGS = $(B)gnmsgid$(_EXE)
endif

# Static and dynamic target libraries
huskylib_TARGETLIB := $(L)$(LIBPREFIX)$(huskylib_LIBNAME)$(LIBSUFFIX)$(_LIB)
huskylib_TARGETDLL := $(B)$(DLLPREFIX)$(huskylib_LIBNAME)$(DLLSUFFIX)$(_DLL)

ifeq ($(DYNLIBS), 1)
    ifeq ($(findstring Windows,$(OS)),)
        huskylib_TARGET = $(huskylib_TARGETDLL).$(huskylib_VER)
    else
        huskylib_TARGET = $(huskylib_TARGETDLL)
    endif
else
    huskylib_TARGET = $(huskylib_TARGETLIB)
endif

huskylib_TARGET_BLD = $(huskylib_BUILDDIR)$(huskylib_TARGET)
huskylib_TARGET_DST = $(LIBDIR_DST)$(huskylib_TARGET)

# List of HUSKY libraries required to build binary file(s)
huskylib_LIBS =

huskylib_CDEFS := $(CDEFS) -I$(huskylib_ROOTDIR)$(huskylib_H_DIR)

.PHONY: huskylib_build huskylib_install huskylib_install-dynlib \
    huskylib_install-programs huskylib_install-man huskylib_uninstall \
    huskylib_clean huskylib_distclean huskylib_depend huskylib_rm_OBJS \
    huskylib_rm_BLD huskylib_rm_DEP huskylib_rm_DEPS

ifeq ($(GNMSGID), 1)
    ifdef MAN1DIR
        huskylib_build: $(huskylib_TARGET_BLD) \
        $(huskylib_BUILDDIR)$(huskylib_PROGS) \
        $(huskylib_BUILDDIR)$(huskylib_MAN1PAGES).gz
    else
        huskylib_build: $(huskylib_BUILDDIR)$(huskylib_TARGET) \
        $(huskylib_BUILDDIR)$(huskylib_PROGS)
    endif
else
    huskylib_build: $(huskylib_BUILDDIR)$(huskylib_TARGET)
endif

ifneq ($(MAKECMDGOALS), depend)
ifneq ($(MAKECMDGOALS), distclean)
ifneq ($(MAKECMDGOALS), uninstall)
    include $(huskylib_DEPS)
endif
endif
endif

# Make a hard link of the library in $(huskylib_BUILDDIR)
$(huskylib_TARGET_BLD): $(huskylib_OBJDIR)$(huskylib_TARGET)
	$(LN) $(LNHOPT) $< $(huskylib_BUILDDIR)

# Build the static library
$(huskylib_OBJDIR)$(huskylib_TARGETLIB): $(huskylib_OBJS) | do_not_run_make_as_root
	cd $(huskylib_OBJDIR); $(AR) $(AR_R) $(huskylib_TARGETLIB) $(^F)
ifdef RANLIB
	cd $(huskylib_OBJDIR); $(RANLIB) $(huskylib_TARGETLIB)
endif

# Build the dynamic library
ifeq ($(DYNLIBS),1)
$(huskylib_OBJDIR)$(huskylib_TARGET): $(huskylib_OBJS) | do_not_run_make_as_root
    ifeq ($(filter gcc clang,$(MKSHARED)),)
		$(LD) $(LFLAGS) -o $@ $^
    else
		$(CC) $(LFLAGS) -shared -Wl,-soname,$(huskylib_TARGET) -o $@ $^
    endif
endif

# Compile .c files
$(huskylib_OBJS): $(huskylib_OBJDIR)%$(_OBJ): $(huskylib_SRCDIR)%.c | $(huskylib_OBJDIR)
	$(CC) $(CFLAGS) $(huskylib_CDEFS) -o $(huskylib_OBJDIR)$*$(_OBJ) $(huskylib_SRCDIR)$*.c

$(huskylib_OBJDIR): | $(huskylib_BUILDDIR) do_not_run_make_as_root
	[ -d $@ ] || $(MKDIR) $(MKDIROPT) $@


# Build auxiliary program
ifeq ($(GNMSGID), 1)
    $(huskylib_BUILDDIR)$(huskylib_PROGS): \
        $(huskylib_OBJDIR)$(huskylib_PROG_OBJ) \
        $(huskylib_BUILDDIR)$(huskylib_TARGET) | do_not_run_make_as_root
	$(CC) $(LFLAGS) $(EXENAMEFLAG) $@ $^

    $(huskylib_OBJDIR)$(huskylib_PROG_OBJ): \
        $(huskylib_SRCDIR)$(huskylib_PROG_SRC) | do_not_run_make_as_root
	$(CC) $(CFLAGS) $(huskylib_CDEFS) -o $@ $<

    ifdef MAN1DIR
        # Build man pages
        $(huskylib_BUILDDIR)$(huskylib_MAN1PAGES).gz: \
            $(huskylib_ROOTDIR)man/$(huskylib_MAN1PAGES) | do_not_run_make_as_root
		gzip -c $<  > $@
    endif
endif

# Install
ifeq ($(GNMSGID), 1)
    ifdef MAN1DIR
        huskylib_install: huskylib_install-dynlib huskylib_install-programs \
                          huskylib_install-man ;
    else
        huskylib_install: huskylib_install-dynlib huskylib_install-programs ;
    endif
else
    huskylib_install: huskylib_install-dynlib ;
endif

ifneq ($(DYNLIBS), 1)
    huskylib_install-dynlib: ;
else
    ifneq ($(strip $(LDCONFIG)),)
        huskylib_install-dynlib: \
            $(LIBDIR_DST)$(huskylib_TARGETDLL).$(huskylib_VER)
		-@$(LDCONFIG) >& /dev/null || true
    else
        huskylib_install-dynlib: \
            $(LIBDIR_DST)$(huskylib_TARGETDLL).$(huskylib_VER) ;
    endif

    $(LIBDIR_DST)$(huskylib_TARGETDLL).$(huskylib_VER): \
        $(huskylib_BUILDDIR)$(huskylib_TARGETDLL).$(huskylib_VER) | \
        $(DESTDIR)$(LIBDIR)
		$(INSTALL) $(ILOPT) $< $(DESTDIR)$(LIBDIR); \
		cd $(DESTDIR)$(LIBDIR); \
		$(TOUCH) $(huskylib_TARGETDLL).$(huskylib_VER); \
		$(LN) $(LNOPT) $(huskylib_TARGETDLL).$(huskylib_VER) $(huskylib_TARGETDLL).$(huskylib_VERH); \
		$(LN) $(LNOPT) $(huskylib_TARGETDLL).$(huskylib_VER) $(huskylib_TARGETDLL)

    $(DESTDIR)$(LIBDIR):
		-[ -d "$(DESTDIR)$(LIBDIR)" ] || $(MKDIR) $(MKDIROPT) $(DESTDIR)$(LIBDIR)
endif

ifneq ($(GNMSGID), 1)
    huskylib_install-programs: ;
else
    huskylib_install-programs: $(BINDIR_DST)$(huskylib_PROGS) ;

    $(BINDIR_DST)$(huskylib_PROGS): $(huskylib_BUILDDIR)$(huskylib_PROGS) | $(DESTDIR)$(BINDIR)
		$(INSTALL) $(IBOPT) $? $(DESTDIR)$(BINDIR); \
		$(TOUCH) $@
endif

ifdef MAN1DIR
    huskylib_install-man: $(DESTDIR)$(MAN1DIR)$(DIRSEP)$(huskylib_MAN1PAGES).gz ;

    $(DESTDIR)$(MAN1DIR)$(DIRSEP)$(huskylib_MAN1PAGES).gz: \
        $(huskylib_BUILDDIR)$(huskylib_MAN1PAGES).gz | $(DESTDIR)$(MAN1DIR)
	$(INSTALL) $(IMOPT) $? $(DESTDIR)$(MAN1DIR); \
	$(TOUCH) $@
endif


# Clean
huskylib_clean: huskylib_rm_OBJS
	-[ -d "$(huskylib_OBJDIR)" ] && $(RMDIR) $(huskylib_OBJDIR) || true

huskylib_rm_OBJS:
	-$(RM) $(RMOPT) $(huskylib_OBJDIR)*


# Distclean
huskylib_distclean: huskylib_clean huskylib_rm_BLD
	-[ -d "$(huskylib_BUILDDIR)" ] && $(RMDIR) $(huskylib_BUILDDIR) || true

huskylib_rm_BLD: huskylib_rm_DEP
	-$(RM) $(RMOPT) $(huskylib_BUILDDIR)$(huskylib_TARGETLIB)
	-$(RM) $(RMOPT) $(huskylib_BUILDDIR)$(huskylib_TARGETDLL)*
ifeq ($(GNMSGID), 1)
	-$(RM) $(RMOPT) $(huskylib_BUILDDIR)$(huskylib_PROGS)
endif
ifdef MAN1DIR
	-$(RM) $(RMOPT) $(huskylib_BUILDDIR)$(huskylib_MAN1PAGES).gz
endif

huskylib_rm_DEP: huskylib_rm_DEPS
	-[ -d "$(huskylib_DEPDIR)" ] && $(RMDIR) $(huskylib_DEPDIR) || true

huskylib_rm_DEPS:
	-$(RM) $(RMOPT) $(huskylib_DEPDIR)*


# Uninstall
huskylib_uninstall:
ifeq ($(DYNLIBS), 1)
	-$(RM) $(RMOPT) $(DESTDIR)$(LIBDIR)$(DIRSEP)$(huskylib_TARGETDLL)*
endif
ifeq ($(GNMSGID), 1)
	-$(RM) $(RMOPT) $(BINDIR_DST)$(huskylib_PROGS)
endif
ifdef MAN1DIR
	-$(RM) $(RMOPT) $(DESTDIR)$(MAN1DIR)$(DIRSEP)$(huskylib_MAN1PAGES).gz
endif
# The next line is placed here in case the previous lines are missing
	@pwd > /dev/null
