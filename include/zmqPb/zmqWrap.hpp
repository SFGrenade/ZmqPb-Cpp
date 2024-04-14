#ifndef ZMQWRAP_HPP_
#define ZMQWRAP_HPP_

#include <functional>
#include <google/protobuf/message.h>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <zmq.hpp>

#include "zmqPb/_export.hpp"

namespace ZmqPb {

struct ZMQPB_EXPORT Subscription {
  Subscription();
  Subscription( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback );
  google::protobuf::Message* message;
  std::function< void( google::protobuf::Message const& ) > callback;
};

class ZMQPB_EXPORT ZmqWrap {
  public:
  ZmqWrap( std::string const& host, zmq::socket_type socketType, zmq::context_t* contextToUse );
  ~ZmqWrap();

  void subscribe( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback );
  virtual void sendMessage( google::protobuf::Message* message );

  virtual void run();

  protected:
  virtual bool canSend() const = 0;
  virtual void didSend() = 0;
  virtual bool canRecv() const = 0;
  virtual void didRecv() = 0;

  protected:
  std::string host_;

  bool ownsContext_;
  zmq::context_t* zmqContext_;
  zmq::socket_t zmqSocket_;

  std::mutex mutexForSendQueue_;
  std::queue< zmq::message_t* > queueToSend_;
  std::map< std::string, ZmqPb::Subscription > subscribedMessages_;
};

}  // namespace ZmqPb

#if defined( ZMQPB_DO_EXPORT_WINDOWS )

template class ZMQPB_EXPORT std::function< void( google::protobuf::Message const& ) >;
template class ZMQPB_EXPORT std::queue< zmq::message_t* >;
template class ZMQPB_EXPORT std::map< std::string, ZmqPb::Subscription >;

#endif

#endif /* ZMQWRAP_HPP_ */
