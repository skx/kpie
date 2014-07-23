--
-- This sample shows how you can include other files.
--

--
-- The standard way of including a file, from Lua, will work
-- just fine:
--
--    dofile( "/some/path/foo.lua" )
--


--
-- However the more common approach is to read all files from
-- a particular directory, such as ~/.kpie.d/
--
-- The following code does that.


--
-- We'll read ~/.kpie.d/
--
dir = os.getenv( "HOME" ) .. "/.kpie.d/"

--
-- If the directory exists..
--
if ( exists( dir ) ) then

   --
   --  Read the contents
   --
   ents = readdir( dir )

   --
   -- If any of the files matches the pattern "*.lua" then load them.
   --
   for k,v in ipairs( ents ) do
      if ( string.match( v, "\.lua$" ) ) then
         if ( DEBUG ) then print( "Loading file: " .. dir .. v ) end
         dofile( dir .. v )
      end
   end

end
