#ifdef ZMQ_BUILD_DRAFT_API
#include "zmqPb/clientServer.hpp"

namespace ZmqPb {

ClientServer::ClientServer( std::string const& host, bool isServer )
    : ZmqWrap( host, isServer ? zmq::socket_type::server : zmq::socket_type::client ), isServer_( isServer ) {
  if( isServer_ ) {
    zmqSocket_.bind( host_ );
  } else {
    zmqSocket_.connect( host_ );
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
