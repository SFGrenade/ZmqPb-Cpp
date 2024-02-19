#ifndef ZMQWRAP_HPP_
#define ZMQWRAP_HPP_

#if defined( _DO_EXPORT_CPP_CLASSES_ )
#if defined( _MSC_VER )
#define EXPORT __declspec( dllexport )
#elif defined( __GNUC__ )
#define EXPORT __attribute__( ( visibility( "default" ) ) )
#endif
#else
#define EXPORT
#endif

#include <functional>
#include <google/protobuf/message.h>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <zmq.hpp>

namespace ZmqPb {

struct EXPORT Subscription {
  Subscription();
  Subscription( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback );
  google::protobuf::Message* message;
  std::function< void( google::protobuf::Message const& ) > callback;
};

class EXPORT ZmqWrap {
  public:
  ZmqWrap( std::string const& host, zmq::socket_type socketType, zmq::context_t* contextToUse );
  ~ZmqWrap();

  void subscribe( google::protobuf::Message* message, std::function< void( google::protobuf::Message const& ) > callback );
  virtual void sendMessage( google::protobuf::Message* message );

  virtual void run();

  protected:
  virtual bool canSend() const = 0;
  virtual void didSend() = 0;
  virtual bool canRecv() const = 0;
  virtual void didRecv() = 0;

  protected:
  std::string host_;

  bool ownsContext_;
  zmq::context_t* zmqContext_;
  zmq::socket_t zmqSocket_;

  std::mutex mutexForSendQueue_;
  std::queue< zmq::message_t* > queueToSend_;
  std::map< std::string, Subscription > subscribedMessages_;
};

}  // namespace ZmqPb

#endif /* ZMQWRAP_HPP_ */
