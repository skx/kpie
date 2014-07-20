--
-- Media-players will default to "always on top"
--
--

if ( ( window_application() == "xine" ) or
     ( window_application() == "MPlayer" ) or
     ( window_application() == "Totem Movie Player" ) ) then
   above()
end
