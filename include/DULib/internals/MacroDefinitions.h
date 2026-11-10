
#ifdef _MSC_VER
#define DU_LIB_FORCEINLINE __forceinline
#elif defined(__GNUC__)
#define DU_LIB_FORCEINLINE __inline__ __attribute__((__always_inline__))
#else
#define DU_LIB_FORCEINLINE inline
#endif