#include "zmqPb/reqRep.hpp"

namespace ZmqPb {

ReqRep::ReqRep( std::string const& host, bool isServer, zmq::context_t* contextToUse )
    : ZmqWrap( host, isServer ? zmq::socket_type::rep : zmq::socket_type::req, contextToUse ),
      isServer_( isServer ),
      status_( isServer ? ReqRep::Status::Receiving : ReqRep::Status::Sending ) {
  if( isServer_ ) {
    getSocketPtr()->bind( host );
  } else {
    getSocketPtr()->connect( host );
  }
}

ReqRep::~ReqRep() {}

bool ReqRep::canSend() const {
  return status_ == ReqRep::Status::Sending;
}

void ReqRep::didSend() {
  status_ = ReqRep::Status::Receiving;
}

bool ReqRep::canRecv() const {
  return status_ == ReqRep::Status::Receiving;
}

void ReqRep::didRecv() {
  status_ = ReqRep::Status::Sending;
}

}  // namespace ZmqPb
