--
-- Dump all current windows, along with their sizes and positions.
--
--
--
-- Usage:
--
--    kpie [--single] ./samples.dump.lua
--




--
-- Show some information just the once.
--
if ( not shown ) then
    print( "-- Screen width : " .. screen_width() )
    print( "-- Screen height: " .. screen_height() )
    shown = true
end

--
-- Get the title, class, and application of each window.
--
title = window_title()
class = window_class()
role  = window_role()
app   = window_application()
type  = window_type()

--
-- Get the position and size of each window.
--
x,y = xy()
w,h = size()


--
-- Write out the config in a way that can be manipulated.
--
print( "if ( ( window_title() == \"" .. title .. "\" ) and " )
print( "     ( window_class() == \"" .. class .. "\" ) and"  )
print( "     ( window_type() == \"" .. type .. "\" ) and "  )
print( "     ( window_role() == \"" .. role .. "\" ) and "  )
print( "     ( window_application() == \"" .. app .. "\" ) ) then"  )
print( "     xy(" .. x .. "," .. y .. " )" )
print( "     size(" .. w .. "," .. h .. " )" )

if ( workspace() >= 0 ) then
   print( "     workspace(" .. workspace() .. ")" )
end

if ( is_maximized() ) then
   print( "     maximize()" );
end

if ( is_minimized() ) then
   print( "     minimize()" );
end

if ( is_fullscreen() ) then
   print( "     fullscreen()" );
end

if ( is_focussed() ) then
   print( "     focus()" );
end

print("end" )
