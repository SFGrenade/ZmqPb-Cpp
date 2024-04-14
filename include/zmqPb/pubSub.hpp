#ifndef PUBSUB_HPP_
#define PUBSUB_HPP_

#include "zmqPb/_export.hpp"
#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class ZMQPB_EXPORT PubSub : public ZmqWrap {
  public:
  enum class Status { Receiving, Sending };

  public:
  PubSub( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~PubSub();

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;

  private:
  bool isServer_;
  PubSub::Status status_;
};

}  // namespace ZmqPb

#endif /* PUBSUB_HPP_ */
