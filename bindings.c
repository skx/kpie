/**
 * bindings.c - Implementation of the lua primitives.
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


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>
#include <sys/stat.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE 1
#include <libwnck/libwnck.h>

#include "bindings.h"




/**
 * The lua_open function is present in Lua 5.1 but not Lua 5.2.
 */
#ifndef lua_open
#define lua_open luaL_newstate
#endif


/**
 * Avoid warnings about unused variables.
 */
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif





/**
 * The externally defined - in kpie.c - pointer to the *current* window.
 *
 * The current window is updated via libwnck and all the primitives
 * which are implemented in this file apply to that current window.
 */
extern WnckWindow *g_window;


/**
 * The Lua state.
 *
 * Private to this compilation unit.
 */
lua_State *L;


/**
 * The configuration file we'll load/execute.
 *
 * Private to this compilation unit.
 *
 * Set in init_lua().
 */
char *g_config = NULL;


/**
 * The debug setting.
 *
 * Private to this compilation unit.
 *
 * Set in init_lua().
 */
int g_debug;





/**
 * Setup our bindings.
 */
void init_lua(int _debug, const char *config_file)
{
    /**
     * Open Lua, and load the standard libraries.
     */
    L = lua_open();
    luaL_openlibs(L);


    /**
     * Register our primitives to the Lua environment.
     */
    lua_register(L, "above", lua_above);
    lua_register(L, "activate_workspace", lua_activate_workspace);
    lua_register(L, "below", lua_below);
    lua_register(L, "exists", lua_exists);
    lua_register(L, "focus", lua_focus);
    lua_register(L, "fullscreen", lua_fullscreen);
    lua_register(L, "is_focussed", lua_is_focussed);
    lua_register(L, "is_fullscreen", lua_is_fullscreen);
    lua_register(L, "is_maximized", lua_is_maximized);
    lua_register(L, "is_minimized", lua_is_minimized);
    lua_register(L, "kill", lua_kill);
    lua_register(L, "maximize", lua_maximize);
    lua_register(L, "minimize", lua_minimize);
    lua_register(L, "pin", lua_pin);
    lua_register(L, "readdir", lua_readdir);
    lua_register(L, "screen_height", lua_screen_height);
    lua_register(L, "screen_width", lua_screen_width);
    lua_register(L, "shade", lua_shade);
    lua_register(L, "size", lua_size);
    lua_register(L, "unfullscreen", lua_unfullscreen);
    lua_register(L, "unmaximize", lua_unmaximize);
    lua_register(L, "unminimize", lua_unminimize);
    lua_register(L, "unpin", lua_unpin);
    lua_register(L, "unshade", lua_unshade);
    lua_register(L, "window_application", lua_window_application);
    lua_register(L, "window_class", lua_window_class);
    lua_register(L, "window_id", lua_window_id);
    lua_register(L, "window_pid", lua_window_pid);
    lua_register(L, "window_title", lua_window_title);
    lua_register(L, "window_type", lua_window_type);
    lua_register(L, "workspace", lua_workspace);
    lua_register(L, "workspaces", lua_workspaces);
    lua_register(L, "xy", lua_xy);

    /**
     * Set the value DEBUG to be true/false depending on how we were
     * invoked.
     */
    if (_debug)
        lua_pushboolean(L, 1);
    else
        lua_pushboolean(L, 0);
    lua_setglobal(L, "DEBUG");


    /**
     * Set the global variables VERSION and CONFIG.
     */
    lua_pushstring(L, config_file);
    lua_setglobal(L, "CONFIG");
    lua_pushnumber(L, VERSION);
    lua_setglobal(L, "VERSION");

    /**
     * Save the constructor paramaters.
     */
    g_config = g_strdup(config_file);
    g_debug = _debug;
}


/**
 * Invoke the configuration file against the currently activated
 * window, if it exists.
 */
void invoke_lua()
{
    /**
     * See if the users lua-file is present, if not return.
     */
    struct stat sb;
    if (stat(g_config, &sb) < 0)
        return;

    int error = luaL_dofile(L, g_config);

    if (error)
    {
        if (!lua_isstring(L, lua_gettop(L)))
            printf("ERROR: no detail found\n");

        const char *str = lua_tostring(L, lua_gettop(L));
        lua_pop(L, 1);

        printf("ERROR: %s\n", str);
        exit(1);
    }
}


/**
 * Close our Lua interpretter.
 */
