--
-- Media-players should default to being "always on top".
--
-- Here we cope with mplayer, totem, and xine.
--
--
--
-- Usage:
--
--    kpie [--single] ./samples/media.lua
--


--
-- Xine will update its title/application/class to include the
-- name of the media that is being played.
--
-- To cope with that we have to take the substring for comparison
-- purposes.
--
if ( ( string.sub(window_application(), 0, 5) == "xine:" ) or
     ( window_application() == "MPlayer" ) or
     ( window_application() == "Totem Movie Player" ) ) then
   above()
end

