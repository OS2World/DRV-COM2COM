#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

class CircularBuffer
{
public:
  CircularBuffer() {};
  void init();
  // returns number of bytes has been read
  unsigned read(ULONG physOutput,unsigned length,ULONG timeout,bool __far &isError);
  unsigned write(ULONG physInput,unsigned length);
  unsigned writeVirt(BYTE __far *pInput,unsigned length);
  void setInitDone();
private:
  unsigned m_size;
  unsigned m_begin;
  unsigned m_length;
  BYTE m_data[8192];
};

#endif
