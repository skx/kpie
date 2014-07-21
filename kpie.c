/**
 * kpie.c - Devils Pie-like window manipulation with Lua.
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
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <getopt.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE 1
#include <libwnck/libwnck.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


#ifndef lua_open
#  define lua_open luaL_newstate
#endif


#ifndef UNUSED
#  define UNUSED(x) (void)(x)
#endif



/**
 * The window we're currently manipulating.
 *
 * We use a callback from libwnck which is invoked every time a window
 * is created.
 *
 * The libwnck callback updates this pointer to point to a single window,
 * then invokes the Lua function upon it.
 *
 * This is safe because the Lua invocation is single-threaded.
 *
 */
WnckWindow *g_window;


/**
 * The Lua state.
 */
lua_State* L;


/**
 * The Lua configuration file we parse/use.
 */
gchar *g_config_file;


/**
 * Are we running with --debug ?
 */
int g_debug = FALSE;



/**
 * Return the title of this window.
 */
static int lua_window_title(lua_State *L)
{
    lua_pushstring( L, wnck_window_get_name( g_window ) );
    return 1;
}


/**
 * Return the application which created this window.
 */
static int lua_window_application(lua_State *L)
{
    WnckApplication *a = wnck_window_get_application(g_window);

    lua_pushstring( L, wnck_application_get_name(a) );
    return 1;
}


/**
 * Return the class of this window.
 */
static int lua_window_class(lua_State *L)
{
    WnckClassGroup *x = wnck_window_get_class_group (g_window);
    const char *class =wnck_class_group_get_name( x );
    lua_pushstring( L, class );
    return 1;
}


/**
 * Return the ID of this window.
 */
static int lua_window_id(lua_State *L)
{
    const char *id = wnck_window_get_session_id( g_window );
    if ( id )
        lua_pushstring( L, wnck_window_get_session_id( g_window ) );
    else
    {
        g_warning("Failed to find ID");
        lua_pushstring(L,"");
    }
    return 1;
}


/**
 * Return the PID of the process that created this window.
 */
static int lua_window_pid(lua_State *L)
{
    int pid = wnck_window_get_pid( g_window );
    if ( pid )
        lua_pushinteger( L, pid );
    else
    {
        g_warning("Failed to find PID");
        lua_pushinteger(L,0);
    }
    return 1;
}


/**
 * Get the size of the screen.
 */
static int lua_screen_width(lua_State *L)
{
    WnckScreen *screen = wnck_window_get_screen(g_window);
    lua_pushinteger( L, wnck_screen_get_width(screen));
    return 1;
}

/**
 * Get the size of the screen.
 */
static int lua_screen_height(lua_State *L)
{
    WnckScreen *screen = wnck_window_get_screen(g_window);
    lua_pushinteger( L, wnck_screen_get_height(screen));
    return 1;
}


/**
 * Maximize the current window.
 */
static int lua_maximize( lua_State *L)
{
    UNUSED(L);
    wnck_window_maximize(g_window);
    return 0;
}


/**
 * Minimize the current window.
 */
static int lua_minimize( lua_State *L)
{
    UNUSED(L);
    wnck_window_minimize(g_window);
    return 0;
}


/**
 * UnMaximize the current window.
 */
static int lua_unmaximize( lua_State *L)
{
    UNUSED(L);
    wnck_window_unmaximize(g_window);
    return 0;
}


/**
 * Unminimize the current window.
 */
static int lua_unminimize( lua_State *L)
{
    UNUSED(L);
    wnck_window_unminimize(g_window, 0);
    return 0;
}


/**
 * Set the window to be fullscreen.
 */
static int lua_fullscreen( lua_State *L)
{
    UNUSED(L);
    wnck_window_set_fullscreen(g_window, TRUE );
    return 0;
}


/**
 * Unset the window to be fullscreen.
 */
static int lua_unfullscreen( lua_State *L)
{
    UNUSED(L);
    wnck_window_set_fullscreen(g_window, FALSE );
    return 0;
}


/**
 * Is the window maximized?
 */
static int lua_is_maximized( lua_State *L )
{
    if ( wnck_window_is_maximized( g_window ) )
        lua_pushboolean(L,1);
    else
        lua_pushboolean(L,0);
    return 1;
}


/**
 * Is the window minimized?
 */
