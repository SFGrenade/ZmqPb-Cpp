#ifndef REQREP_HPP_
#define REQREP_HPP_

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class ReqRep : public ZmqWrap {
  public:
  enum class Status { Receiving, Sending };

  public:
  ReqRep( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~ReqRep() override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;

  private:
  ReqRep::Status status_;
};

ReqRep::ReqRep( std::string const& host, bool isServer, zmq::context_t* contextToUse )
    : ZmqWrap( host, isServer, isServer ? zmq::socket_type::rep : zmq::socket_type::req, contextToUse ),
      status_( isServer ? ReqRep::Status::Receiving : ReqRep::Status::Sending ) {
  connectSocket();
}

ReqRep::~ReqRep() {}

bool ReqRep::canSend() const {
  return status_ == ReqRep::Status::Sending;
}

void ReqRep::didSend() {
  status_ = ReqRep::Status::Receiving;
}

bool ReqRep::canRecv() const {
  return status_ == ReqRep::Status::Receiving;
}

void ReqRep::didRecv() {
  status_ = ReqRep::Status::Sending;
}

}  // namespace ZmqPb

#endif /* REQREP_HPP_ */
