#ifndef COMDEVICE_H
#define COMDEVICE_H

#include "base/strategy.h"
#include "CircularBuffer.h"

class ComDevice
{
public:
  void init();
  void setPeer(ComDevice *peer);
  void strategyOpen(PREQPACKET prp);
  void strategyClose(PREQPACKET prp);
  void strategyRead(PREQPACKET prp);
  void strategyWrite(PREQPACKET prp);
  void strategyIOCtl(PREQPACKET prp);
private:
  void initDCBInfo(bool isReinit);
  void ioctlSetBaudRate(PREQPACKET prp);
  void ioctlExtSetBaudRate(PREQPACKET prp);
  void ioctlSetDCBInfo(PREQPACKET prp);
  void ioctlGetBaudRate(PREQPACKET prp);
  void ioctlExtGetBaudRate(PREQPACKET prp);
  void ioctlGetDCBInfo(PREQPACKET prp);
  int m_openCount;
  CircularBuffer m_buf;
  ULONG m_bitRate;
  BYTE m_bitRateFraction;
  DCBINFO m_dcbinfo;
  ComDevice *m_peer;
};

#endif
