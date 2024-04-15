#include <gtest/gtest.h>
#include <zmqPb/clientServer.hpp>
#include <zmqPb/pair.hpp>
#include <zmqPb/pubSub.hpp>
#include <zmqPb/pushPull.hpp>
#include <zmqPb/radioDish.hpp>
#include <zmqPb/reqRep.hpp>

TEST( ZmqPb, ConstructorTest01 ) {
#ifdef ZMQ_BUILD_DRAFT_API
  ZmqPb::ClientServer* network = new ZmqPb::ClientServer( "tcp://127.0.0.1:13337", false );
  EXPECT_NE( network, nullptr );
  delete network;
#else
  EXPECT_TRUE( true );
#endif
}

TEST( ZmqPb, ConstructorTest02 ) {
  ZmqPb::Pair* network = new ZmqPb::Pair( "tcp://127.0.0.1:13337", false );
  EXPECT_NE( network, nullptr );
  delete network;
}

TEST( ZmqPb, ConstructorTest03 ) {
  ZmqPb::PubSub* network = new ZmqPb::PubSub( "tcp://127.0.0.1:13337", false );
  EXPECT_NE( network, nullptr );
  delete network;
}

TEST( ZmqPb, ConstructorTest04 ) {
  ZmqPb::PushPull* network = new ZmqPb::PushPull( "tcp://127.0.0.1:13337", false );
  EXPECT_NE( network, nullptr );
  delete network;
}

TEST( ZmqPb, ConstructorTest05 ) {
#ifdef ZMQ_BUILD_DRAFT_API
  ZmqPb::RadioDish* network = new ZmqPb::RadioDish( "tcp://127.0.0.1:13337", false );
  EXPECT_NE( network, nullptr );
  delete network;
#else
  EXPECT_TRUE( true );
#endif
}

TEST( ZmqPb, ConstructorTest06 ) {
  ZmqPb::ReqRep* network = new ZmqPb::ReqRep( "tcp://127.0.0.1:13337", false );
  EXPECT_NE( network, nullptr );
  delete network;
}
