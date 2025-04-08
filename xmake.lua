set_project( "ZmqPb" )

set_version( "0.10.4", { build = "%Y%m%d", soname = true } )

set_warnings( "allextra" )

add_rules( "mode.debug", "mode.release", "mode.releasedbg", "mode.minsizerel" )

-- technically 11, but abseil (dep of protobuf-cpp) needs >=14, but uses >=17 types
set_languages( "c++17" )

add_requires( "cppzmq" )
add_requires( "hedley" )
add_requires( "protobuf-cpp" )

-- test framework
add_requires( "gtest" )

target( "ZmqPb" )
    set_kind( "$(kind)" )
    set_default( true )
    set_group( "LIBS" )

    add_defines( "ZMQPB_COMPILING" )
    if is_kind( "shared" ) then
        add_defines( "ZMQPB_IS_SHARED" )
    end

    add_packages( "cppzmq", { public = true } )
    add_packages( "hedley", { public = true } )
    add_packages( "protobuf-cpp", { public = true } )

    add_rules( "protobuf.cpp" )
    add_files( "proto/**.proto", { proto_public = false, proto_rootdir = path.join( "proto" ) } )

    add_includedirs( "include", { public = true } )
    add_headerfiles( "include/(zmqPb/*.hpp)" )
    add_files( "src/*.cpp" )

target( "ZmqPb_Tests_Messages" )
    set_kind( "static" )
    set_default( false )
    set_group( "LIBS" )

    add_deps( "ZmqPb", { public = true } )

    add_rules( "protobuf.cpp" )
    add_files( "test/proto/**.proto", { proto_public = true, proto_rootdir = path.join( "." ) } )

target( "ZmqPb_Tests" )
    set_kind( "binary" )
    set_default( false )
    set_group( "TESTS" )

    add_packages( "gtest", { public = true } )

    add_deps( "ZmqPb", { public = true } )
    add_deps( "ZmqPb_Tests_Messages", { public = true } )

    add_files( "test/*.cpp" )
