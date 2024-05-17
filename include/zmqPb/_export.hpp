#include <hedley/hedley.h>

#if defined( ZMQPB_IS_SHARED ) && defined( ZMQPB_COMPILING )

#define ZMQPB_API HEDLEY_PUBLIC

#elif defined( ZMQPB_IS_SHARED )

#define ZMQPB_API HEDLEY_IMPORT

#else

#define ZMQPB_API

#endif
