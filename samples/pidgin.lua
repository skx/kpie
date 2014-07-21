--
-- This is my personal configuration for Pidgin.
--
-- The rough layout is a pair of windows looking like this, on workspace 2:
--
--     ---------------------------------------
--     | Chat Window           |  Buddy List |
--     |                       |             |
--     ---------------------------------------
--
-- The values are calculated by looking at the screen-size, with the
-- buddy-list occupying 1/5th of the screen estate.
--
--
--
-- Usage:
--
--    kpie [--single] ./samples/pidgin.lua
--


if ( window_class() == "Pidgin" ) then

   -- The width/height of the screen.
   width  = screen_width()
   height = screen_height()

   -- The buddy window will be 1/5 of the screen width.
   buddy = width / 5;


   --
   -- Move the buddy-window to the correct location
   --
   if ( window_title() == "Buddy List" ) then

      if ( is_maximized() ) then unmaximize() end
      if ( is_fullscreen() ) then unfullscreen() end

      -- x,y
      xy(width-buddy,0 )

      -- width,height
      size(buddy,height )
   else

      if ( is_maximized() ) then unmaximize() end
      if ( is_fullscreen() ) then unfullscreen() end

      -- x,y
      xy(0,0)

      -- width,height
      size(width-buddy,height )
   end


   -- Both windows on workspace two.
   workspace(2)
end
