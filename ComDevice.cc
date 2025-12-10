#define INCL_DOSDEVIOCTL
#define INCL_NOPMAPI
#include <os2.h>

#include "base/devhelp.h"
#include "base/string.h"
#include "resident.h"

#include "ComDevice.h"

static printDCBInfo(DCBINFO __far &dcbinfo)
{
  logPrintf("Write timeout=%d\n",dcbinfo.usWriteTimeout);
  logPrintf("Read timeout=%d\n",dcbinfo.usReadTimeout);
  logPrintf("CtlHndShake=0x%02X FlowReplace=0x%02X fbTimeout=0x%02X\n",dcbinfo.fbCtlHndShake,dcbinfo.fbFlowReplace,dcbinfo.fbTimeout);
  logPrintf("ErrorChar=0x%02X BreakChar=0x%02X XONChar=0x%02X XOFFChar=0x%02X\n",dcbinfo.bErrorReplacementChar,dcbinfo.bBreakReplacementChar,dcbinfo.bXONChar,dcbinfo.bXOFFChar);
}

void ComDevice::init()
{
  m_openCount=0;
  m_buf.init();
  m_bitRate=1200;
  m_bitRateFraction=0;
  initDCBInfo(false);
  m_peer=NULL;
}

void ComDevice::setPeer(ComDevice *peer)
{
  m_peer=peer;
}

void ComDevice::strategyOpen(PREQPACKET prp)
{
  if (!m_openCount)
  {
    initDCBInfo(true);
  }
  ++m_openCount;

  prp->usStatus=RPDONE;
}

void ComDevice::strategyClose(PREQPACKET prp)
{
  if (m_openCount>0)
  {
    --m_openCount;
  }

  prp->usStatus=RPDONE;
}

void ComDevice::strategyRead(PREQPACKET prp)
{
  if (prp->s.io.usCount==0)
  {
    prp->usStatus=RPDONE;
    return;
  }

  logPrintf("strategyRead(0x%04X):\n",this);
  printDCBInfo(m_dcbinfo);
  ULONG timeout=(m_dcbinfo.fbTimeout&6==MODE_NOWAIT_READ_TIMEOUT)?0:((ULONG)m_dcbinfo.usReadTimeout+1)*10;
  bool isError;
  logPrintf("strategyRead calling buf.read. Length=%u Timeout=%lu:\n",prp->s.io.usCount,timeout);
  prp->s.io.usCount=m_buf.read(prp->s.io.ulAddress,prp->s.io.usCount,timeout,isError);
  logPrintf("strategyRead returning buf.read. Length=%u IsError=%u\n",prp->s.io.usCount,isError);
  prp->usStatus=(isError)?RPDONE|RPERR|RPCHARIOINT:RPDONE;
}

void ComDevice::strategyWrite(PREQPACKET prp)
{
  if (prp->s.io.usCount==0)
  {
    prp->usStatus=RPDONE;
    return;
  }

  prp->s.io.usCount=m_peer->m_buf.write(prp->s.io.ulAddress,prp->s.io.usCount);
  prp->usStatus=RPDONE;
}

