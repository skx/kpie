
#
#  Our version number
#
VERSION=0.7
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
ifeq ($(UNAME),Linux)
	LVER=lua
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
CFLAGS=-DVERSION=$(VERSION)
#CFLAGS+=$(shell dpkg-buildflags --get CFLAGS)
CFLAGS+=-pedantic -std=c99 -Wall -Wextra
CFLAGS+=$(shell pkg-config --cflags gdk-3.0)
CFLAGS+=$(shell pkg-config --cflags libwnck-1.0)
CFLAGS+=$(shell pkg-config --cflags ${LVER})


#
#  LDFlags
#
#  1.  Add Debian hardening flags.
#  2.  Add libwnck + lua5.1
#
#LDFLAGS=$(shell dpkg-buildflags --get LDFLAGS)
LDFLAGS+=$(shell pkg-config --libs gdk-3.0)
LDFLAGS+=$(shell pkg-config --libs libwnck-1.0)
LDFLAGS+=$(shell pkg-config --libs ${LVER})
LDFLAGS+=-lX11


SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=%.o)


kpie: $(OBJECTS)
	$(LINKER) $@ $(LFLAGS) $(OBJECTS) $(CFLAGS) $(LDFLAGS)



indent:
	find . \( -name '*.c' -o -name '*.h' \) -exec indent  --braces-after-if-line --no-tabs  --k-and-r-style --line-length 90 --indent-level 4 -bli0 \{\} \;

#
#  Clean
#
clean:
	rm kpie *.o || true
	find . -name '*~' -delete || true


dist:   clean
	rm -rf $(DIST_PREFIX)/$(BASE)-$(VERSION)
	rm -f $(DIST_PREFIX)/$(BASE)-$(VERSION).tar.gz
	cp -R . $(DIST_PREFIX)/$(BASE)-$(VERSION)
	find  $(DIST_PREFIX)/$(BASE)-$(VERSION) -name ".git*" -print | xargs rm -rf
	find  $(DIST_PREFIX)/$(BASE)-$(VERSION) -name "debian" -print | xargs rm -rf
	cd $(DIST_PREFIX) && tar -cvf $(DIST_PREFIX)/$(BASE)-$(VERSION).tar $(BASE)-$(VERSION)/
	gzip $(DIST_PREFIX)/$(BASE)-$(VERSION).tar
	mv $(DIST_PREFIX)/$(BASE)-$(VERSION).tar.gz .
