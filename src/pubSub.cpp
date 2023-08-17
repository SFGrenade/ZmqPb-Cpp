#include "pubSub.hpp"

#include <fmt/core.h>

namespace ZmqPb {

PubSub::PubSub( std::string const& host, uint16_t port, bool isServer )
    : ZmqWrap( host, port, isServer ? zmq::socket_type::pub : zmq::socket_type::sub ),
      isServer_( isServer ),
      status_( isServer ? PubSub::Status::Sending : PubSub::Status::Receiving ) {
  if( isServer_ ) {
    zmqSocket_.bind( fmt::format( "{}:{}", host_, port_ ) );
  } else {
    zmqSocket_.set( zmq::sockopt::subscribe, "" );  // subscribe to all incoming messages
    zmqSocket_.connect( fmt::format( "{}:{}", host_, port_ ) );
  }
}

PubSub::~PubSub() {}

bool PubSub::canSend() const {
  return status_ == PubSub::Status::Sending;
}

void PubSub::didSend() {}

bool PubSub::canRecv() const {
  return status_ == PubSub::Status::Receiving;
}

void PubSub::didRecv() {}

}  // namespace ZmqPb
