#include <gtest/gtest.h>
#include <zmqPb/clientServer.hpp>
#include <zmqPb/pair.hpp>
#include <zmqPb/pubSub.hpp>
#include <zmqPb/pushPull.hpp>
#include <zmqPb/radioDish.hpp>
#include <zmqPb/reqRep.hpp>

#include "test/proto/test.pb.h"

TEST( ZmqPb, ConstructorTest01 ) {
#ifdef ZMQ_BUILD_DRAFT_API
  ZmqPb::ClientServer* server = new ZmqPb::ClientServer( "tcp://127.0.0.1:20010", true );
  EXPECT_NE( server, nullptr );
  zmq_sleep( 1 );
  ZmqPb::ClientServer* client = new ZmqPb::ClientServer( "tcp://127.0.0.1:20010", false );
  EXPECT_NE( client, nullptr );
  zmq_sleep( 1 );

  bool checkValue = false;
  bool oneMoreLoop = false;

  server->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  client->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  ZmqPb::Test::TestMessage* msgToSend1 = new ZmqPb::Test::TestMessage();
  msgToSend1->set_test( "test" );
  server->sendMessage( msgToSend1 );
  ZmqPb::Test::TestMessage* msgToSend2 = new ZmqPb::Test::TestMessage();
  msgToSend2->set_test( "test" );
  client->sendMessage( msgToSend2 );

  for( int _ = 0; ( _ < 10 ) && !oneMoreLoop; _++ ) {
    oneMoreLoop = checkValue;
    EXPECT_NO_THROW( server->run() );
    EXPECT_NO_THROW( client->run() );
    zmq_sleep( 1 );
  }

  EXPECT_TRUE( checkValue );

  delete client;
  delete server;
#else
  EXPECT_TRUE( true );
#endif
}

TEST( ZmqPb, ConstructorTest02 ) {
  ZmqPb::Pair* server = new ZmqPb::Pair( "tcp://127.0.0.1:20020", true );
  EXPECT_NE( server, nullptr );
  zmq_sleep( 1 );
  ZmqPb::Pair* client = new ZmqPb::Pair( "tcp://127.0.0.1:20020", false );
  EXPECT_NE( client, nullptr );
  zmq_sleep( 1 );

  bool checkValue = false;
  bool oneMoreLoop = false;

  server->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  client->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  ZmqPb::Test::TestMessage* msgToSend1 = new ZmqPb::Test::TestMessage();
  msgToSend1->set_test( "test" );
  server->sendMessage( msgToSend1 );
  ZmqPb::Test::TestMessage* msgToSend2 = new ZmqPb::Test::TestMessage();
  msgToSend2->set_test( "test" );
  client->sendMessage( msgToSend2 );

  for( int _ = 0; ( _ < 10 ) && !oneMoreLoop; _++ ) {
    oneMoreLoop = checkValue;
    EXPECT_NO_THROW( server->run() );
    EXPECT_NO_THROW( client->run() );
    zmq_sleep( 1 );
  }

  EXPECT_TRUE( checkValue );

  delete client;
  delete server;
}

TEST( ZmqPb, ConstructorTest03 ) {
  ZmqPb::PubSub* server = new ZmqPb::PubSub( "tcp://127.0.0.1:20030", true );
  EXPECT_NE( server, nullptr );
  zmq_sleep( 1 );
  ZmqPb::PubSub* client = new ZmqPb::PubSub( "tcp://127.0.0.1:20030", false );
  EXPECT_NE( client, nullptr );
  zmq_sleep( 1 );

  bool checkValue = false;
  bool oneMoreLoop = false;

  server->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  client->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  ZmqPb::Test::TestMessage* msgToSend1 = new ZmqPb::Test::TestMessage();
  msgToSend1->set_test( "test" );
  server->sendMessage( msgToSend1 );
  ZmqPb::Test::TestMessage* msgToSend2 = new ZmqPb::Test::TestMessage();
  msgToSend2->set_test( "test" );
  client->sendMessage( msgToSend2 );

  for( int _ = 0; ( _ < 10 ) && !oneMoreLoop; _++ ) {
    oneMoreLoop = checkValue;
    EXPECT_NO_THROW( server->run() );
    EXPECT_NO_THROW( client->run() );
    zmq_sleep( 1 );
  }

  EXPECT_TRUE( checkValue );

  delete client;
  delete server;
}

