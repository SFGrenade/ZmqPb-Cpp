add_rules( "mode.debug", "mode.release" )
add_rules( "plugin.compile_commands.autoupdate", { outputdir = ".vscode" } )

set_project( "ZmqPb" )

if is_plat( "windows" ) then
    set_languages( "cxxlatest" )

    add_cxflags( "/Zc:__cplusplus" )
    add_cxflags( "/Zc:preprocessor" )

    add_cxflags( "/permissive-" )

    set_runtimes(is_mode("debug") and "MDd" or "MD")
else
    set_languages( "cxx20" )
end

set_warnings( "allextra" )

if is_mode( "debug" ) then
    add_defines( "DEBUG" )
end
if is_mode( "release" ) then
    add_defines( "NDEBUG" )
end

add_requireconfs( "*", { configs = { shared = false } } )

add_requires( "cppzmq" )
add_requires( "protobuf-cpp" )

target( "ZmqPb" )
    set_kind( "static" )

    add_packages( "cppzmq" )
    add_packages( "protobuf-cpp", { public = false } )

    add_rules( "protobuf.cpp" )

    add_includedirs( "include" )
    add_headerfiles( "include/(zmqPb/*.hpp)" )
    add_files( "proto/zmqPb/*.proto", { proto_public = false } )
    add_files( "src/*.cpp" )
