#include "zmqPb/pushPull.hpp"

#include <fmt/core.h>

namespace ZmqPb {

PushPull::PushPull( std::string const& host, bool isServer )
    : ZmqWrap( host, isServer ? zmq::socket_type::push : zmq::socket_type::pull ), isServer_( isServer ) {
  if( isServer_ ) {
    zmqSocket_.bind( host_ );
  } else {
    zmqSocket_.connect( host_ );
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
