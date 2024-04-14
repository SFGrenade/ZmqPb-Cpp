#ifndef PAIR_HPP_
#define PAIR_HPP_

#include "zmqPb/_export.hpp"
#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class ZMQPB_EXPORT Pair : public ZmqWrap {
  public:
  Pair( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
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
