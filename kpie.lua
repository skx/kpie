--
-- Show some information just the once.
--
if ( not shown ) then
   shown = true
   print( "Screen width : " .. screen_width() )
   print( "Screen height: " .. screen_height() )
end



--
-- If Xine is launched it should be "always on top"
--
if ( window_application() == "xine" ) then
   above()
end

--
-- The xlogo program is so cool it should be visible on all
-- workspaces
--
if ( window_title() == "xlogo" ) then
   pin()
end


--
-- This configuration file is Lua, so we can look for substrings too:
--
if ( string.find( window_title(), "emacs@" ) ) then
   print( "Emacs is running and we like it!" )
   print( "Window Title: " .. window_title() )
end


--
-- As a final tip if you wish to change a string to lower-case you
-- can use the following:
--
-- print ( window_title():lower() )
--