#include "zmqPb/pair.hpp"

namespace ZmqPb {

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