static int lua_is_minimized( lua_State *L )
{
    if ( wnck_window_is_minimized( g_window ) )
        lua_pushboolean(L,1);
    else
        lua_pushboolean(L,0);
    return 1;
}


/**
 * Is the window fullscreen?
 */
static int lua_is_fullscreen( lua_State *L)
{
    if ( wnck_window_is_fullscreen( g_window ) )
        lua_pushboolean(L,1);
    else
        lua_pushboolean(L,0);
    return 1;
}


/**
 * Set the window to be above all other windows.
 */
static int lua_above( lua_State *L)
{
    UNUSED(L);
    wnck_window_make_above (g_window);
    return 0;
}


/**
 * Unset the window from being above all windows.
 */
static int lua_below( lua_State *L)
{
    UNUSED(L);
    wnck_window_unmake_above (g_window);
    return 0;
}


/**
 * Pin the window to all workspaces.
 */
static int lua_pin( lua_State *L)
{
    UNUSED(L);
    wnck_window_pin (g_window);
    return 0;
}


/**
 * Unpin the window from all workspaces.
 */
static int lua_unpin( lua_State *L)
{
    UNUSED(L);
    wnck_window_unpin (g_window);
    return 0;
}


/**
 * Shade this window.
 */
static int lua_shade( lua_State *L )
{
    UNUSED(L);
    wnck_window_shade (g_window);
    return 0;
}


/**
 * Unshade this window.
 */
static int lua_unshade( lua_State *L )
{
    UNUSED(L);
    wnck_window_unshade (g_window);
    return 0;
}


/**
 * Get/Set the X/Y coords of the window.
 */
static int lua_xy( lua_State *L )
{
    int top = lua_gettop(L);

    /**
     * Set the values?
     */
    if ( top > 0 )
    {
        int newx = luaL_checknumber(L,1);
        int newy = luaL_checknumber(L,2);

        wnck_window_set_geometry(g_window,
                                 WNCK_WINDOW_GRAVITY_CURRENT,
                                 WNCK_WINDOW_CHANGE_X + WNCK_WINDOW_CHANGE_Y,
                                 newx,newy,
                                 -1,-1);

    }

    int x;
    int y;
    int width;
    int height;
    wnck_window_get_geometry(g_window, &x, &y, &width, &height );
    lua_pushinteger(L, x );
    lua_pushinteger(L, y );
    return 2;
}


/**
 * Get/Set the width/height of the window.
 */
static int lua_size( lua_State *L )
{
    int top = lua_gettop(L);

    /**
     * Set the values?
     */
    if ( top > 0 )
    {
        int h = luaL_checknumber(L,1);
        int w = luaL_checknumber(L,2);

        wnck_window_set_geometry(g_window,
                                 WNCK_WINDOW_GRAVITY_CURRENT,
                                 WNCK_WINDOW_CHANGE_WIDTH + WNCK_WINDOW_CHANGE_HEIGHT,
                                 -1,-1,h,w);

    }

    int x;
    int y;
    int width;
    int height;
    wnck_window_get_geometry(g_window, &x, &y, &width, &height );
    lua_pushinteger(L, width );
    lua_pushinteger(L, height );
    return 2;
}


/**
 * Focus the current window.
 */
static int lua_focus(lua_State *L)
{
    UNUSED(L);
    wnck_window_activate (g_window, 0);
    return 0;
}


/**
 * Get/set the workspace the window is on.
 */
