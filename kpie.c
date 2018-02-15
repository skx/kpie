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
#include <sys/types.h>

#include <getopt.h>

#include <gdk/gdk.h>
#include <gdk/gdkx.h>


#define WNCK_I_KNOW_THIS_IS_UNSTABLE 1
#include <libwnck/libwnck.h>

#include "bindings.h"




/**
 * The window we're currently manipulating.
 *
 * A callback from libwnck which is invoked every time a window is created.
 *
 * The callback updates _this_ pointer to point to the newly-created window,
 * which all the Lua primitives then use.
 *
 * NOTE: This is safe because the Lua invocation is single-threaded.
 *
 */
WnckWindow *g_window;



/**
 * This function is called when a new window is created.
 *
 * It has two jobs:
 *
 *   1. Save the window pointer to our global g_window variable.
 *
 *   2. Invoke the lua callback.
 *
 * The prototype is here to avoid the warning from clang, which is
 * a surprise.
 *
 */
void on_window_opened(WnckScreen * screen, WnckWindow * window, gpointer data);
void on_window_opened(WnckScreen * screen, WnckWindow * window, gpointer data)
{

    (void)(screen);
    (void)(data);

    /**
     * Update the global "current window" to point to the current
     * window which has just been created.
     */
    g_window = window;

    /**
     * Load/Invoke the lua callback.
     */
    invoke_lua();
}



/**
 * Entry point to our code.
 */
int main(int argc, char **argv)
{
    /**
     * The Lua configuration file we parse/use.
     */
    gchar *config_file;


    /**
     * Setup our default configuration file.
     */
    if (getenv("HOME") != NULL)
        config_file = g_strdup_printf("%s/.kpie.lua", getenv("HOME"));
    else
        config_file = "";


    /**
     * Command-line flags;
     */
    int debug = 0;
    int single = 0;


    /**
     * Parse our options.
     */
    while (1)
    {
        static struct option long_options[] =
        {
            {"debug", no_argument, 0, 'd'},
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
            debug = TRUE;
            break;

        case 'c':
            g_free(config_file);
            config_file = g_strdup(optarg);
            break;

        case 's':
            single = TRUE;
            break;

        case 'v':
            printf("kpie - %s", VERSION);
#ifdef LUA_VERSION
            printf(" (built against " LUA_VERSION ")");
#endif
            printf("\n");
            return (0);

        case '?':
            /* getopt_long already printed an error message. */
            return (1);
        }
    }


    /**
     * If a configuration file was specified use a different one.
     */
    int index;

    for (index = optind; index < argc; index++)
    {
        g_free(config_file);
        config_file = g_strdup(argv[index]);
    }


    /**
     * Show details if we should.
     */
    if (debug)
        printf("Loading configuration file: %s\n", config_file);

    if (debug && single)
        printf("Single run\n");

    /**
     * Initialize Lua.
     */
    init_lua(debug, config_file);


    /**
     * Initialize GDK
     */
    gdk_init(&argc, &argv);


    /**
     * Get a new loop and the screen.
     */
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    /**
     * If we're running just the once, thanks to "--single", then do that.
     * once.
     */
    if (single)
    {

        /**
         * Force a sync.
         */
        wnck_screen_force_update(wnck_screen_get_default());


        /**
         * Count each screen.
         */
        int i;
        int count = gdk_display_get_n_screens(gdk_display_get_default());

        /**
         * For each screen ..
         */
        for (i = 0; i < count; i++)
        {
            WnckScreen *screen = wnck_screen_get(i);

            /**
             * Get all windows.
             */
            GList *window_l;

            for (window_l = wnck_screen_get_windows(screen);
                    window_l != NULL; window_l = window_l->next)
            {
                /**
                 * Update the global pointer to the current window.
                 */
                WnckWindow *w = WNCK_WINDOW(window_l->data);

                /**
                 * Invoke our callback.
                 */
                on_window_opened(screen, w, NULL);
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
        for (i = 0; i < count; i++)
        {

            /**
             * Ensure our callback is invoked when new windows are opened.
             */
            WnckScreen *screen = wnck_screen_get(i);
            g_signal_connect(screen, "window-opened", G_CALLBACK(on_window_opened), NULL);
        }

        /**
         * Launch - NOTE: Never returns.
         */
        g_main_loop_run(loop);
    }


    /**
     * Never reached unless --single was used.
     */


    /**
     * Cleanup
     */
    g_main_loop_unref(loop);
    g_free(config_file);
    close_lua();

    return 0;
}
