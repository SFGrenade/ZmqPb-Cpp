#ifndef REQREP_HPP_
#define REQREP_HPP_

#include "zmqPb/_export.hpp"
#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

EXPORT class ReqRep : public ZmqWrap {
  public:
  enum class Status { Receiving, Sending };

  public:
  ReqRep( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~ReqRep();

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;

  private:
  bool isServer_;
  ReqRep::Status status_;
};

}  // namespace ZmqPb

#endif /* REQREP_HPP_ */
