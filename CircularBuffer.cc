#define INCL_NOPMAPI
#include <os2.h>

#include "base/devhelp.h"

#include "CircularBuffer.h"

#include "dbgos2.h"

inline unsigned min(unsigned a,unsigned b)
{
  return (a<b)?a:b;
}

void CircularBuffer::init()
{
  m_size=8192;
  m_begin=0;
  m_length=0;
}

unsigned CircularBuffer::read(ULONG addrOutput,unsigned length,ULONG timeout,bool __far &isError)
{
  isError=false;
  if (length==0) {return 0;}

  unsigned reqLength=length;
  unsigned completedLength=0;
  while (1)
  {
    BYTE __far *pOutput;
    DevHelp_PhysToVirt(addrOutput,reqLength,&pOutput,0);
    pOutput+=completedLength;
    while (length>0 && m_length>0)
    {
      unsigned lenRead=min(min(m_length,length),m_size-m_begin);
      BYTE* pData=m_data+m_begin;
      length-=lenRead;
      m_length-=lenRead;
      m_begin+=lenRead;
      completedLength+=lenRead;
      if (m_begin==m_size) {m_begin=0;}
      while (lenRead-->0)
      {
        *(pOutput++)=*(pData++);
      }
    }

    if (length==0 || timeout==0) {return completedLength;}

    //DebugInt3();
    USHORT waitStatus=DevHelp_ProcBlock((ULONG)(void __far*)this,timeout,WAIT_IS_INTERRUPTABLE);
    if (!waitStatus || waitStatus==WAIT_TIMED_OUT)
    {
      if (m_length>0) {continue;} // data is in buffer so continue read
      return completedLength;
    }

    isError=true; // thread aborted
    return completedLength;
  }
}

unsigned CircularBuffer::write(ULONG addrInput,unsigned length)
{
  unsigned reminder=m_size-m_length;
  if (length>reminder) {length=reminder;}
  if (length==0) {return 0;}

  BYTE __far *pInput;
  DevHelp_PhysToVirt(addrInput,length,&pInput,0);

  return writeVirt(pInput,length);
}

unsigned CircularBuffer::writeVirt(BYTE __far *pInput,unsigned length)
{
//  DebugInt3();
  unsigned reminder=m_size-m_length;
  if (length>reminder) {length=reminder;}
  if (length==0) {return 0;}

  unsigned result=length;
  unsigned lenLeft=m_size-m_begin;
  if (lenLeft>m_length)
  {
    // we have some space without need to round
    unsigned lenWrite=min(lenLeft-m_length,length);
    BYTE* pSpace=m_data+(m_begin+m_length);
    length-=lenWrite;
    m_length+=lenWrite;
    while (lenWrite-->0)
    {
      *(pSpace++)=*(pInput++);
    }
  }

  if (length>0)
  {
    // now we have no space to the right of the begin pointer
    BYTE* pSpace=m_data+(m_begin+m_length-m_size);
    m_length+=length;
    while (length-->0)
    {
      *(pSpace++)=*(pInput++);
    }
  }

//  DebugInt3();
  USHORT awakeCount;
  DevHelp_ProcRun((ULONG)(void __far*)this,&awakeCount); // unblock read on the same buffer
  return result;
}
