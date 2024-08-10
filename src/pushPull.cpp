#include "zmqPb/pushPull.hpp"

namespace ZmqPb {

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
