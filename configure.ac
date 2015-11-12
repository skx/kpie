#
# Initial attempt at making a autoconf-based builder
#

AC_INIT([kpie], [0.8], [steve@steve.org.uk])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_HEADER_STDC


###
# Checks for Lua
# Try versioned Lua 5.2 first
PKG_CHECK_MODULES([LUA52], [lua5.2],,[
	PKG_CHECK_MODULES([LUA52], [lua52],,[
		PKG_CHECK_MODULES([LUA52], [lua-5.2],,[:])
	])
])

if test -z "${LUA52_PKG_ERRORS}" -a "${LUA52}" != no -a "${LUAC52}" != no ; then
	LUA_VERSION="5.2"
	LUA_CFLAGS="${LUA52_CFLAGS}"
	LUA_LIBS="${LUA52_LIBS}"
else
	# Fall back to versioned Lua 5.1
	PKG_CHECK_MODULES([LUA51], [lua5.1  >= 5.1.3],,[
		PKG_CHECK_MODULES([LUA51], [lua51   >= 5.1.3],,[
			PKG_CHECK_MODULES([LUA51], [lua-5.1 >= 5.1.3],,[:])
		])
	])

	if test -z "${LUA51_PKG_ERRORS}" -a "${LUA51}" != no -a "${LUAC51}" != no ; then
		LUA_VERSION="5.1"
		LUA_CFLAGS="${LUA51_CFLAGS}"
		LUA_LIBS="${LUA51_LIBS}"
	else
        AC_MSG_ERROR([Need a Lua toolchain with matching versions ('lua' library and 'lua' and 'luac' programs)])
	fi
fi

LIBS="-lm ${LIBS} ${LUA_LIBS}"
CFLAGS="${CFLAGS} ${LUA_CFLAGS}"


# Fall back to versioned Lua 5.1
#PKG_CHECK_MODULES([LUA51], [lua5.1  >= 5.1.3],,[
#   PKG_CHECK_MODULES([LUA51], [lua51   >= 5.1.3],,[
#      PKG_CHECK_MODULES([LUA51], [lua-5.1 >= 5.1.3],,[:])
#   ])
#])


#
# This seems to work for detecting the libraries.
#
AC_CHECK_LIB([gdk-x11-2.0], [gdk_display_get_n_screens], [],
    [AC_MSG_ERROR([gtk+-2.x is missing or not built with X support])])
AC_CHECK_LIB([wnck-1], [wnck_window_get_name], [],
    [AC_MSG_ERROR([libwnck-1 is missing])])

#
# Suspect by doing this I'm missing the whole point of things.
#
LIBS="-lm -lX11 ${LIBS} ${LUA_LIBS} $(${PKG_CONFIG} --libs gdk-x11-2.0 libwnck-1.0)"
CFLAGS="${CFLAGS} ${LUA_CFLAGS} $(${PKG_CONFIG} --cflags gdk-x11-2.0 libwnck-1.0)"


AC_CONFIG_FILES([ Makefile ])
AC_OUTPUT

AC_MSG_NOTICE([
Building kpie against Lua ${LUA_VERSION}
])
