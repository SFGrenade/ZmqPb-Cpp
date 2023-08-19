#ifndef PAIR_HPP_
#define PAIR_HPP_

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class Pair : public ZmqWrap {
  public:
  Pair( std::string const& host, uint16_t port, bool isServer );
  ~Pair();

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;

  private:
  bool isServer_;
};

}  // namespace ZmqPb

#endif /* PAIR_HPP_ */
