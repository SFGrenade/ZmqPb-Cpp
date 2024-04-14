#include "zmqPb/pushPull.hpp"

namespace ZmqPb {

PushPull::PushPull( std::string const& host, bool isServer, zmq::context_t* contextToUse )
    : ZmqWrap( host, isServer ? zmq::socket_type::push : zmq::socket_type::pull, contextToUse ), isServer_( isServer ) {
  if( isServer_ ) {
    getSocketPtr()->bind( host );
  } else {
    getSocketPtr()->connect( host );
  }
}

PushPull::~PushPull() {}

bool PushPull::canSend() const {
  return isServer_;
}

void PushPull::didSend() {}

bool PushPull::canRecv() const {
  return !isServer_;
}

void PushPull::didRecv() {}

}  // namespace ZmqPb
