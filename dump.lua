
--
-- Show some information just the once.
--
if ( not shown ) then
    print( "-- Screen width : " .. screen_width() )
    print( "-- Screen height: " .. screen_height() )
    shown = true
end

--
-- Get the title and class of each window.
--
title = window_title()
class = window_class()

--
-- Get the position and size of each window.
--
x,y = xy()
w,h = size()


--
-- Write out the config in a way that can be manipulated.
--
print( "if ( ( window_title() == \"" .. title .. "\" ) and " )
print( "     ( window_class() == \"" .. class .. "\" ) ) then"  )
print( "     xy(" .. x .. "," .. y .. " )" )
print( "     size(" .. w .. "," .. h .. " )" )
print( "     workspace(" .. workspace() .. ")" )
-- todo: if maximized .. end
-- todo: if fullscreen .. end
print("end" )
