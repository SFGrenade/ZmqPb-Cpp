#include "zmqPb/zmqWrap.hpp"

#include "zmqWrap.pb.h"

namespace ZmqPb {

Subscription::Subscription() : message( nullptr ), callback( nullptr ) {}
Subscription::Subscription( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback )
    : message( message ), callback( callback ) {}

ZmqWrap::ZmqWrap( std::string const& host, zmq::socket_type socketType, zmq::context_t* contextToUse )
    : host_( host ),
      ownsContext_( contextToUse == nullptr ),
      zmqContext_( ownsContext_ ? new zmq::context_t( 1 ) : contextToUse ),
      zmqSocket_( *zmqContext_, socketType ),
      queueToSend_() {
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
  if( ownsContext_ ) {
    zmqContext_->shutdown();
  }
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
      didSend();
      queueToSend_.pop();
      delete msgToSend;
    } else {
    }
    mutexForSendQueue_.unlock();
  } else if( canRecv() ) {
    zmq::message_t receivedReply;
    ZmqPb::Proto::Wrapper receivedWrapper;
    zmq::recv_result_t recvResult = zmqSocket_.recv( receivedReply, zmq::recv_flags::dontwait );
    if( recvResult ) {
      didRecv();
      receivedWrapper.ParseFromString( receivedReply.to_string() );
      auto found = subscribedMessages_.find( receivedWrapper.protoname() );
      if( found != subscribedMessages_.end() ) {
        found->second.message->ParseFromString( receivedWrapper.protocontent() );
        found->second.callback( *( found->second.message ) );
      } else {
        throw std::runtime_error( "Topic '" + receivedWrapper.protoname() + "' not subscribed!" );
      }
    } else {
    }
  }
}

}  // namespace ZmqPb
