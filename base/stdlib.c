#include "base/stdlib.h"

#include "dbgos2.h"

static const char* digitalAlphabet="0123456789abcdefghijklmnopqrstuvwxyz";

char __far * _itoa(int value,char __far *buffer,int radix)
{
  char __far *p=buffer;
  if (radix==10 && value<0)
  {
    value=-value;
    *p='-';
    ++p;
  }

  _utoa(value,p,radix);
  return buffer;
}

char __far * _utoa(unsigned int value,char __far *buffer,int radix)
{
  char __far *p=buffer;
  char backBuf[32];
  char __far *pBackBuf=backBuf;
  unsigned quot=value;
  unsigned rem;

  while (quot>0)
  {
    rem=quot%radix;
    quot/=radix;
    *pBackBuf=digitalAlphabet[rem];
    ++pBackBuf;
  }

  if (pBackBuf!=backBuf)
  {
    while (pBackBuf!=backBuf)
    {
      --pBackBuf;
      *p=*pBackBuf;
      ++p;
    }
  }
  else
  {
    *p='0';
    ++p;
  }

  *p=0;
  return buffer;
}

char __far * _ltoa(long value,char __far *buffer,int radix)
{
  char __far *p=buffer;
  if (radix==10 && value<0)
  {
    value=-value;
    *p='-';
    ++p;
  }

  _ultoa(value,p,radix);
  return buffer;
}

char __far * _ultoa(unsigned long value,char __far *buffer,int radix)
{
  char __far *p=buffer;
  char backBuf[32];
  char __far *pBackBuf=backBuf;
  unsigned long quot=value;
  unsigned rem;

  while (quot>0)
  {
    rem=quot%radix;
    quot/=radix;
    *pBackBuf=digitalAlphabet[rem];
    ++pBackBuf;
  }

  if (pBackBuf!=backBuf)
  {
    while (pBackBuf!=backBuf)
    {
      --pBackBuf;
      *p=*pBackBuf;
      ++p;
    }
  }
  else
  {
    *p='0';
    ++p;
  }

  *p=0;
  return buffer;
}
