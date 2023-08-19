#ifdef ZMQ_BUILD_DRAFT_API
#ifndef RADIODISH_HPP_
#define RADIODISH_HPP_

#include "zmqPb/zmqWrap.hpp"

namespace ZmqPb {

class RadioDish : public ZmqWrap {
  public:
  RadioDish( std::string const& host, uint16_t port, bool isServer );
  ~RadioDish();

  virtual void sendMessage( google::protobuf::Message* message ) override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;

  private:
  bool isServer_;
};

}  // namespace ZmqPb

#endif /* RADIODISH_HPP_ */
#endif /* ZMQ_BUILD_DRAFT_API */