TEST( ZmqPb, ConstructorTest04 ) {
  ZmqPb::PushPull* server = new ZmqPb::PushPull( "tcp://127.0.0.1:20040", true );
  EXPECT_NE( server, nullptr );
  zmq_sleep( 1 );
  ZmqPb::PushPull* client = new ZmqPb::PushPull( "tcp://127.0.0.1:20040", false );
  EXPECT_NE( client, nullptr );
  zmq_sleep( 1 );

  bool checkValue = false;
  bool oneMoreLoop = false;

  client->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  ZmqPb::Test::TestMessage* msgToSend1 = new ZmqPb::Test::TestMessage();
  msgToSend1->set_test( "test" );
  server->sendMessage( msgToSend1 );

  for( int _ = 0; ( _ < 10 ) && !oneMoreLoop; _++ ) {
    oneMoreLoop = checkValue;
    EXPECT_NO_THROW( server->run() );
    EXPECT_NO_THROW( client->run() );
    zmq_sleep( 1 );
  }

  EXPECT_TRUE( checkValue );

  delete client;
  delete server;
}

TEST( ZmqPb, ConstructorTest05 ) {
#ifdef ZMQ_BUILD_DRAFT_API
  ZmqPb::RadioDish* server = new ZmqPb::RadioDish( "tcp://127.0.0.1:20050", true );
  EXPECT_NE( server, nullptr );
  zmq_sleep( 1 );
  ZmqPb::RadioDish* client = new ZmqPb::RadioDish( "tcp://127.0.0.1:20050", false );
  EXPECT_NE( client, nullptr );
  zmq_sleep( 1 );

  bool checkValue = false;
  bool oneMoreLoop = false;

  server->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  client->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  ZmqPb::Test::TestMessage* msgToSend1 = new ZmqPb::Test::TestMessage();
  msgToSend1->set_test( "test" );
  server->sendMessage( msgToSend1 );
  ZmqPb::Test::TestMessage* msgToSend2 = new ZmqPb::Test::TestMessage();
  msgToSend2->set_test( "test" );
  client->sendMessage( msgToSend2 );

  for( int _ = 0; ( _ < 10 ) && !oneMoreLoop; _++ ) {
    oneMoreLoop = checkValue;
    EXPECT_NO_THROW( server->run() );
    EXPECT_NO_THROW( client->run() );
    zmq_sleep( 1 );
  }

  EXPECT_TRUE( checkValue );

  delete client;
  delete server;
#else
  EXPECT_TRUE( true );
#endif
}

TEST( ZmqPb, ConstructorTest06 ) {
  ZmqPb::ReqRep* server = new ZmqPb::ReqRep( "tcp://127.0.0.1:20060", true );
  EXPECT_NE( server, nullptr );
  zmq_sleep( 1 );
  ZmqPb::ReqRep* client = new ZmqPb::ReqRep( "tcp://127.0.0.1:20060", false );
  EXPECT_NE( client, nullptr );
  zmq_sleep( 1 );

  bool checkValue = false;
  bool oneMoreLoop = false;

  server->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  client->subscribe( new ZmqPb::Test::TestMessage(), [&checkValue]( google::protobuf::Message const& msg ) {
    ZmqPb::Test::TestMessage const& actualMsg = static_cast< ZmqPb::Test::TestMessage const& >( msg );
    EXPECT_STREQ( "test", actualMsg.test().c_str() );
    checkValue = true;
  } );
  ZmqPb::Test::TestMessage* msgToSend1 = new ZmqPb::Test::TestMessage();
  msgToSend1->set_test( "test" );
  server->sendMessage( msgToSend1 );
  ZmqPb::Test::TestMessage* msgToSend2 = new ZmqPb::Test::TestMessage();
  msgToSend2->set_test( "test" );
  client->sendMessage( msgToSend2 );

  for( int _ = 0; ( _ < 10 ) && !oneMoreLoop; _++ ) {
    oneMoreLoop = checkValue;
    EXPECT_NO_THROW( server->run() );
    EXPECT_NO_THROW( client->run() );
    zmq_sleep( 1 );
  }

  EXPECT_TRUE( checkValue );

  delete client;
  delete server;
}

TEST( ZmqPb, MessageTest ) {
  ZmqPb::Test::TestMessage* tmpMessage = new ZmqPb::Test::TestMessage();
  delete tmpMessage;
}
