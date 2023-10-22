#include "zmqPb/pubSub.hpp"

namespace ZmqPb {

PubSub::PubSub( std::string const& host, bool isServer )
    : ZmqWrap( host, isServer ? zmq::socket_type::pub : zmq::socket_type::sub ),
      isServer_( isServer ),
      status_( isServer ? PubSub::Status::Sending : PubSub::Status::Receiving ) {
  if( isServer_ ) {
    zmqSocket_.bind( host_ );
  } else {
    zmqSocket_.set( zmq::sockopt::subscribe, "" );  // subscribe to all incoming messages
    zmqSocket_.connect( host_ );
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
