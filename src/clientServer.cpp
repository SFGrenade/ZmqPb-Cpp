#ifdef ZMQ_BUILD_DRAFT_API
#include "zmqPb/clientServer.hpp"

#include <fmt/core.h>

namespace ZmqPb {

ClientServer::ClientServer( std::string const& host, uint16_t port, bool isServer )
    : ZmqWrap( host, port, isServer ? zmq::socket_type::server : zmq::socket_type::client ), isServer_( isServer ) {
  if( isServer_ ) {
    zmqSocket_.bind( fmt::format( "{}:{}", host_, port_ ) );
  } else {
    zmqSocket_.connect( fmt::format( "{}:{}", host_, port_ ) );
  }
}

ClientServer::~ClientServer() {}

bool ClientServer::canSend() const {
  return true;
}

void ClientServer::didSend() {}

bool ClientServer::canRecv() const {
  return true;
}

void ClientServer::didRecv() {}

}  // namespace ZmqPb
#endif /* ZMQ_BUILD_DRAFT_API */
