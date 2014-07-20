KPie
----

A simple window manipulation tool, modeled after devil's pie.

The configuration is dynamic and built around lua.


Overview
--------

If the file `~/.kpie.lua` exists then it will be parsed every time a new
window is opened upon your system.

The configuration file is pure-lua with the addition of some window-related
primitives.  To give you a flavour this is the default configuration file:


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



Installation
------------

Install the dependencies:


      sudo apt-get install libwnck-de
      sudo apt-get install liblua5.1-0-dev

Then build via:

    make



Primitives
----------

The following primitives are available:

* Information
  * `window_title` - Get the title of the new window.
  * `window_class` - Get the class of the new window.
  * `window_id` - Get the ID of the new window.
  * `window_pid` - Get the PID of the new window.
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
  * `move_to` - Move to the given workspace (1-8).
  * `pin` - Pin on all workspaces.
  * `unpin` - Don't pin on all workspaces.
* Movement
  * `xy` - Get/Set the X/Y coordinates of a window.
  * `size` - Get/Set the width/height of a window.