void close_lua()
{
    lua_close(L);
    if (g_config != NULL)
        free(g_config);
    g_config = NULL;
}


/**
 * Show a message if we're debugging.
 */
void debug(const char *msg)
{
    if (g_debug)
        printf("DEBUG: %s\n", msg);
}


/**
 * Some of the libwnck operations require a timestamp, for the time
 * an event was emittied.
 *
 * This function returns something suitable.
 */
guint32 get_timestamp()
{
    GTimeVal timestamp;
    g_get_current_time(&timestamp);
    return(timestamp.tv_sec);
}


/**
 **
 ** The actual primitives now - in alphabetical order.
 **
 **
 **/


/**
 * Set the window to be above all other windows.
 */
int lua_above(lua_State * L)
{
    UNUSED(L);
    debug("above window");
    wnck_window_make_above(g_window);
    return 0;
}


/**
 * Activate the given workspace.
 */
int lua_activate_workspace(lua_State * L)
{
    int number = luaL_checknumber(L, 1);

    /**
     * Get the count of workspaces.
     */
    WnckScreen *screen = wnck_window_get_screen(g_window);
    int count = wnck_screen_get_workspace_count(screen);

    if (number < 1 || number > count)
    {
        g_warning("Workspace number out of bounds: %d", number);
    } else
    {
        char *x = g_strdup_printf("activate workspace %d", number);
        debug(x);
        g_free(x);

        WnckScreen *screen;
        WnckWorkspace *workspace;

        screen = wnck_window_get_screen(g_window);
        workspace = wnck_screen_get_workspace(screen, number - 1);

        if (workspace)
            wnck_workspace_activate(workspace, get_timestamp() );
        else
            g_warning("Failed to get workspace %d", number);
    }
    return 0;
}


/**
 * Unset the window from being above all windows.
 */
int lua_below(lua_State * L)
{
    UNUSED(L);
    debug("below window");
    wnck_window_unmake_above(g_window);
    return 0;
}


/**
 * Does the given file/directory exist?
 */
int lua_exists(lua_State * L)
{
    const char *path = luaL_checkstring(L, 1);

    struct stat sb;
    if (stat(path, &sb) < 0)
        lua_pushboolean(L, 0);
    else
        lua_pushboolean(L, 1);
    return 1;
}


/**
 * Focus the current window.
 */
int lua_focus(lua_State * L)
{
    UNUSED(L);
    debug("focus window");
    wnck_window_activate(g_window, get_timestamp() );
    return 0;
}


/**
 * Set the window to be fullscreen.
 */
int lua_fullscreen(lua_State * L)
{
    UNUSED(L);
    debug("fullscreen window");
    wnck_window_set_fullscreen(g_window, TRUE);
    return 0;
}


/**
 * Is the window active?
 */
int lua_is_focussed(lua_State * L)
{
    WnckScreen *screen = wnck_window_get_screen(g_window);
    WnckWindow *cur = wnck_screen_get_active_window(screen);

    if (cur == g_window )
        lua_pushboolean(L, 1);
    else
        lua_pushboolean(L, 0);
    return 1;
}

/**
 * Is the window fullscreen?
 */
int lua_is_fullscreen(lua_State * L)
{
    if (wnck_window_is_fullscreen(g_window))
        lua_pushboolean(L, 1);
    else
        lua_pushboolean(L, 0);
    return 1;
}


/**
 * Is the window maximized?
 */
int lua_is_maximized(lua_State * L)
{
    if (wnck_window_is_maximized(g_window))
        lua_pushboolean(L, 1);
    else
        lua_pushboolean(L, 0);
    return 1;
}


/**
 * Is the window minimized?
 */
int lua_is_minimized(lua_State * L)
{
    if (wnck_window_is_minimized(g_window))
        lua_pushboolean(L, 1);
    else
        lua_pushboolean(L, 0);
    return 1;
}


/**
 * Kill the current window.
 */
int lua_kill(lua_State * L)
{
    UNUSED(L);
    debug("kill window");
    wnck_window_close(g_window, get_timestamp() );
    return (0);
}


/**
 * Maximize the current window.
 */
int lua_maximize(lua_State * L)
{
    UNUSED(L);
    debug("maximizing window");
    wnck_window_maximize(g_window);
    return 0;
}


/**
 * Minimize the current window.
 */
int lua_minimize(lua_State * L)
{
    UNUSED(L);
    debug("minimizing window");
    wnck_window_minimize(g_window);
    return 0;
}


