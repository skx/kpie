KPie
----

KPie is a simple window manipulation tool, modeled after devil's pie,
with a Lua-based configuration file.

Usage is:

     kpie [options] [lua-config-file.lua]

Options are currently limited to:

    --debug    - Show debugging information.
    --single   - Process each window once, then exit.
    --version  - Show the version number.


Overview
--------

If the file `~/.kpie.lua` exists then it will be parsed and executed
every time a new window is opened upon your system.

If you start `kpie` with a named Lua file that will be used instead, for
example:

    $ kpie ~/.config/kpie.lua

Regardless of which file is used, the configuration file is Lua with
the addition of some window-related primitives.  To give you a flavour this
is a sample configuration file:

    --
    -- If Xine is launched it should be "always on top"
    --
    if ( window_class() == "xine" ) then
        above()
    end

    --
    -- The xlogo program is so cool it should be visible on all
    -- workspaces
    --
    if ( window_title() == "xlogo" ) then
        pin()
    end

**NOTE**: More configuration files are included in the [sample directory](samples/).

The key thing to understand is that the **same** configuration file will be
invoked for **every** window on your system, on the basis that you'll limit
your actions to specific windows via the matching options.

For example if you had a configuration file which read merely "`maximize()`"
your desktop would become very odd, as **all** the windows would be maximized,
including your panel(s).



Configuration Generator
-----------------------

Included within the repository is a sample configuration file,
[dump.lua](samples/dump.lua), which is designed to be a helpful starting
point if you wish to script the manipulation of your windows.

Simply run:

    $ ./kpie ./samples/dump.lua

This will output chunks of config which you can edit or save:

    -- Screen width : 1920
    -- Screen height: 1080
    if ( ( window_title() == "feeds" ) and
         ( window_class() == "Pidgin" ) ) then
            xy(0,0)
            size(1438,1023 )
            workspace(2)
    end
    if ( ( window_title() == "Buddy List" ) and
         ( window_class() == "Pidgin" ) ) then
         xy(1438,0 )
         size(482,1023 )
         workspace(2)
    end

As you can see this has iterated over all existing windows, and shown
you their current state - this is perfect if you wish to reproduce a
complex layout interactively.



Installation
------------

Firstly install the dependencies:

      sudo apt-get install libwnck-de
      sudo apt-get install liblua5.1-0-dev

Then build via:

    make



Command Line Options
--------------------


Primitives
----------

The following primitives are available:

* Information
  * `window_title` - Get the title of the new window.
  * `window_application` - Get the application which created the window.
  * `window_class` - Get the class of the new window.
  * `window_id` - Get the ID of the new window.
     * This may be empty.
  * `window_pid` - Get the PID of the new window.
     * This may be zero on error.
  * `screen_height` - Get the size of the screen.
  * `screen_width` - Get the size of the screen.
* Depth
  * `above` - Make the window "always on top".
  * `below` - Remove the "always on top" flag.
* Max/Min
  * `maximize` - Maximize the window.
  * `fullscreen` - Make the window "fullscreen".
  * `is_maximized` - Is the window maximized?
  * `is_fullscreen` - Is the window fullscreen?
* Workspace
  * `pin` - Pin on all workspaces.
  * `unpin` - Don't pin on all workspaces.
* Movement
  * `xy` - Get/Set the X/Y coordinates of a window.
  * `size` - Get/Set the width/height of a window.
* Misc
  * `focus` - Focus the window.
  * `kill` - Close the window, forcibly.
  * `workspace` - Get/set the workspace the window is active on.
    * The return value may be -1 if the window is pinned, or invisible.


Some of these primitives are [documented in the sample-scripts](samples/).

Steve
--
