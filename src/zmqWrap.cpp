#include "zmqWrap.hpp"

#include <fmt/core.h>

#include "main.pb.h"

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
    google::protobuf::Message* tmp = queueToSend_.front();
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
  queueToSend_.push( message );
  mutexForSendQueue_.unlock();
}

void ZmqWrap::run() {
  if( canSend() && !queueToSend_.empty() ) {
    mutexForSendQueue_.lock();
    google::protobuf::Message* msgToSend = queueToSend_.front();
    ZmqPb::Proto::Wrapper* actualMessage = new ZmqPb::Proto::Wrapper();
    actualMessage->set_protoname( msgToSend->GetTypeName() );
    actualMessage->set_protocontent( msgToSend->SerializeAsString() );
    zmq::send_result_t sendResult = zmqSocket_.send( zmq::buffer( actualMessage->SerializeAsString() ), zmq::send_flags::dontwait );
    delete actualMessage;
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
