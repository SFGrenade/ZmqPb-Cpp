#ifndef ZMQWRAP_HPP_
#define ZMQWRAP_HPP_

#include <functional>
#include <google/protobuf/message.h>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <zmq.hpp>

namespace ZmqPb {

struct Subscription {
  Subscription();
  Subscription( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback );
  google::protobuf::Message* message;
  std::function< void( google::protobuf::Message const& ) > callback;
};

class ZmqWrap {
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
  std::map< std::string, Subscription > subscribedMessages_;
};

}  // namespace ZmqPb

#endif /* ZMQWRAP_HPP_ */
