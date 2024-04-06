set_project( "ZmqPb" )

set_version( "0.5.0", { build = "%Y%m%d", soname = true } )

set_warnings( "allextra" )

add_rules( "mode.debug", "mode.release", "mode.releasedbg", "mode.minsizerel" )
add_rules( "plugin.compile_commands.autoupdate", { outputdir = ".vscode" } )

if is_plat( "windows" ) then
    -- technically 11, but abseil (dep of protobuf-cpp) needs >=14
    set_languages( "cxx14" )

    add_cxflags( "/Zc:__cplusplus" )
    add_cxflags( "/Zc:preprocessor" )

    add_cxflags( "/permissive-" )
else
    -- technically 11, but abseil (dep of protobuf-cpp) needs >=14
    set_languages( "c++14" )
end

add_requireconfs( "*", { configs = { shared = get_config( "kind" ) == "shared" } } )

add_requires( "cppzmq" )
add_requires( "protobuf-cpp" )

target( "ZmqPb" )
    set_kind( "$(kind)" )

    add_packages( "cppzmq", { public = true } )
    add_packages( "protobuf-cpp", { public = true } )

    add_rules( "protobuf.cpp" )

    add_includedirs( "include", { public = true } )
    add_headerfiles( "include/(zmqPb/*.hpp)" )
    add_files( "proto/zmqPb/*.proto", { proto_public = false } )
    add_files( "src/*.cpp" )
