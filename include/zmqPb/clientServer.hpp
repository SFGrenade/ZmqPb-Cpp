#ifndef CLIENTSERVER_HPP_
#define CLIENTSERVER_HPP_

#include "zmqPb/_export.hpp"
#include "zmqPb/zmqWrap.hpp"

#ifdef ZMQ_BUILD_DRAFT_API

namespace ZmqPb {

class ZMQPB_API_CLASSES ClientServer : public ZmqWrap {
  public:
  ClientServer( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~ClientServer() override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;
};

#endif /* ZMQ_BUILD_DRAFT_API */
#endif /* CLIENTSERVER_HPP_ */
