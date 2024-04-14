#include "zmqPb/zmqWrap.hpp"

#include "zmqWrap.pb.h"

namespace ZmqPb {

class ZmqWrap::impl {
  public:
  std::string host_;

  bool ownsContext_;
  zmq::context_t* zmqContext_;
  zmq::socket_t zmqSocket_;

  std::mutex mutexForSendQueue_;
  std::queue< zmq::message_t* > queueToSend_;
  std::map< std::string, Subscription > subscribedMessages_;
};

ZmqWrap::ZmqWrap( std::string const& host, zmq::socket_type socketType, zmq::context_t* contextToUse ) : pimpl( new impl ) {
  pimpl->host_ = host;
  pimpl->ownsContext_ = contextToUse == nullptr;
  pimpl->zmqContext_ = pimpl->ownsContext_ ? new zmq::context_t( 1 ) : contextToUse;
  pimpl->zmqSocket_ = zmq::socket_t( *pimpl->zmqContext_, socketType );
  pimpl->zmqSocket_.set( zmq::sockopt::linger, 0 );  // don't wait after destructor is called
}

ZmqWrap::~ZmqWrap() {
  while( !pimpl->queueToSend_.empty() ) {
    zmq::message_t* tmp = pimpl->queueToSend_.front();
    if( tmp ) {
      delete tmp;
    }
    pimpl->queueToSend_.pop();
  }
  pimpl->subscribedMessages_.clear();
  pimpl->zmqSocket_.close();
  if( pimpl->ownsContext_ ) {
    pimpl->zmqContext_->shutdown();
  }
}

void ZmqWrap::subscribe( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback ) {
  std::string messageType = message->GetTypeName();
  auto found = pimpl->subscribedMessages_.find( messageType );
  if( found == pimpl->subscribedMessages_.end() ) {
    pimpl->subscribedMessages_[messageType] = Subscription{ message, callback };
  }
}

void ZmqWrap::sendMessage( google::protobuf::Message* message ) {
  pimpl->mutexForSendQueue_.lock();
  ZmqPb::Proto::Wrapper* wrappedMessage = new ZmqPb::Proto::Wrapper();
  wrappedMessage->set_protoname( message->GetTypeName() );
  wrappedMessage->set_protocontent( message->SerializeAsString() );
  zmq::message_t* newMessage = new zmq::message_t( wrappedMessage->SerializeAsString() );
  pimpl->queueToSend_.push( newMessage );
  pimpl->mutexForSendQueue_.unlock();
  delete wrappedMessage;
  delete message;
}

void ZmqWrap::run() {
  if( canSend() && !pimpl->queueToSend_.empty() ) {
    pimpl->mutexForSendQueue_.lock();
    zmq::message_t* msgToSend = pimpl->queueToSend_.front();
    zmq::send_result_t sendResult = pimpl->zmqSocket_.send( *msgToSend, zmq::send_flags::dontwait );
    if( sendResult ) {
      didSend();
      pimpl->queueToSend_.pop();
      delete msgToSend;
    } else {
    }
    pimpl->mutexForSendQueue_.unlock();
  } else if( canRecv() ) {
    zmq::message_t receivedReply;
    ZmqPb::Proto::Wrapper receivedWrapper;
    zmq::recv_result_t recvResult = pimpl->zmqSocket_.recv( receivedReply, zmq::recv_flags::dontwait );
    if( recvResult ) {
      didRecv();
      receivedWrapper.ParseFromString( receivedReply.to_string() );
      auto found = pimpl->subscribedMessages_.find( receivedWrapper.protoname() );
      if( found != pimpl->subscribedMessages_.end() ) {
        found->second.getMessage()->ParseFromString( receivedWrapper.protocontent() );
        found->second.getCallback()( *( found->second.getMessage() ) );
      } else {
        throw std::runtime_error( "Topic '" + receivedWrapper.protoname() + "' not subscribed!" );
      }
    } else {
    }
  }
}

zmq::socket_t* ZmqWrap::getSocketPtr() {
  return &pimpl->zmqSocket_;
}

}  // namespace ZmqPb