/**
 * Pin the window to all workspaces.
 */
int lua_pin(lua_State * L)
{
    UNUSED(L);
    debug("pin window");
    wnck_window_pin(g_window);
    return 0;
}


/**
 * Return a table of all files in the given directory.
 *
 * This is for inclusion-purposes, see the FAQ.
 */
int lua_readdir(lua_State * L)
{
    struct dirent *dp;
    DIR *dir;
    int count = 0;

    const char *directory = luaL_checkstring(L, 1);

    lua_newtable(L);

    dir = opendir(directory);

    while (dir)
    {
        if ((dp = readdir(dir)) != NULL)
        {
            lua_pushnumber(L, count);
            lua_pushstring(L, dp->d_name);
            lua_settable(L, -3);

            count += 1;
        } else
        {
            closedir(dir);
            dir = 0;
        }
    }

    lua_pushliteral(L, "n");
    lua_pushnumber(L, count - 1);
    lua_rawset(L, -3);

    return 1;
}


/**
 * Get the size of the screen.
 */
int lua_screen_height(lua_State * L)
{
    WnckScreen *screen = wnck_window_get_screen(g_window);
    lua_pushinteger(L, wnck_screen_get_height(screen));
    return 1;
}


/**
 * Get the size of the screen.
 */
int lua_screen_width(lua_State * L)
{
    WnckScreen *screen = wnck_window_get_screen(g_window);
    lua_pushinteger(L, wnck_screen_get_width(screen));
    return 1;
}


/**
 * Shade this window.
 */
int lua_shade(lua_State * L)
{
    UNUSED(L);
    debug("shade window");
    wnck_window_shade(g_window);
    return 0;
}


/**
 * Get/Set the width/height of the window.
 */
int lua_size(lua_State * L)
{
    int top = lua_gettop(L);

    /**
     * Set the values?
     */
    if (top > 0)
    {
        int h = luaL_checknumber(L, 1);
        int w = luaL_checknumber(L, 2);


        char *x = g_strdup_printf("size(%d,%d);", h, w);
        debug(x);
        g_free(x);

        wnck_window_set_geometry(g_window,
                                 WNCK_WINDOW_GRAVITY_CURRENT,
                                 WNCK_WINDOW_CHANGE_WIDTH +
                                 WNCK_WINDOW_CHANGE_HEIGHT, -1, -1, h, w);

    }

    int x;
    int y;
    int width;
    int height;
    wnck_window_get_geometry(g_window, &x, &y, &width, &height);
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    return 2;
}


/**
 * Unset the window to be fullscreen.
 */
int lua_unfullscreen(lua_State * L)
{
    UNUSED(L);
    debug("unfullscreen window");
    wnck_window_set_fullscreen(g_window, FALSE);
    return 0;
}


/**
 * UnMaximize the current window.
 */
int lua_unmaximize(lua_State * L)
{
    UNUSED(L);
    debug("unmaximize window");
    wnck_window_unmaximize(g_window);
    return 0;
}


/**
 * Unminimize the current window.
 */
int lua_unminimize(lua_State * L)
{
    UNUSED(L);
    debug("unminimize window");
    wnck_window_unminimize(g_window, get_timestamp() );
    return 0;
}


/**
 * Unpin the window from all workspaces.
 */
int lua_unpin(lua_State * L)
{
    UNUSED(L);
    debug("unpin window");
    wnck_window_unpin(g_window);
    return 0;
}


/**
 * Unshade this window.
 */
int lua_unshade(lua_State * L)
{
    UNUSED(L);
    debug("unshade window");
    wnck_window_unshade(g_window);
    return 0;
}


/**
 * Return the application which created this window.
 */
int lua_window_application(lua_State * L)
{
    WnckApplication *a = wnck_window_get_application(g_window);
    lua_pushstring(L, wnck_application_get_name(a));
    return 1;
}


/**
 * Return the class of this window.
 */
int lua_window_class(lua_State * L)
{
    WnckClassGroup *x = wnck_window_get_class_group(g_window);
    const char *class = wnck_class_group_get_name(x);
    lua_pushstring(L, class);
    return 1;
}


/**
 * Return the ID of this window.
 */
int lua_window_id(lua_State * L)
{
    const char *id = wnck_window_get_session_id(g_window);
    if (id)
        lua_pushstring(L, wnck_window_get_session_id(g_window));
    else
    {
        g_warning("Failed to find ID");
        lua_pushstring(L, "");
    }
    return 1;
}


