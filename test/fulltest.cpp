#include <zmqPb/reqRep.hpp>

int main( int argc, char** argv ) {
  ZmqPb::ReqRep network( "tcp://127.0.0.1:13337", true );
  network.run();
  return 0;
}
