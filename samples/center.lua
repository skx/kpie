--
-- Shows how to center a window.
--
-- Usage:
--
--    kpie [--single] ./center.lua
--



--
-- This function centers a window
--
function center()

   -- get the size of the window
   width,height = size()

   -- get the size of the screen
   sx = screen_width()
   sy = screen_height()

   newx = (sx/2) - (width/2)
   newy = (sy/2) - (height/2)

   xy(newx,newy)

   --
   -- Move the pointer to the middle of the window
   --
   pointer( sx/2, sy/2);

   if ( DEBUG ) then
      print( "Moving to " .. newx .. "," .. newy )
   end
end


--
--  We only process "Terminal" windows.
--
if ( ( window_title() == "Terminal" ) and
     ( window_class() == "Terminal" ) and
     ( window_application() == "Terminal" ) ) then
   center()
end
