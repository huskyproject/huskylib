# Microsoft Visual C++ Makefile
include ../huskymak.cfg
include version.inc

CDEFS= -DUNIX -I../

ifeq ($(DEBUG), 1)
	COPT = -Wall -ggdb
        LFLAGS = -ggdb
else
        COPT= -O2
	LFLAGS = 
endif

ifeq ($(DYNLIBS), 1)
	HUSKYLIB = libhusky.so.$(VER)
else
	HUSKYLIB = libhusky.a
endif

include makefile.inc

all: $(HUSKYLIB) 

.c$(OBJ):
		$(CC) $(CFLAGS) $(CDEFS) $(COPT) -c $<

ifeq ($(DYNLIBS), 1)
$(HUSKYLIB): $(OBJS)
		$(CC) -shared -o $(HUSKYLIB) $(OBJS)
else
$(HUSKYLIB): $(OBJS)
		$(AR) $(AR_R) $(HUSKYLIB) $(OBJS)
endif

clean:
		-$(RM) *$(OBJ) *~ core

distclean: clean
		-$(RM) $(HUSKYLIB)

install: $(HUSKYLIB)
		-$(MKDIR) $(MKDIROPT) $(INCDIR)
		-$(MKDIR) $(MKDIROPT) $(INCDIR)$(DIRSEP)huskylib
		-$(MKDIR) $(MKDIROPT) $(LIBDIR)
		$(INSTALL) $(IIOPT) afixcmd.h $(INCDIR)$(DIRSEP)huskylib
		$(INSTALL) $(IIOPT) arealist.h $(INCDIR)$(DIRSEP)huskylib
		$(INSTALL) $(IIOPT) crc.h $(INCDIR)$(DIRSEP)huskylib
		$(INSTALL) $(IIOPT) fscmn.h $(INCDIR)$(DIRSEP)huskylib
		$(INSTALL) $(IIOPT) log.h $(INCDIR)$(DIRSEP)huskylib
		$(INSTALL) $(IIOPT) recode.h $(INCDIR)$(DIRSEP)huskylib
		$(INSTALL) $(IIOPT) $(HUSKYLIB) $(LIBDIR)
ifneq (~$(LDCONFIG)~, ~~)
		$(LDCONFIG)
endif
        
                
                
                

                                