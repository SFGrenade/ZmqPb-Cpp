#ifndef PUSHPULL_HPP_
#define PUSHPULL_HPP_

#include "zmqPb/_export.hpp"
#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class ZMQPB_API_CLASSES PushPull : public ZmqWrap {
  public:
  PushPull( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~PushPull() override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;
};

PushPull::PushPull( std::string const& host, bool isServer, zmq::context_t* contextToUse )
    : ZmqWrap( host, isServer, isServer ? zmq::socket_type::push : zmq::socket_type::pull, contextToUse ) {
  connectSocket();
}

PushPull::~PushPull() {}

bool PushPull::canSend() const {
  return getIsServer();
}

void PushPull::didSend() {}

bool PushPull::canRecv() const {
  return !getIsServer();
}

void PushPull::didRecv() {}

}  // namespace ZmqPb

#endif /* PUSHPULL_HPP_ */
