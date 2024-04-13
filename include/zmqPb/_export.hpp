#ifdef ZMQPB_EXPORT

#define EXPORT __declspec( dllexport )

#else

#define EXPORT

#endif /* ZMQPB_EXPORT */
