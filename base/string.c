#include "base/string.h"

void __far *memcpy( void __far *dst, const void __far *src, size_t length )
{
  char __far *pDst=dst;
  const char __far *pSrc=src;
  while (length>0)
  {
    *pDst=*pSrc;
    ++pDst;
    ++pSrc;
    --length;
  }

  return dst;
}

size_t strlen( const char __far *s )
{
  size_t len=0;
  while (*s)
  {
    ++len;
    ++s;
  }

  return len;
}
