#ifndef PUBSUB_HPP_
#define PUBSUB_HPP_

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class PubSub : public ZmqWrap {
  public:
  PubSub( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~PubSub() override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;
};

PubSub::PubSub( std::string const& host, bool isServer, zmq::context_t* contextToUse )
    : ZmqWrap( host, isServer, isServer ? zmq::socket_type::pub : zmq::socket_type::sub, contextToUse ) {
  if( !getIsServer() ) {
    getSocketPtr()->set( zmq::sockopt::subscribe, "" );  // subscribe to all incoming messages
  }
  connectSocket();
}

PubSub::~PubSub() {}

bool PubSub::canSend() const {
  return getIsServer();
}

void PubSub::didSend() {}

bool PubSub::canRecv() const {
  return !getIsServer();
}

void PubSub::didRecv() {}

}  // namespace ZmqPb

#endif /* PUBSUB_HPP_ */
