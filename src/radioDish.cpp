#ifdef ZMQ_BUILD_DRAFT_API
#include "zmqPb/radioDish.hpp"

#include <stdexcept>

namespace ZmqPb {

RadioDish::RadioDish( std::string const& host, bool isServer, std::vector< std::string > const& joinGroups, zmq::context_t* contextToUse )
    : ZmqWrap( host, isServer, isServer ? zmq::socket_type::dish : zmq::socket_type::radio, contextToUse ) {
  if( !getIsServer() ) {
    for( std::string joinGroup : joinGroups ) {
      getSocketPtr()->join( joinGroup );
    }
  }
  connectSocket();
}

RadioDish::~RadioDish() {}

void RadioDish::sendMessage( google::protobuf::Message* message, std::string const& group ) {
  mutexForSendQueue_.lock();
  ZmqPb::Proto::Wrapper* wrappedMessage = new ZmqPb::Proto::Wrapper();
  wrappedMessage->set_protoname( message->GetTypeName() );
  wrappedMessage->set_protocontent( message->SerializeAsString() );
  zmq::message_t* newMessage = new zmq::message_t( wrappedMessage->SerializeAsString() );

  newMessage->set_group( group );

  queueToSend_.push( newMessage );
  mutexForSendQueue_.unlock();
  delete wrappedMessage;
  delete message;
}

void RadioDish::sendMessage( google::protobuf::Message* message ) {
  throw new invalid_argument( "RadioDish::sendMessage - overloaded function without group not implemented!" );
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
