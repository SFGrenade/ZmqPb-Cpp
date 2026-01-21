#ifndef RADIODISH_HPP_
#define RADIODISH_HPP_

#include "zmqPb/_export.hpp"
#include "zmqPb/zmqWrap.hpp"

#ifdef ZMQ_BUILD_DRAFT_API

#include <vector>

namespace ZmqPb {

class ZMQPB_API_CLASSES RadioDish : public ZmqWrap {
  public:
  RadioDish( std::string const& host, bool isServer, std::vector< std::string > const& joinGroups, zmq::context_t* contextToUse = nullptr );
  ~RadioDish() override;

  virtual void sendMessage( google::protobuf::Message* message, std::string const& group ) override;
  virtual void sendMessage( google::protobuf::Message* message ) override;

  protected:
  virtual bool canSend() const override;
  virtual void didSend() override;
  virtual bool canRecv() const override;
  virtual void didRecv() override;
};

}  // namespace ZmqPb

#endif /* RADIODISH_HPP_ */
#endif /* ZMQ_BUILD_DRAFT_API */
