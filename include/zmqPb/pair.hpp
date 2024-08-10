#ifndef PAIR_HPP_
#define PAIR_HPP_

#include "zmqPb/_export.hpp"
#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class ZMQPB_API_CLASSES Pair : public ZmqWrap {
  public:
  Pair( std::string const& host, bool isServer, zmq::context_t* contextToUse = nullptr );
  ~Pair() override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;
};

}  // namespace ZmqPb

#endif /* PAIR_HPP_ */
