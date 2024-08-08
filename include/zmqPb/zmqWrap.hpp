#ifndef ZMQWRAP_HPP_
#define ZMQWRAP_HPP_

#include <functional>
#include <google/protobuf/message.h>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <utility>
#include <zmq.hpp>

#include "zmqPb/_export.hpp"
#include "zmqPb/subscription.hpp"

namespace ZmqPb {

ZMQPB_API_CLASSES class ZmqWrap {
  public:
  ZmqWrap() = delete;
  ZmqWrap( ZmqWrap const& ) = delete;
  ZmqWrap( std::string const& host, bool isServer, zmq::socket_type socketType, zmq::context_t* contextToUse );
  virtual ~ZmqWrap();

  void subscribe( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback );
  virtual void sendMessage( google::protobuf::Message* message );

  virtual void run();

  protected:
  virtual bool canSend() const = 0;
  virtual void didSend() = 0;
  virtual bool canRecv() const = 0;
  virtual void didRecv() = 0;

  void connectSocket();
  bool getIsServer() const;
  zmq::socket_t* getSocketPtr() const;

  private:
  class impl;
  impl* pimpl;
};

class ZmqWrap::impl {
  public:
  std::string host_;
  bool isServer_;

  bool ownsContext_;
  zmq::context_t* zmqContext_;
  zmq::socket_t zmqSocket_;

  std::mutex mutexForSendQueue_;
  std::queue< std::pair< std::string, std::string > > queueToSend_;
  std::map< std::string, Subscription > subscribedMessages_;
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
    // std::pair< std::string, std::string > const& tmp = pimpl->queueToSend_.front();
    pimpl->queueToSend_.pop();
  }
  pimpl->subscribedMessages_.clear();
  pimpl->zmqSocket_.close();
  if( pimpl->ownsContext_ ) {
    pimpl->zmqContext_->shutdown();
  }
  delete pimpl;
  pimpl = nullptr;
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
  std::pair< std::string, std::string > wrappedMessage;
  wrappedMessage.first = message->GetTypeName();
  wrappedMessage.second = message->SerializeAsString();
  pimpl->queueToSend_.push( wrappedMessage );
  pimpl->mutexForSendQueue_.unlock();
  delete message;
}

void ZmqWrap::run() {
  if( canSend() && !pimpl->queueToSend_.empty() ) {
    pimpl->mutexForSendQueue_.lock();
    std::pair< std::string, std::string > msgToSend = pimpl->queueToSend_.front();
    zmq::message_t* msgToSendPartOne = new zmq::message_t( msgToSend.first );
    zmq::send_result_t sendResultPartOne = pimpl->zmqSocket_.send( *msgToSendPartOne, zmq::send_flags::dontwait );
    zmq::message_t* msgToSendPartTwo = new zmq::message_t( msgToSend.second );
    zmq::send_result_t sendResultPartTwo = pimpl->zmqSocket_.send( *msgToSendPartTwo, zmq::send_flags::dontwait );
    if( sendResultPartOne && sendResultPartTwo ) {
      didSend();
      pimpl->queueToSend_.pop();
      delete msgToSendPartOne;
      delete msgToSendPartTwo;
    } else {
    }
    pimpl->mutexForSendQueue_.unlock();
  } else if( canRecv() ) {
    std::pair< std::string, std::string > receivedWrapper;
    zmq::message_t receivedReplyPartOne;
    zmq::recv_result_t recvResultPartOne = pimpl->zmqSocket_.recv( receivedReplyPartOne, zmq::recv_flags::dontwait );
    zmq::message_t receivedReplyPartTwo;
    zmq::recv_result_t recvResultPartTwo = pimpl->zmqSocket_.recv( receivedReplyPartTwo, zmq::recv_flags::dontwait );
    if( recvResultPartOne && recvResultPartTwo ) {
      didRecv();
      receivedWrapper.first = receivedReplyPartOne.to_string();
      receivedWrapper.second = receivedReplyPartTwo.to_string();
      auto found = pimpl->subscribedMessages_.find( receivedWrapper.first );
      if( found != pimpl->subscribedMessages_.end() ) {
        found->second.getMessage()->ParseFromString( receivedWrapper.second );
        found->second.getCallback()( *( found->second.getMessage() ) );
      } else {
        throw std::runtime_error( "Topic '" + receivedWrapper.first + "' not subscribed!" );
      }
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

#endif /* ZMQWRAP_HPP_ */