static int lua_workspace(lua_State *L)
{

    int top = lua_gettop(L);

    /**
     * Set the value?
     */
    if ( top > 0 )
    {
        int number = luaL_checknumber(L, 1);


        /**
         * Count the workspaces.
         */
        WnckScreen *screen  = wnck_window_get_screen(g_window);
        int count = 0;
        GList *ws_l;
        for (ws_l = wnck_screen_get_workspaces (screen); ws_l != NULL; ws_l = ws_l->next)
            count += 1;


        if (number<0 || number>count) {
            g_warning("Workspace number out of bounds: %d", number);
        }
        else
        {
            WnckScreen *screen;
            WnckWorkspace *workspace;

            screen = wnck_window_get_screen(g_window);
            workspace = wnck_screen_get_workspace(screen, number-1);

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
    WnckWorkspace *w =  wnck_window_get_workspace( g_window );
    if ( w == NULL )
        lua_pushinteger(L, -1);
    else
        lua_pushinteger(L, wnck_workspace_get_number( w ) + 1 );
    return 1;
}


/**
 * Kill the current window.
 */
static int lua_kill( lua_State *L )
{
    UNUSED(L);
    wnck_window_close(g_window, 0);
    return( 0 );
}

/**
 * Count the workspaces.
 */
static int lua_workspaces( lua_State *L )
{
    /**
     * Get the screen, and from that the list.
     */
    WnckScreen *screen  = wnck_window_get_screen(g_window);

    /**
     * Iterate over the workspaces.
     */
    int count = 0;

    GList *ws_l;
    for (ws_l = wnck_screen_get_workspaces (screen); ws_l != NULL; ws_l = ws_l->next)
    {
        count += 1;
    }

    lua_pushinteger(L, count );
    return 1;
}


/**
 * Return a table of all files in the given directory.
 *
 * This is for inclusion-purposes, see the FAQ.
 */
static int lua_readdir(lua_State *L)
{
    struct dirent *dp;
    DIR *dir;
    int count = 0;

    const char *directory = luaL_checkstring(L,1);

    lua_newtable(L);

    dir = opendir(directory);

    while (dir)
    {
        if ((dp = readdir(dir)) != NULL)
        {
            /* Store in the table. */
            lua_pushnumber(L, count);        /* table index */
            lua_pushstring(L, dp->d_name );  /* value this index */
            lua_settable(L, -3);

            count += 1;
        }
        else
        {
            closedir( dir );
            dir = 0;
        }
    }


    /* Make sure LUA knows how big our table is. */
    lua_pushliteral(L, "n");
    lua_pushnumber(L, count -1 );
    lua_rawset(L, -3);

    return 1;        /* we've left one item on the stack */
}


/**
 * This function is called when a new window is created.
 *
 * It has two jobs:
 *
 *   1. Save the window pointer to our global g_window variable.
 *
 *   2. Invoke the lua file ~/.kpie.lua - or whatever file we've been configured with.
 *
 */
static void
on_window_opened (WnckScreen *screen,
                  WnckWindow *window,
                  gpointer    data)
{

    UNUSED(screen);
    UNUSED(data);

    /**
     * Update the global "current window" to point to the current
     * window which has just been created.
     */
    g_window = window;

    /**
     * See if the users lua-file is present, if not return.
     */
    struct stat sb;
    if(stat(g_config_file,&sb) < 0)
        return;


    /**
     * Load/Invoke the configuration file.
     */
    int error = luaL_dofile(L, g_config_file);

    if(error)
    {
        if (!lua_isstring(L, lua_gettop(L)))
            printf("ERROR: no detail found\n" );

        const char * str = lua_tostring(L, lua_gettop(L));
        lua_pop(L, 1);

        printf("ERROR: %s\n", str );
        exit(1);
    }

}



/**
 * Entry point to our code.
 */
int main (int argc, char **argv)
{

    /**
     * Initialize Lua.
     */
    L = lua_open();

    luaL_openlibs(L);


    /**
     * register our global-function
     */

    /**
     * Information.
     */
    lua_register(L, "window_title", lua_window_title);
    lua_register(L, "window_application", lua_window_application);
    lua_register(L, "window_class", lua_window_class);
    lua_register(L, "window_id",    lua_window_id);
    lua_register(L, "window_pid",   lua_window_pid);
    lua_register(L, "screen_width", lua_screen_width);
    lua_register(L, "screen_height", lua_screen_height);

    /**
     * Min/Max/Fullscreen
     */
    lua_register(L, "maximize", lua_maximize);
    lua_register(L, "minimize", lua_minimize);
    lua_register(L, "unmaximize", lua_unmaximize);
    lua_register(L, "unminimize", lua_unminimize);
    lua_register(L, "is_maximized", lua_is_maximized);
    lua_register(L, "is_minimized", lua_is_minimized);
    lua_register(L, "fullscreen", lua_fullscreen);
    lua_register(L, "unfullscreen", lua_unfullscreen);
    lua_register(L, "is_fullscreen", lua_is_fullscreen);


    /**
     * On top / below all
     */
    lua_register(L, "above", lua_above );
    lua_register(L, "below", lua_below );
    lua_register(L, "pin", lua_pin );
    lua_register(L, "unpin", lua_unpin );
    lua_register(L, "shade", lua_shade );
    lua_register(L, "unshade", lua_unshade );


    /**
     * Size/Position.
     */
    lua_register(L,"xy", lua_xy );
    lua_register(L,"size", lua_size );


    /**
     * Misc.
     */
    lua_register(L,"focus", lua_focus );
    lua_register(L,"workspace", lua_workspace );
    lua_register(L,"kill", lua_kill );
    lua_register(L,"workspaces", lua_workspaces );
    lua_register(L,"readdir", lua_readdir );


    /**
     * Setup our default configuration file.
     */
    if ( getenv( "HOME" ) != NULL )
        g_config_file = g_strdup_printf( "%s/.kpie.lua", getenv( "HOME" ) );


    /**
     * A flag to control if we just run once.
     */
    int g_single = 0;


    /**
     * Parse our options.
     */
    while (1)
    {
        static struct option long_options[] =
            {
                {"debug",  no_argument, 0, 'd'},
                {"single", no_argument, 0, 's'},
                {"config", required_argument, 0, 'c'},
                {"version", no_argument, 0, 'v'},
                {0, 0, 0, 0}
            };

        char c;

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "dsc:v", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
        case 'd':
            g_debug = TRUE;
            break;
        case 'c':
            g_free( g_config_file );
            g_config_file = g_strdup( optarg );
            break;
        case 's':
            g_single = TRUE;
            break;
        case 'v':
            printf( "kpie - %.1f", VERSION );
#ifdef LUA_VERSION
            printf( " (built against " LUA_VERSION ")" );
#endif
            printf( "\n");
            exit(0);
            break;
        case '?':
            /* getopt_long already printed an error message. */
            exit(1);
            break;
        }
    }


    /**
     * If a configuration file was specified use a different one.
     */
    int index;
    for (index = optind; index < argc; index++)
    {
        g_free( g_config_file );
        g_config_file = g_strdup( argv[index] );
    }

    if ( g_debug )
        printf( "Loading configuration file: %s\n", g_config_file );

    if ( g_debug && g_single )
        printf( "Single run\n" );


    /**
     * Set the value DEBUG to be true/false depending on how we were
     * invoked.
     */
    if ( g_debug )
        lua_pushboolean(L, 1 );
    else
        lua_pushboolean(L, 0 );
    lua_setglobal(L, "DEBUG" );


    /**
     * Set the global variables VERSION and CONFIG.
     */
    lua_pushstring(L, g_config_file );
    lua_setglobal(L, "CONFIG" );
    lua_pushnumber(L, VERSION );
    lua_setglobal(L, "VERSION" );


    /**
     * Initialize GDK
     */
    gdk_init (&argc, &argv);

    GMainLoop *loop = g_main_loop_new (NULL, FALSE);
    WnckScreen *screen = wnck_screen_get_default ();


    /**
     * If we're running a single run then just run the loop
     * once.
     */
    if ( g_single )
    {

        /**
         * Force a sync.
         */
        wnck_screen_force_update (screen);


        /**
         * Count each screen.
         */
        int i;
        int count = gdk_display_get_n_screens(gdk_display_get_default());

        /**
         * For each screen ..
         */
        for (i=0;i<count;i++)
        {
            WnckScreen *screen=wnck_screen_get(i);

            /**
             * Get all windows.
             */
            GList *window_l;
            for (window_l = wnck_screen_get_windows (screen); window_l != NULL; window_l = window_l->next)
            {
                /**
                 * Update the global pointer to the current window.
                 */
                WnckWindow *w = WNCK_WINDOW (window_l->data);

                /**
                 * Invoke our callback.
                 */
                on_window_opened( screen, w, NULL );
            }
        }

    }
    else
    {
        /**
         * Count the screens on the system.
         */
        int i;
        int count = gdk_display_get_n_screens(gdk_display_get_default());

        /**
         * For each screen.
         */
        for (i=0;i<count;i++)
        {

            /**
             * Ensure our callback is invoked when new windows are opened.
             */
            WnckScreen *screen=wnck_screen_get(i);
            g_signal_connect (screen, "window-opened",
                              G_CALLBACK (on_window_opened), NULL);
        }

        /**
         * Launch - NOTE: Never returns.
         */
        g_main_loop_run (loop);
    }


    /**
     * Never reached unless --single was used.
     */
    g_main_loop_unref (loop);
    lua_close(L);

    g_free(g_config_file);

    return 0;
}

