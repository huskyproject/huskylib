# Makefile for huskylib man pages
# Written by Stas Degteff 2:5080/102 <g@grumbler.org>
# Modified for huskylib by Pavel Andreew 2:5080/59
#
# Use with GNU version of make program (gmake)
#
# include Husky Makefile Config
ifeq ($(DEBIAN), 1)
include /usr/share/husky/huskymak.cfg
else ifdef RPM_BUILD_ROOT
# For RPM build is require all files in one directory branch
include ../huskymak.cfg
else
include ../../huskymak.cfg
endif

MAN1PAGES = gnmsgid.1

MAN1DIR = $(DESTDIR)$(MANDIR)/man1

all:

install:
	$(MKDIR) $(MKDIROPT) $(MAN1DIR)
	$(INSTALL) $(IMOPT) $(MAN1PAGES) $(MAN1DIR)


clean:

distclean: clean

uninstall:
	-for f in $(MAN1PAGES) ; do $(RM) $(RMOPT) $(MAN1DIR)/$$f ; done
