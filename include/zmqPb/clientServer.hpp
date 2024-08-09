#ifdef ZMQ_BUILD_DRAFT_API
#ifndef CLIENTSERVER_HPP_
#define CLIENTSERVER_HPP_

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class ClientServer : public ZmqWrap {
  public:
  ClientServer( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~ClientServer() override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;
};

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

#endif /* CLIENTSERVER_HPP_ */
#endif /* ZMQ_BUILD_DRAFT_API */