void ComDevice::strategyIOCtl(PREQPACKET prp)
{
  prp->usStatus = RPDONE;
  if (prp->s.ioctl.bCategory!=IOCTL_ASYNC)
  {
    logPrintf("Non ASYNC IOCTL\n");
    prp->usStatus|=RPERR|RPGENFAIL;
    return;
  }

  switch (prp->s.ioctl.bCode)
  {
  case ASYNC_SETBAUDRATE:
    ioctlSetBaudRate(prp);
    break;
  case ASYNC_SETLINECTRL:
    logPrintf("ASYNC_SETLINECTRL. Ignored\n");
    break;
  case ASYNC_EXTSETBAUDRATE:
    ioctlExtSetBaudRate(prp);
    break;
  case ASYNC_STOPTRANSMIT:
    logPrintf("ASYNC_STOPTRANSMIT. Ignored\n");
    break;
  case ASYNC_STARTTRANSMIT:
    logPrintf("ASYNC_STARTTRANSMIT. Ignored\n");
    break;
  case ASYNC_SETDCBINFO:
    ioctlSetDCBInfo(prp);
    break;
  case ASYNC_GETBAUDRATE:
    ioctlGetBaudRate(prp);
    break;
  case ASYNC_EXTGETBAUDRATE:
    ioctlExtGetBaudRate(prp);
    break;
  case ASYNC_GETDCBINFO:
    ioctlGetDCBInfo(prp);
    break;
  case ASYNC_TRANSMITIMM:
  case ASYNC_SETBREAKOFF:
  case ASYNC_SETMODEMCTRL:
  case ASYNC_SETBREAKON:
  case ASYNC_SETENHANCEDMODEPARMS:
  case ASYNC_GETLINECTRL:
  case ASYNC_GETCOMMSTATUS:
  case ASYNC_GETLINESTATUS:
  case ASYNC_GETMODEMOUTPUT:
  case ASYNC_GETMODEMINPUT:
  case ASYNC_GETINQUECOUNT:
  case ASYNC_GETOUTQUECOUNT:
  case ASYNC_GETCOMMERROR:
  case ASYNC_GETCOMMEVENT:
  case ASYNC_GETENHANCEDMODEPARMS:
  default:
    logPrintf("Non handled ASYNC IOCTL called. Returning error\n");
    prp->usStatus|=RPERR|RPGENFAIL;
  }
}

void ComDevice::initDCBInfo(bool isReinit)
{
  m_dcbinfo.usWriteTimeout=6000; // 1 minute
  m_dcbinfo.usReadTimeout=6000;  // 1 minute
  if (!isReinit)
  {
    m_dcbinfo.fbCtlHndShake=MODE_DTR_CONTROL|MODE_CTS_HANDSHAKE|MODE_DSR_HANDSHAKE|MODE_DSR_SENSITIVITY;
  }
  if (!isReinit)
  {
    m_dcbinfo.fbFlowReplace=MODE_RTS_CONTROL;
  }
  else
  {
    m_dcbinfo.fbFlowReplace&=~(MODE_ERROR_CHAR|MODE_NULL_STRIPPING|MODE_BREAK_CHAR);
  }
  if (!isReinit)
  {
    m_dcbinfo.fbTimeout=MODE_READ_TIMEOUT;
  }
  else
  {
    m_dcbinfo.fbTimeout=m_dcbinfo.fbTimeout&MODE_NO_WRITE_TIMEOUT|MODE_READ_TIMEOUT;
  }
  m_dcbinfo.bErrorReplacementChar=0;
  m_dcbinfo.bBreakReplacementChar=0;
  m_dcbinfo.bXONChar=0x11;
  m_dcbinfo.bXOFFChar=0x13;
}

#define ASSERT_REQPACKET_PARAMETER(prp,minSize,access) \
  if ((prp)->s.ioctl.usPLength<(minSize)) \
  { \
    (prp)->usStatus|=RPERR|RPGENFAIL; \
    return; \
  } \
  if(DevHelp_VerifyAccess(SELECTOROF((prp)->s.ioctl.pvParm), (prp)->s.ioctl.usPLength, OFFSETOF((prp)->s.ioctl.pvParm), (access))) \
  { \
    (prp)->usStatus|=RPERR|RPGENFAIL; \
    return; \
  }

#define ASSERT_REQPACKET_DATA(prp,minSize,access) \
  if ((prp)->s.ioctl.usDLength<(minSize)) \
  { \
    (prp)->usStatus|=RPERR|RPGENFAIL; \
    return; \
  } \
  if(DevHelp_VerifyAccess(SELECTOROF((prp)->s.ioctl.pvData), (prp)->s.ioctl.usDLength, OFFSETOF((prp)->s.ioctl.pvData), (access))) \
  { \
    (prp)->usStatus|=RPERR|RPGENFAIL; \
    return; \
  }

