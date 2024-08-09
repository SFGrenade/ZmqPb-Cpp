#ifdef ZMQ_BUILD_DRAFT_API
#ifndef RADIODISH_HPP_
#define RADIODISH_HPP_

#include <vector>

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class RadioDish : public ZmqWrap {
  public:
  RadioDish( std::string const& host, bool isServer, std::vector< std::string > const& joinGroups, zmq::context_t* contextToUse = nullptr );
  ~RadioDish() override;

  virtual void sendMessage( google::protobuf::Message* message, std::string const& group ) override;
  virtual void sendMessage( google::protobuf::Message* message ) override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;
};

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
  return !getIsServer();
}

void RadioDish::didSend() {}

bool RadioDish::canRecv() const {
  return getIsServer();
}

void RadioDish::didRecv() {}

}  // namespace ZmqPb

#endif /* RADIODISH_HPP_ */
#endif /* ZMQ_BUILD_DRAFT_API */
