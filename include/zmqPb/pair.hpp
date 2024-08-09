#ifndef PAIR_HPP_
#define PAIR_HPP_

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class Pair : public ZmqWrap {
  public:
  Pair( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~Pair() override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;
};

Pair::Pair( std::string const& host, bool isServer, zmq::context_t* contextToUse ) : ZmqWrap( host, isServer, zmq::socket_type::pair, contextToUse ) {
  connectSocket();
}

Pair::~Pair() {}

bool Pair::canSend() const {
  return true;
}

void Pair::didSend() {}

bool Pair::canRecv() const {
  return true;
}

void Pair::didRecv() {}

}  // namespace ZmqPb

#endif /* PAIR_HPP_ */
