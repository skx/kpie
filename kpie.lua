--
-- If kpie is launched with --debug then the DEBUG variable will be
-- set to "true".
--
-- Otherwise it will be set to "false" and this block will not be executed.
--
if ( DEBUG ) then

   print( "KPie version " .. VERSION .. " running with config-file " .. CONFIG )

   if ( not shown ) then
      shown = true
      print( "Screen width : " .. screen_width() )
      print( "Screen height: " .. screen_height() )
   end

   print( "Title " .. window_title() )
   print( "\tApplication: " .. window_application() )
   print( "\tClass " .. window_class() )

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