#ifndef SUBSCRIPTION_HPP_
#define SUBSCRIPTION_HPP_

#include <functional>
#include <google/protobuf/message.h>

#include "zmqPb/_export.hpp"

namespace ZmqPb {

class ZMQPB_EXPORT Subscription {
  public:
  Subscription();
  Subscription( Subscription const& other );
  Subscription( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback );
  ~Subscription();

  google::protobuf::Message* getMessage() const;
  std::function< void( google::protobuf::Message const& ) > getCallback() const;

  private:
  class impl;
  impl* pimpl;
};

}  // namespace ZmqPb

#endif /* SUBSCRIPTION_HPP_ */
