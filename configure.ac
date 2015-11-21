#
# Initial attempt at making a autoconf-based builder
#

AC_INIT([kpie], [0.8], [steve@steve.org.uk])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_HEADER_STDC



dnl Additional macro definitions are in here
AC_CONFIG_MACRO_DIR([m4])

AX_PROG_LUA
AX_LUA_HEADERS
AX_LUA_LIBS


#
# This seems to work for detecting the libraries.
#
AC_CHECK_LIB([gdk-x11-2.0], [gdk_display_get_n_screens], [],
    [AC_MSG_ERROR([gtk+-2.x is missing or not built with X support])])
AC_CHECK_LIB([wnck-1], [wnck_window_get_name], [],
    [AC_MSG_ERROR([libwnck-1 is missing])])

#
# This should be improved, but I'm not sure how.
#
LIBS="-lm ${LIBS} -lX11 ${LUA_LIB} $(pkg-config --libs gtk+-2.0 gdk-x11-2.0 libwnck-1.0)"
CFLAGS="${CFLAGS} ${LUA_INCLUDE} $(pkg-config --cflags gtk+-2.0 gdk-x11-2.0 libwnck-1.0)"

AC_CONFIG_FILES([ Makefile ])
AC_OUTPUT

AC_MSG_NOTICE([
Building kpie against Lua ${LUA_VERSION}
])
