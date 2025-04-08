#include "zmqPb/zmqWrap.hpp"

#include <map>
#include <mutex>
#include <queue>
#include <utility>

#include "zmqPb/proto/wrap.pb.h"

namespace ZmqPb {

// typedef std::string ZmqpbSubscriptionStringType;
typedef absl::string_view ZmqpbSubscriptionStringType;

class ZmqWrap::impl {
  public:
  std::string host_;
  bool isServer_;

  bool ownsContext_;
  zmq::context_t* zmqContext_;
  zmq::socket_t zmqSocket_;

  std::mutex mutexForSendQueue_;
  std::queue< zmq::message_t* > queueToSend_;
  std::map< ZmqpbSubscriptionStringType, Subscription > subscribedMessages_;
};

ZmqWrap::ZmqWrap( std::string const& host, bool isServer, zmq::socket_type socketType, zmq::context_t* contextToUse ) : pimpl( new impl ) {
  pimpl->host_ = host;
  pimpl->isServer_ = isServer;
  pimpl->ownsContext_ = contextToUse == nullptr;
  pimpl->zmqContext_ = pimpl->ownsContext_ ? new zmq::context_t( 1 ) : contextToUse;
  pimpl->zmqSocket_ = zmq::socket_t( *pimpl->zmqContext_, socketType );
  pimpl->zmqSocket_.set( zmq::sockopt::linger, 0 );  // don't wait after destructor is called
}

ZmqWrap::~ZmqWrap() {
  while( !pimpl->queueToSend_.empty() ) {
    zmq::message_t* tmp = pimpl->queueToSend_.front();
    delete tmp;
    pimpl->queueToSend_.pop();
  }
  pimpl->subscribedMessages_.clear();
  pimpl->zmqSocket_.close();
  if( pimpl->ownsContext_ ) {
    pimpl->zmqContext_->shutdown();
    delete pimpl->zmqContext_;
  }
  delete pimpl;
  pimpl = nullptr;
}

void ZmqWrap::subscribe( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback ) {
  ZmqpbSubscriptionStringType messageType = message->GetTypeName();
  auto found = pimpl->subscribedMessages_.find( messageType );
  if( found == pimpl->subscribedMessages_.end() ) {
    pimpl->subscribedMessages_.emplace( messageType, Subscription{ message, callback } );
    // pimpl->subscribedMessages_[messageType] = Subscription{ message, callback };
  }
}

void ZmqWrap::sendMessage( google::protobuf::Message* message ) {
  pimpl->mutexForSendQueue_.lock();
  ZmqPb::Proto::Wrap* wrappedMessage = new ZmqPb::Proto::Wrap();
  wrappedMessage->set_topic( message->GetTypeName() );
  wrappedMessage->set_message( message->SerializeAsString() );
  zmq::message_t* serializedWrappedMessage = new zmq::message_t( wrappedMessage->SerializeAsString() );
  pimpl->queueToSend_.push( serializedWrappedMessage );
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
      delete msgToSend;
      pimpl->queueToSend_.pop();
    } else {
    }
    pimpl->mutexForSendQueue_.unlock();
  } else if( canRecv() ) {
    zmq::message_t receivedReply;
    zmq::recv_result_t recvResult = pimpl->zmqSocket_.recv( receivedReply, zmq::recv_flags::dontwait );
    if( recvResult.has_value() ) {
    }
    if( recvResult ) {
      didRecv();
      ZmqPb::Proto::Wrap* wrappedMessage = new ZmqPb::Proto::Wrap();
      wrappedMessage->ParseFromString( receivedReply.to_string() );
      auto found = pimpl->subscribedMessages_.find( wrappedMessage->topic() );
      if( found != pimpl->subscribedMessages_.end() ) {
        found->second.getMessage()->ParseFromString( wrappedMessage->message() );
        found->second.getCallback()( *( found->second.getMessage() ) );
      } else {
        delete wrappedMessage;
        throw std::runtime_error( "Topic '" + wrappedMessage->topic() + "' not subscribed!" );
      }
      delete wrappedMessage;
    } else {
    }
  }
}

void ZmqWrap::connectSocket() {
  if( pimpl->isServer_ ) {
    pimpl->zmqSocket_.bind( pimpl->host_ );
  } else {
    pimpl->zmqSocket_.connect( pimpl->host_ );
  }
}

bool ZmqWrap::getIsServer() const {
  return pimpl->isServer_;
}

zmq::socket_t* ZmqWrap::getSocketPtr() const {
  return &pimpl->zmqSocket_;
}

}  // namespace ZmqPb
