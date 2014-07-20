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


--
-- If you want to debug things you could uncomment this to see the windows right now.
--

--
-- print( "Window Title: " .. window_title() )
-- print( "\tClass: " .. window_class() )
--

--
-- x,y = xy()
-- width,height = size()
-- print ( "X,Y: " .. x .. "," .. y .. " Width/Height:" .. width .. "," .. height );
--

