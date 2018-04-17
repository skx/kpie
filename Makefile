
#
#  Our version number
#
VERSION=1.0
DIST_PREFIX=/tmp
BASE=kpie

#
# The version of Lua we build against.
#
LUA_VERSION=5.1

#
# Magic to guess the Lua library versions.
#
LVER=lua$(LUA_VERSION)
UNAME := $(shell uname -s)
ifeq ($(UNAME),DragonFly)
	LVER=lua-$(LUA_VERSION)
endif
ifeq ($(UNAME),Darwin)
	LVER=lua #(use lua-52)
endif


#
#  Common definitions.
#
CC=gcc
LINKER=$(CC) -o


#
#  CFLAGS
#
#  0.  Define a version.
#  1.  Add Debian hardening flags.
#  2.  Add common-options
#  3.  Add libwnck + lua5.1
#
CFLAGS=-DVERSION=\"$(VERSION)\"
#CFLAGS+=$(shell dpkg-buildflags --get CFLAGS)
CFLAGS+=-pedantic -std=c99 -Wall -Wextra -fPIC
CFLAGS+=$(shell pkg-config --cflags libwnck-1.0) $(shell pkg-config --cflags ${LVER})


#
#  LDFlags
#
#  1.  Add Debian hardening flags.
#  2.  Add libwnck + lua5.1
#
LDFLAGS=
LDFLAGS+=$(shell dpkg-buildflags --get LDFLAGS)
LDFLAGS+=$(shell pkg-config --libs libwnck-1.0) $(shell pkg-config --libs ${LVER})
LDFLAGS+=-lX11

#
# Setup our sources.
#
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=%.o)


#
# Build the main-binary.
#
kpie: $(OBJECTS)
	$(LINKER) $@ $(LFLAGS) $(OBJECTS) $(CFLAGS) $(LDFLAGS)



#
# Pretty-print our sources
#
indent:
	astyle --style=allman -A1 --indent=spaces=4   --break-blocks --pad-oper --pad-header --unpad-paren --max-code-length=200 *.c *.h


#
#  Clean
#
clean:
	rm kpie *.o || true
	find . -name '*~' -delete || true


#
#  Create a distribution tarball.
#
dist:   clean
	rm -rf $(DIST_PREFIX)/$(BASE)-$(VERSION)
	rm -f $(DIST_PREFIX)/$(BASE)-$(VERSION).tar.gz
	cp -R . $(DIST_PREFIX)/$(BASE)-$(VERSION)
	find  $(DIST_PREFIX)/$(BASE)-$(VERSION) -name ".git*" -print | xargs rm -rf
	find  $(DIST_PREFIX)/$(BASE)-$(VERSION) -name "debian" -print | xargs rm -rf
	cd $(DIST_PREFIX) && tar -cvf $(DIST_PREFIX)/$(BASE)-$(VERSION).tar $(BASE)-$(VERSION)/
	gzip $(DIST_PREFIX)/$(BASE)-$(VERSION).tar
	mv $(DIST_PREFIX)/$(BASE)-$(VERSION).tar.gz .
