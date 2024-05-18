#include <hedley/hedley.h>

#if defined( ZMQPB_IS_SHARED ) && defined( ZMQPB_COMPILING )

#define ZMQPB_API_FUNCTIONS HEDLEY_PUBLIC
#define ZMQPB_API_CLASSES HEDLEY_PUBLIC

#elif defined( ZMQPB_IS_SHARED )

#define ZMQPB_API_FUNCTIONS HEDLEY_IMPORT

#if defined( _WIN32 ) || defined( __CYGWIN__ )
#define ZMQPB_API_CLASSES HEDLEY_IMPORT
#else
#define ZMQPB_API_CLASSES
#endif

#else

#define ZMQPB_API_FUNCTIONS
#define ZMQPB_API_CLASSES

#endif
