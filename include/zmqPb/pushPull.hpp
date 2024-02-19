#ifndef PUSHPULL_HPP_
#define PUSHPULL_HPP_

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class PushPull : public ZmqWrap {
  public:
  PushPull( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~PushPull();

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;

  private:
  bool isServer_;
};

}  // namespace ZmqPb

#endif /* PUSHPULL_HPP_ */
