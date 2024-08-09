set_project( "ZmqPb" )

set_version( "0.8.0", { build = "%Y%m%d", soname = true } )

set_warnings( "allextra" )

add_rules( "mode.debug", "mode.release", "mode.releasedbg", "mode.minsizerel" )
--add_rules( "plugin.compile_commands.autoupdate", { outputdir = ".vscode" } )

if is_plat( "windows" ) then
    -- technically 11, but abseil (dep of protobuf-cpp) needs >=14, but uses >=17 types
    set_languages( "cxx17" )

    add_cxflags( "/Zc:__cplusplus" )
    add_cxflags( "/Zc:preprocessor" )

    add_cxflags( "/permissive-" )
else
    -- technically 11, but abseil (dep of protobuf-cpp) needs >=14, but uses >=17 types
    set_languages( "c++17" )
end

add_requires( "cppzmq" )
add_requires( "protobuf-cpp" )

-- test framework
add_requires( "gtest" )

target( "ZmqPb" )
    set_kind( "headeronly" )
    set_default( true )
    set_group( "LIBS" )

    add_packages( "cppzmq", { public = true } )
    add_packages( "protobuf-cpp", { public = true } )

    add_rules( "protobuf.cpp" )

    add_includedirs( "include", { public = true } )
    add_headerfiles( "include/zmqPb/*.hpp" )

target( "ZmqPb_Tests" )
    set_kind( "binary" )
    set_default( false )
    set_group( "TESTS" )

    add_deps( "ZmqPb", { public = true } )
    add_packages( "gtest", { public = true } )

    add_files( "test/*.cpp" )