void ComDevice::ioctlSetBaudRate(PREQPACKET prp)
{
  ASSERT_REQPACKET_PARAMETER(prp,2,VERIFY_READONLY)
  USHORT __far *pBitRate=(USHORT __far *)prp->s.ioctl.pvParm;
  logPrintf("ioctlSetBaudRate BitRate=%u\n",*pBitRate);
  m_bitRate=*pBitRate;
  m_bitRateFraction=0;
}

void ComDevice::ioctlExtSetBaudRate(PREQPACKET prp)
{
#pragma pack(1);
  typedef struct
  {
    ULONG bitRate;
    BYTE fraction;
  } ExtSetBaudRate;
#pragma pack();
  ASSERT_REQPACKET_PARAMETER(prp,sizeof(ExtSetBaudRate),VERIFY_READONLY);

  ExtSetBaudRate __far *pExtSetBaudRate=(ExtSetBaudRate __far *)prp->s.ioctl.pvParm;
  logPrintf("ioctlExtSetBaudRate BitRate=%u Fraction=%u\n",pExtSetBaudRate->bitRate,pExtSetBaudRate->fraction);
  m_bitRate=pExtSetBaudRate->bitRate;
  m_bitRateFraction=pExtSetBaudRate->fraction;
}

void ComDevice::ioctlSetDCBInfo(PREQPACKET prp)
{
  ASSERT_REQPACKET_PARAMETER(prp,sizeof(DCBINFO),VERIFY_READONLY);

  DCBINFO dcbinfo;
  memcpy(&dcbinfo,prp->s.ioctl.pvParm,sizeof(DCBINFO));
  logPrintf("ioctlSetDCBInfo request:\n");
  printDCBInfo(dcbinfo);
  dcbinfo.fbTimeout&=7; // hardware buffering is absent
  memcpy(&m_dcbinfo,&dcbinfo,sizeof(DCBINFO));
  logPrintf("ioctlSetDCBInfo(0x%04X) result:\n",this);
  printDCBInfo(m_dcbinfo);
}

void ComDevice::ioctlGetBaudRate(PREQPACKET prp)
{
  ASSERT_REQPACKET_DATA(prp,2,VERIFY_READWRITE);

  USHORT __far *pBitRate=(USHORT __far *)prp->s.ioctl.pvData;
  *pBitRate=(m_bitRate>0xFFFF)?0xFFFF:m_bitRate;
  logPrintf("ioctlGetBaudRate BitRate=%u\n",*pBitRate);
}

void ComDevice::ioctlExtGetBaudRate(PREQPACKET prp)
{
#pragma pack(1);
  typedef struct
  {
    ULONG bitRate;
    BYTE fraction;
    ULONG minBitRate;
    BYTE minFraction;
    ULONG maxBitRate;
    BYTE maxFraction;
  } ExtGetBaudRate;
#pragma pack();
  ASSERT_REQPACKET_DATA(prp,sizeof(ExtGetBaudRate),VERIFY_READWRITE);

  ExtGetBaudRate __far *pExtGetBaudRate=(ExtGetBaudRate __far *)prp->s.ioctl.pvData;
  pExtGetBaudRate->bitRate=m_bitRate;
  pExtGetBaudRate->fraction=m_bitRateFraction;
  pExtGetBaudRate->minBitRate=110;
  pExtGetBaudRate->minFraction=0;
  pExtGetBaudRate->maxBitRate=115200;
  pExtGetBaudRate->maxFraction=0;
  logPrintf("ioctlExtGetBaudRate BitRate=%u Fraction=%u\n",pExtGetBaudRate->bitRate,pExtGetBaudRate->fraction);
}

void ComDevice::ioctlGetDCBInfo(PREQPACKET prp)
{
  ASSERT_REQPACKET_DATA(prp,sizeof(DCBINFO),VERIFY_READWRITE);

  logPrintf("ioctlGetDCBInfo(0x%04X) result:\n",this);
  printDCBInfo(m_dcbinfo);
  memcpy(prp->s.ioctl.pvData,&m_dcbinfo,sizeof(DCBINFO));
}
