#include "zmqPb/pushPull.hpp"

#include <fmt/core.h>

namespace ZmqPb {

PushPull::PushPull( std::string const& host, uint16_t port, bool isServer )
    : ZmqWrap( host, port, isServer ? zmq::socket_type::push : zmq::socket_type::pull ), isServer_( isServer ) {
  if( isServer_ ) {
    zmqSocket_.bind( fmt::format( "{}:{}", host_, port_ ) );
  } else {
    zmqSocket_.connect( fmt::format( "{}:{}", host_, port_ ) );
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
