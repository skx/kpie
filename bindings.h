/**
 * bindings.h - Definitions of the lua primitives.
 *
 * Copyright (c) 2014 by Steve Kemp.  All rights reserved.
 *
 **
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 dated June, 1991, or (at your
 * option) any later version.
 *
 * On Debian GNU/Linux systems, the complete text of version 2 of the GNU
 * General Public License can be found in `/usr/share/common-licenses/GPL-2'
 *
 **
 *
 */



#ifndef _BINDINGS_H
#define _BINDINGS_H 1

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


/**
 * Setup.
 */
void init_lua(int _debug, const char *config_file);


/**
 * Call the users lua file.
 */
void invoke_lua(void);


/**
 * Teardown.
 */
void close_lua(void);


/**
 * Invoke the given Lua script, if it exists.
 */
void invoke_lua(void);


/**
 * Utility functions.
 */
void debug(const char *msg);
guint32 get_timestamp(void);




/**
 **
 ** Now the primitives themselves, in alphabetical order.
 **
 **/


int lua_above(lua_State * L);
int lua_activate_workspace(lua_State * L);
int lua_below(lua_State * L);
int lua_exists(lua_State * L);
int lua_focus(lua_State * L);
int lua_fullscreen(lua_State * L);
int lua_is_focussed(lua_State * L);
int lua_is_fullscreen(lua_State * L);
int lua_is_maximized(lua_State * L);
int lua_is_minimized(lua_State * L);
int lua_kill(lua_State * L);
int lua_maximize(lua_State * L);
int lua_maximize_horizontally(lua_State * L);
int lua_maximize_vertically(lua_State * L);
int lua_minimize(lua_State * L);
int lua_pin(lua_State * L);
int lua_pointer(lua_State *L);
int lua_readdir(lua_State * L);
int lua_screen_height(lua_State * L);
int lua_screen_width(lua_State * L);
int lua_shade(lua_State * L);
int lua_size(lua_State * L);
int lua_unfullscreen(lua_State * L);
int lua_unmaximize(lua_State * L);
int lua_unminimize(lua_State * L);
int lua_unpin(lua_State * L);
int lua_unshade(lua_State * L);
int lua_window_application(lua_State * L);
int lua_window_class(lua_State * L);
int lua_window_id(lua_State * L);
int lua_window_xid(lua_State * L);
int lua_window_pid(lua_State * L);
int lua_window_role(lua_State * L);
int lua_window_title(lua_State * L);
int lua_window_type(lua_State * L);
int lua_workspace(lua_State * L);
int lua_workspaces(lua_State * L);
int lua_xy(lua_State * L);

#endif
