#include "zmqPb/pubSub.hpp"

namespace ZmqPb {

PubSub::PubSub( std::string const& host, bool isServer, zmq::context_t* contextToUse )
    : ZmqWrap( host, isServer, isServer ? zmq::socket_type::pub : zmq::socket_type::sub, contextToUse ) {
  if( !getIsServer() ) {
    getSocketPtr()->set( zmq::sockopt::subscribe, "" );  // subscribe to all incoming messages
  }
  connectSocket();
}

PubSub::~PubSub() {}

bool PubSub::canSend() const {
  return getIsServer();
}

void PubSub::didSend() {}

bool PubSub::canRecv() const {
  return !getIsServer();
}

void PubSub::didRecv() {}

}  // namespace ZmqPb
