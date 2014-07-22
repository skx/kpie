
#
#  Our version number
#
VERSION=0.4

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
#  CFLAGS
#
#  1.  Add Debian hardening flags.
#  2.  Add common-options
#  3.  Add libwnck + lua5.1
#
CFLAGS=$(shell dpkg-buildflags --get CFLAGS)
CFLAGS+=-pedantic -ansi -std=c99 -Wall -Wextra
CFLAGS+=$(shell pkg-config --cflags libwnck-1.0) $(shell pkg-config --cflags ${LVER})


#
#  LDFlags
#
#  1.  Add Debian hardening flags.
#  2.  Add libwnck + lua5.1
#
LDFLAGS=$(shell dpkg-buildflags --get LDFLAGS)
LDFLAGS+=$(shell pkg-config --libs libwnck-1.0) $(shell pkg-config --libs ${LVER})


#
#  Build the application.
#
kpie: kpie.c Makefile
	gcc -o kpie kpie.c -DVERSION=$(VERSION) $(CFLAGS) $(LDFLAGS)


#
#  Clean
#
clean:
	rm kpie || true

