
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

`kpie` contains a `readdir` primitive to allow you to read a directorys
contents.  There is also an `exists` primitive which will let you test
a file/directory exists.

Together the two primitives can allow you to include every file in a
directory, for example:


    --
    -- We'll read ~/.kpie.d/
    --
    dir = os.getenv( "HOME" ) .. "/.kpie.d/"

    --
    -- If the directory exists..
    --
    if ( exists( dir ) ) then

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
    else
        print( "Directory does not exist " .. dir )
    end
