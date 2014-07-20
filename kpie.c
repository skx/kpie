#define WNCK_I_KNOW_THIS_IS_UNSTABLE 1


#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <libwnck/libwnck.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>



/**
 * The window we're currently manipulating..
 */
WnckWindow *g_window;


/**
 * The Lua state.
 */
lua_State* L;





/**
 * Return the window title
 */
static int lua_window_title(lua_State *L)
{
    lua_pushstring( L, wnck_window_get_name( g_window ) );
    return 1;
}


/**
 * Return the window class.
 */
static int lua_window_class(lua_State *L)
{
    WnckClassGroup *x = wnck_window_get_class_group (g_window);
    const char *class =wnck_class_group_get_name( x );
    lua_pushstring( L, class );
    return 1;
}


/**
 * Return the window ID.
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
 * Return the window PID.
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
 * Maximize the window.
 */
static int lua_maximize( lua_State *L)
{
    wnck_window_maximize(g_window);
    return 0;
}

/**
 * Set the window to be fullscreen.
 */
static int lua_fullscreen( lua_State *L)
{
    wnck_window_set_fullscreen(g_window, TRUE );
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
 * Move to the given workspace.
 */
static int lua_move_to( lua_State *L)
{
    int number = lua_tonumber(L, 1);

    WnckScreen *screen;
    WnckWorkspace *workspace;

    screen = wnck_window_get_screen(g_window);
    workspace = wnck_screen_get_workspace(screen, number-1);

    if (!workspace) {
        g_warning("Workspace number %d does not exist!", number);
    }
    wnck_window_move_to_workspace(g_window, workspace);

    return 0;
}


/**
 * Set the window to be above all windows.
 */
static int lua_above( lua_State *L)
{
    wnck_window_make_above (g_window);
    return 0;
}


/**
 * Unset the window from being above all windows.
 */
static int lua_below( lua_State *L)
{
    wnck_window_unmake_above (g_window);
    return 0;
}


/**
 * Pin the window.
 */
static int lua_pin( lua_State *L)
{
    wnck_window_pin (g_window);
    return 0;
}

/**
 * Unpin the window.
 */
static int lua_unpin( lua_State *L)
{
    wnck_window_unpin (g_window);
    return 0;
}

/**
 * Get the X/Y coords of the window.
 */
static int lua_xy( lua_State *L )
{
    int top = lua_gettop(L);

    /**
     * Set the values.
     */
    if ( top > 0 )
    {
        int newx = lua_tonumber(L,1);
        int newy = lua_tonumber(L,2);

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
 * This function is called when a new window is created.
 *
 * It has two jobs:
 *
 *   1. Save the window pointer to our global g_window variable.
 *
 *   2. Invoke the lua file ~/.kpie.lua.
 *
 */
static void
on_window_opened (WnckScreen *screen,
                  WnckWindow *window,
                  gpointer    data)
{

    /**
     * Update the global "current window" to point to the current
     * window which has just been created.
     */
    g_window = window;

    /**
     * See if the user has ~/.kpie.lua present.
     */
    char buf[1024] = { '\0' };

    if ( getenv( "HOME" ) == NULL )
        return;

    struct stat sb;
    snprintf( buf, 1000, "%s/.kpie.lua", getenv( "HOME" ) );

    if(stat(buf,&sb) < 0)
        return;

    int error = luaL_dofile(L, buf );

    if(error)
    {
        // the top of the stack should be the error string
        if (!lua_isstring(L, lua_gettop(L)))
            printf("ERROR: no detail found\n" );

        // get the top of the stack as the error and pop it off
        const char * str = lua_tostring(L, lua_gettop(L));
        lua_pop(L, 1);

        printf("ERROR: %s\n", str );

        exit(1);
    }

}



/**
 * Entry point.
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
    lua_register(L, "window_class", lua_window_class);
    lua_register(L, "window_id",    lua_window_id);
    lua_register(L, "window_pid",   lua_window_pid);
    lua_register(L, "screen_width", lua_screen_width);
    lua_register(L, "screen_height", lua_screen_height);

    /**
     * Max
     */
    lua_register(L, "maximize", lua_maximize);
    lua_register(L, "is_maximized", lua_is_maximized);
    lua_register(L, "fullscreen", lua_fullscreen);
    lua_register(L, "is_fullscreen", lua_is_fullscreen);

    /**
     * Workspaces
     */
    lua_register(L, "move_to", lua_move_to);

    /**
     * On top / below all
     */
    lua_register(L, "above", lua_above );
    lua_register(L, "below", lua_below );
    lua_register(L, "pin", lua_pin );
    lua_register(L, "unpin", lua_unpin );


    /**
     * Size/Position.
     */
    lua_register(L,"xy", lua_xy );


    GMainLoop *loop;
    WnckScreen *screen;

    gdk_init (&argc, &argv);

    loop = g_main_loop_new (NULL, FALSE);
    screen = wnck_screen_get_default ();

    g_signal_connect (screen, "window-opened",
                      G_CALLBACK (on_window_opened), NULL);

    g_main_loop_run (loop);

    g_main_loop_unref (loop);

    return 0;
}

