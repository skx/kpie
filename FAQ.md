
Why the name KPie?
------------------

Based on the obvious simularity to devilspie I wanted to call
this tool cherrypie, which is my favourite kind of pie.

The name seemed a little dodgy though so I chose kpie, via the
Finnish word "kirsi" meaning "cherry".


How can I include other files?
------------------------------

You can include other Lua files by use of the `dofile` primitive:

     dofile( "test.lua" )

However this does not allow you to test for a files existance, or handle
cases such as "include ~/.config/*.lua" - for that see the next question.



How can I handle wildcards?
---------------------------

kpie contains a `readdir` primitive, which will allow you to scan a
directory looking for files.  This could be used like so:


    --
    -- Read the directory entries beneath ~/.kpie.d/
    --
    dir = os.getenv( "HOME" ) .. "/.kpie.d/"
    ents = readdir( dir )

    --
    -- If a file is found that matches "*.lua" then load it.
    --
    for k,v in ipairs( ents ) do
        if ( string.match( v, "\.lua$" ) ) then
            if ( DEBUG ) then print( "Loading file: " .. dir .. v ) end
            dofile( dir .. v )
        end
    end

