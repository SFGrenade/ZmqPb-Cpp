#ifdef ZMQ_BUILD_DRAFT_API
#ifndef CLIENTSERVER_HPP_
#define CLIENTSERVER_HPP_

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class ClientServer : public ZmqWrap {
  public:
  ClientServer( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~ClientServer();

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;

  private:
  bool isServer_;
};

}  // namespace ZmqPb

#endif /* CLIENTSERVER_HPP_ */
#endif /* ZMQ_BUILD_DRAFT_API */
