#if defined( ZMQPB_DO_EXPORT_LINUX )

#define ZMQPB_EXPORT __attribute__( ( dllexport ) )

#elif defined( ZMQPB_DO_EXPORT_MACOSX )

#define ZMQPB_EXPORT __attribute__( ( visibility( "default" ) ) )

#elif defined( ZMQPB_DO_EXPORT_WINDOWS )

#define ZMQPB_EXPORT __declspec( dllexport )

#else

#define ZMQPB_EXPORT

#endif
