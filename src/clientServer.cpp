#include "zmqPb/clientServer.hpp"

#ifdef ZMQ_BUILD_DRAFT_API

namespace ZmqPb {

ClientServer::ClientServer( std::string const& host, bool isServer, zmq::context_t* contextToUse )
    : ZmqWrap( host, isServer, isServer ? zmq::socket_type::server : zmq::socket_type::client, contextToUse ) {
  connectSocket();
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
