#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void __far *memcpy( void __far *dst, const void __far *src, size_t length );
size_t strlen( const char __far *s );

#ifdef __cplusplus
};
#endif
