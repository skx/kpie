
#
#  Our version number
#
VERSION=0.3

#
#  CFLAGS
#
#  1.  Add Debian hardening flags.
#  2.  Add common-options
#  3.  Add libwnck + lua5.1
#
CFLAGS=$(shell dpkg-buildflags --get CFLAGS)
CFLAGS+=-pedantic -ansi -std=c99 -Wall -Wextra
CFLAGS+=$(shell pkg-config --cflags libwnck-1.0) $(shell pkg-config --cflags lua5.1)


#
#  LDFlags
#
#  1.  Add Debian hardening flags.
#  2.  Add libwnck + lua5.1
#
LDFLAGS=$(shell dpkg-buildflags --get LDFLAGS)
LDFLAGS+=$(shell pkg-config --libs libwnck-1.0) $(shell pkg-config --libs lua5.1)


#
#  Build the application.
#
kpie: kpie.c Makefile
	gcc -DVERSION=$(VERSION) $(CFLAGS) $(LDFLAGS) -o kpie kpie.c


#
#  Clean
#
clean:
	rm kpie || true

