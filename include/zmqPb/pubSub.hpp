#ifndef PUBSUB_HPP_
#define PUBSUB_HPP_

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class PubSub : public ZmqWrap {
  public:
  enum class Status { Receiving, Sending };

  public:
  PubSub( std::string const& host, uint16_t port, bool isServer );
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
