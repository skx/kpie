KPie
----

KPie is a simple window manipulation tool, modeled after devil's pie,
with a Lua-based configuration file.

Usage is:

     kpie [options] [lua-config-file.lua]

Options are currently limited to:

    --config   - Explicitly specify a configuration file.
    --debug    - Show debugging information.
    --single   - Process each window once, then exit.
    --version  - Show the version number.


Overview
--------

A single Lua configuration file will be parsed and executed for every
window on your system, including invisible windows, and windows in different
workspaces/virtual-desktops.

Unless you're running `kpie --single` then the configuration file
will be invoked for each window that is opened in the future, until
you terminate `kpie`.

By default `kpie` looks for `~/.kpie.lua`, but you may choose another
file when you start `kpie` via:

    $ kpie ~/.config/kpie.lua

As noted the configuration file is Lua with the addition of some
window-related primitives.  To give you a flavour this is a sample
configuration file:

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

You can install binary packages for Debian GNU/Linux from the authors
repository:

* http://packages.steve.org.uk/kpie/
   * The package will also add an entry to your system-menu at "`System Tools | Preferences | Startup Applications`"

If you prefer to build from source you can do so providing you have the
dependencies installed.  Beyond the necessities, a compiler and `make`,
you'll need:

* `libwnck-dev`
* `liblua5.1-0-dev`

Upon a Debian GNU/Linux system these may be installed via:

      sudo apt-get install libwnck-dev liblua5.1-0-dev

With the dependencies in-place you should be able to compile
the binary by simply running `make`.


Command Line Options
--------------------


Primitives
----------

The following primitives are available:

* Information
  * `window_title` - Get the title of the new window.
  * `window_type` - Get the type of the window.
     * `"WINDOW_NORMAL"`, `"WINDOW_TOOLBAR"`, etc.
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
  * `maximize_horizontally - Maximize horizontally.
  * `maximize_vertically - Maximize vertically.
  * `fullscreen` - Make the window "fullscreen".
  * `focus` - Focus the window.
  * `is_focussed` - Is the window focussed?
  * `is_maximized` - Is the window maximized?
  * `is_fullscreen` - Is the window fullscreen?
* Workspace
  * `pin` - Pin on all workspaces.
  * `unpin` - Don't pin on all workspaces.
* Movement
  * `xy` - Get/Set the X/Y coordinates of a window.
  * `size` - Get/Set the width/height of a window.
* Workspaces
  * `activate_workspace` - Change to the given workspace/virtual-desktop.
  * `workspace` - Get/set the workspace the window is active on.
    * The return value may be -1 if the window is pinned, or invisible.
  * `workspaces` - Get/set the number of workspaces/virtual-desktops.
* Misc
  * `kill` - Close the window, forcibly.
  * `exists` - Does the given file/directory exist?
    * See [samples/includes.lua](samples/includes.lua)
  * `pointer` - Get/Set the position of the mouse pointer.
  * `readdir` - Read the contents of a directory.
      * See [samples/includes.lua](samples/includes.lua)
* Variables
  * `DEBUG` will be declared as a boolean false value, unless you launch `kpie` with the `--debug` flag.
    * This can be used to control whether you output debugging information.
  * `CONFIG` will be defined as the path to the configuration file loaded.
  * `VERSION` will be an integer containing the `kpie` release.

Some of these primitives are [documented in the sample-scripts](samples/).

Steve
--
