# ZmqPb

This is a library to use 0mq and protobuf together more easily.

## Usage

```proto
syntax = "proto3";

message MyMessage {
  string message = 1;
}
```

```cpp
#include <reqRep.hpp>

int main() {
  // create a req-rep client on address 127.0.0.1 at port 13337
  ZmqPb::ReqRep network( "tcp://127.0.0.1:13337", false );

  // flag to indicate when to stop the loop
  bool running = true;

  // subscribe to incoming proto-messages of type `MyMessage`
  network.subscribe( new MyMessage(), [&running]( google::protobuf::Message const& message ) {
    std::cout << "My message" << std::endl;
    // stop the network once we received any answer
    running = false;
  } );

  // send a network message, messages are managed by the client
  MyMessage* msg = new MyMessage();
  msg->set_message( "Hello World!" );
  rrNetwork_.sendMessage( msg );

  // run the client
  while( running ) {
    network.run();
  }

  // cleanup happens on destruction
  return 0;
}
```
