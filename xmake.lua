set_project( "ZmqPb" )

set_version( "0.5.0", { build = "%Y%m%d", soname = true } )

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

add_requireconfs( "*", { configs = { shared = get_config( "kind" ) == "shared" } } )

add_requires( "cppzmq" )
add_requires( "protobuf-cpp" )
-- protobuf-* needs it and somehow just doesn't publicizes the linkage
--add_requires( "abseil" )
add_requires( "utf8_range" )

-- test framework
add_requires( "gtest" )

target( "ZmqPb" )
    set_kind( "$(kind)" )
    set_default( true )
    set_group( "LIBS" )

    if is_kind( "shared" ) then
        if is_plat( "linux" ) then
            add_defines( "ZMQPB_DO_EXPORT_LINUX" )
        elseif is_plat( "macosx" ) then
            add_defines( "ZMQPB_DO_EXPORT_MACOSX" )
        elseif is_plat( "windows" ) then
            add_defines( "ZMQPB_DO_EXPORT_WINDOWS" )
        end
    end

    add_packages( "cppzmq", { public = true } )
    add_packages( "protobuf-cpp", { public = true } )
    -- protobuf-* needs it and somehow just doesn't publicizes the linkage
    --add_packages( "abseil", { public = true } )
    add_packages( "utf8_range", { public = true } )

    add_rules( "protobuf.cpp" )

    add_includedirs( "include", { public = true } )
    add_headerfiles( "include/(zmqPb/*.hpp)" )
    add_files( "proto/zmqPb/*.proto", { proto_public = false } )
    add_files( "src/*.cpp" )

target( "ZmqPb_Tests" )
    set_kind( "binary" )
    set_default( false )
    set_group( "TESTS" )

    add_deps( "ZmqPb", { shared = get_config( "kind" ) == "shared", public = true } )
    add_packages( "gtest", { public = true } )

    add_files( "test/*.cpp" )
    if is_plat( "windows" ) then
        --add_tests( "default" )
    else
        add_tests( "default" )
    end
