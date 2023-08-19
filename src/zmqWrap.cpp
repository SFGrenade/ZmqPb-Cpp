#include "zmqPb/zmqWrap.hpp"

#include <fmt/core.h>

#include "zmqWrap.pb.h"

namespace ZmqPb {

Subscription::Subscription() : message( nullptr ), callback( nullptr ) {}
Subscription::Subscription( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback )
    : message( message ), callback( callback ) {}

ZmqWrap::ZmqWrap( std::string const& host, uint16_t port, zmq::socket_type socketType )
    : host_( host ), port_( port ), zmqContext_( 1 ), zmqSocket_( zmqContext_, socketType ), queueToSend_() {
  zmqSocket_.set( zmq::sockopt::linger, 0 );  // don't wait after destructor is called
}

ZmqWrap::~ZmqWrap() {
  while( !queueToSend_.empty() ) {
    zmq::message_t* tmp = queueToSend_.front();
    if( tmp ) {
      delete tmp;
    }
    queueToSend_.pop();
  }
  for( auto pair : subscribedMessages_ ) {
    delete pair.second.message;
  }
  subscribedMessages_.clear();
  zmqSocket_.close();
  zmqContext_.shutdown();
}

void ZmqWrap::subscribe( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback ) {
  std::string messageType = message->GetTypeName();
  auto found = subscribedMessages_.find( messageType );
  if( found != subscribedMessages_.end() ) {
    subscribedMessages_[messageType].callback = callback;
  } else {
    subscribedMessages_[messageType] = Subscription{ message, callback };
  }
}

void ZmqWrap::sendMessage( google::protobuf::Message* message ) {
  mutexForSendQueue_.lock();
  ZmqPb::Proto::Wrapper* wrappedMessage = new ZmqPb::Proto::Wrapper();
  wrappedMessage->set_protoname( message->GetTypeName() );
  wrappedMessage->set_protocontent( message->SerializeAsString() );
  zmq::message_t* newMessage = new zmq::message_t( wrappedMessage->SerializeAsString() );
  queueToSend_.push( newMessage );
  mutexForSendQueue_.unlock();
  delete wrappedMessage;
  delete message;
}

void ZmqWrap::run() {
  if( canSend() && !queueToSend_.empty() ) {
    mutexForSendQueue_.lock();
    zmq::message_t* msgToSend = queueToSend_.front();
    zmq::send_result_t sendResult = zmqSocket_.send( *msgToSend, zmq::send_flags::dontwait );
    if( sendResult ) {
      queueToSend_.pop();
      delete msgToSend;
      didSend();
    } else {
    }
    mutexForSendQueue_.unlock();
  } else if( canRecv() ) {
    zmq::message_t receivedReply;
    ZmqPb::Proto::Wrapper receivedWrapper;
    zmq::recv_result_t recvResult = zmqSocket_.recv( receivedReply, zmq::recv_flags::dontwait );
    if( recvResult ) {
      receivedWrapper.ParseFromString( receivedReply.to_string() );
      auto found = subscribedMessages_.find( receivedWrapper.protoname() );
      if( found != subscribedMessages_.end() ) {
        found->second.message->ParseFromString( receivedWrapper.protocontent() );
        found->second.callback( *( found->second.message ) );
        didRecv();
      } else {
        throw std::runtime_error( fmt::format( "Topic '{}' not subscribed!", receivedWrapper.protoname() ) );
      }
    } else {
    }
  }
}

}  // namespace ZmqPb