/**
 * Return the PID of the process that created this window.
 */
int lua_window_pid(lua_State * L)
{
    int pid = wnck_window_get_pid(g_window);
    if (pid)
        lua_pushinteger(L, pid);
    else
    {
        g_warning("Failed to find PID");
        lua_pushinteger(L, 0);
    }
    return 1;
}


/**
 * Return the title of this window.
 */
int lua_window_title(lua_State * L)
{
    lua_pushstring(L, wnck_window_get_name(g_window));
    return 1;
}


/**
 * Return the type of this window.
 */
int lua_window_type(lua_State * L)
{
    WnckWindowType t = wnck_window_get_window_type(g_window);

    switch (t)
    {
    case WNCK_WINDOW_NORMAL:
        lua_pushstring(L, "WINDOW_NORMAL");
        break;
    case WNCK_WINDOW_DESKTOP:
        lua_pushstring(L, "WINDOW_DESKTOP");
        break;
    case WNCK_WINDOW_DOCK:
        lua_pushstring(L, "WINDOW_DOCK");
        break;
    case WNCK_WINDOW_DIALOG:
        lua_pushstring(L, "WINDOW_DIALOG");
        break;
    case WNCK_WINDOW_TOOLBAR:
        lua_pushstring(L, "WINDOW_TOOLBAR");
        break;
    case WNCK_WINDOW_MENU:
        lua_pushstring(L, "WINDOW_MENU");
        break;
    case WNCK_WINDOW_UTILITY:
        lua_pushstring(L, "WINDOW_UTILITY");
        break;
    case WNCK_WINDOW_SPLASHSCREEN:
        lua_pushstring(L, "WINDOW_SPLASHSCREEN");
        break;
    default:
        g_warning("Unknown window-type");
        return 0;
        break;
    };
    return 1;
}


/**
 * Get/set the workspace the window is on.
 */
int lua_workspace(lua_State * L)
{

    int top = lua_gettop(L);

    /**
     * Set the value?
     */
    if (top > 0)
    {
        int number = luaL_checknumber(L, 1);


        /**
         * Get the count of workspaces.
         */
        WnckScreen *screen = wnck_window_get_screen(g_window);
        int count = wnck_screen_get_workspace_count(screen);

        if (number < 1 || number > count)
        {
            g_warning("Workspace number out of bounds: %d", number);
        } else
        {

            char *x = g_strdup_printf("move window to workspace %d", number);
            debug(x);
            g_free(x);

            WnckScreen *screen;
            WnckWorkspace *workspace;

            screen = wnck_window_get_screen(g_window);
            workspace = wnck_screen_get_workspace(screen, number - 1);

            if (workspace)
                wnck_window_move_to_workspace(g_window, workspace);
            else
                g_warning("Workspace number %d does not exist!", number);
        }
        return 0;
    }

    /**
     * Get the value.
     */
    WnckWorkspace *w = wnck_window_get_workspace(g_window);
    if (w == NULL)
        lua_pushinteger(L, -1);
    else
        lua_pushinteger(L, wnck_workspace_get_number(w) + 1);
    return 1;
}


/**
 * Count the workspaces.
 */
int lua_workspaces(lua_State * L)
{
    WnckScreen *screen = wnck_window_get_screen(g_window);

    int top = lua_gettop(L);

    /**
     * Set the values?
     */
    if (top > 0)
    {
        int newcount = luaL_checknumber(L, 1);
        wnck_screen_change_workspace_count(screen, newcount);
    }

    int count = wnck_screen_get_workspace_count(screen);
    lua_pushinteger(L, count);
    return 1;
}


/**
 * Get/Set the X/Y coords of the window.
 */
int lua_xy(lua_State * L)
{
    int top = lua_gettop(L);

    /**
     * Set the values?
     */
    if (top > 0)
    {
        int newx = luaL_checknumber(L, 1);
        int newy = luaL_checknumber(L, 2);


        char *x = g_strdup_printf("xy(%d,%d);", newx, newy);
        debug(x);
        g_free(x);

        wnck_window_set_geometry(g_window,
                                 WNCK_WINDOW_GRAVITY_CURRENT,
                                 WNCK_WINDOW_CHANGE_X +
                                 WNCK_WINDOW_CHANGE_Y, newx, newy, -1, -1);

    }

    int x;
    int y;
    int width;
    int height;
    wnck_window_get_geometry(g_window, &x, &y, &width, &height);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    return 2;
}
