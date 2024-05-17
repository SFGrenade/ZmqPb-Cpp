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
#include "zmqPb/subscription.hpp"

namespace ZmqPb {

class ZMQPB_API ZmqWrap {
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

}  // namespace ZmqPb

#endif /* ZMQWRAP_HPP_ */
