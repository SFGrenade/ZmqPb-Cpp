#ifdef ZMQ_BUILD_DRAFT_API
#include "zmqPb/radioDish.hpp"

#include <fmt/core.h>

namespace ZmqPb {

RadioDish::RadioDish( std::string const& host, uint16_t port, bool isServer )
    : ZmqWrap( host, port, isServer ? zmq::socket_type::server : zmq::socket_type::client ), isServer_( isServer ) {
  if( isServer_ ) {
    zmqSocket_.bind( fmt::format( "{}:{}", host_, port_ ) );
  } else {
    // todo: have groups for the sockets, needs to overwrite/overload
    zmqSocket_.join( "Test" );
    zmqSocket_.connect( fmt::format( "{}:{}", host_, port_ ) );
  }
}

RadioDish::~RadioDish() {}

void RadioDish::sendMessage( google::protobuf::Message* message ) {
  mutexForSendQueue_.lock();
  ZmqPb::Proto::Wrapper* wrappedMessage = new ZmqPb::Proto::Wrapper();
  wrappedMessage->set_protoname( message->GetTypeName() );
  wrappedMessage->set_protocontent( message->SerializeAsString() );
  zmq::message_t* newMessage = new zmq::message_t( wrappedMessage->SerializeAsString() );

  // todo: have groups for the messages, needs to overwrite/overload
  newMessage->set_group( "Test" );

  queueToSend_.push( newMessage );
  mutexForSendQueue_.unlock();
  delete wrappedMessage;
  delete message;
}

bool RadioDish::canSend() const {
  return true;
}

void RadioDish::didSend() {}

bool RadioDish::canRecv() const {
  return true;
}

void RadioDish::didRecv() {}

}  // namespace ZmqPb
#endif /* ZMQ_BUILD_DRAFT_API */
