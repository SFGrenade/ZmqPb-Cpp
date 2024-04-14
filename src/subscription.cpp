#include "zmqPb/subscription.hpp"

namespace ZmqPb {

class Subscription::impl {
  public:
  google::protobuf::Message* message;
  std::function< void( google::protobuf::Message const& ) > callback;
};

Subscription::Subscription() : pimpl( new impl ) {
  pimpl->message = nullptr;
  pimpl->callback = nullptr;
}

Subscription::Subscription( Subscription const& other ) : pimpl( new impl ) {
  pimpl->message = other.pimpl->message;
  pimpl->callback = other.pimpl->callback;
}

Subscription::Subscription( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback ) : pimpl( new impl ) {
  pimpl->message = message;
  pimpl->callback = callback;
}

Subscription::~Subscription() {
  delete pimpl->message;
  delete pimpl;
}

google::protobuf::Message* Subscription::getMessage() const {
  return pimpl->message;
}

std::function< void( google::protobuf::Message const& ) > Subscription::getCallback() const {
  return pimpl->callback;
}

}  // namespace ZmqPb
