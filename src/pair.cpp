#include "zmqPb/pair.hpp"

#include <fmt/core.h>

namespace ZmqPb {

Pair::Pair( std::string const& host, uint16_t port, bool isServer ) : ZmqWrap( host, port, zmq::socket_type::pair ), isServer_( isServer ) {
  if( isServer_ ) {
    zmqSocket_.bind( fmt::format( "{}:{}", host_, port_ ) );
  } else {
    zmqSocket_.connect( fmt::format( "{}:{}", host_, port_ ) );
  }
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
