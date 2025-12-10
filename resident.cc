#define INCL_DOSDEVIOCTL
#define INCL_NOPMAPI
#include <os2.h>

#include "base/stdarg.h"
#include "base/stdio.h"
#include "base/string.h"
#include "base/strategy.h"
#include "CircularBuffer.h"
#include "ComDevice.h"

#include "resident.h"

#include "dbgos2.h"

static ComDevice devComFirst,devComSecond;
static CircularBuffer bufLog;

void logPrintRaw(char* pStr,unsigned length)
{
#ifdef DEBUG_RAS
  bufLog.writeVirt((BYTE*)pStr,length);
#endif
}

void logPrintString(char* pStr)
{
#ifdef DEBUG_RAS
  bufLog.writeVirt((BYTE*)pStr,strlen(pStr));
#endif
}

void logPrintf(char* pFormat, ...)
{
#ifdef DEBUG_RAS
  va_list arg_ptr;
  char buf[1024];
  buf[0]=0;

  va_start(arg_ptr, pFormat);
  dev_vsprintf(buf, pFormat, arg_ptr);
  va_end(arg_ptr);

  bufLog.writeVirt((BYTE*)buf,strlen(buf));
#endif
}

void ResidentInit()
{
  bufLog.init();
  devComFirst.init();
  devComSecond.init();
  devComFirst.setPeer(&devComSecond);
  devComSecond.setPeer(&devComFirst);
}

void devLog_read(PREQPACKET prp)
{
  if (prp->s.io.usCount==0)
  {
    prp->usStatus=RPDONE;
    return;
  }

  bool isError;
  prp->s.io.usCount=bufLog.read(prp->s.io.ulAddress,prp->s.io.usCount,-1,isError);
  prp->usStatus=(isError)?RPDONE|RPERR|RPCHARIOINT:RPDONE;
}

void devFirst_strategy(PREQPACKET prp)
{
  switch (prp->bCommand)
  {
  case STRATEGY_INITCOMPLETE:
    prp->usStatus = RPDONE;
    break;
  case STRATEGY_OPEN:
    devComFirst.strategyOpen(prp);
    break;
  case STRATEGY_CLOSE:
    devComFirst.strategyClose(prp);
    break;
  case STRATEGY_READ:
//    DebugInt3();
    devComFirst.strategyRead(prp);
//    DebugInt3();
    break;
  case STRATEGY_WRITE:
    devComFirst.strategyWrite(prp);
    break;
  case STRATEGY_GENIOCTL:
    logPrintf("Strategy First IOCTL enter. Category=0x%02X Code=0x%02X\n",prp->s.ioctl.bCategory,prp->s.ioctl.bCode);
    devComFirst.strategyIOCtl(prp);
    logPrintf("Strategy First IOCTL exit.\n");
    break;
  default:
    prp->usStatus = RPDONE | RPERR | RPBADCMD;
  }
}

void devSecond_strategy(PREQPACKET prp)
{
  switch (prp->bCommand)
  {
  case STRATEGY_INITCOMPLETE:
    prp->usStatus = RPDONE;
    break;
  case STRATEGY_OPEN:
    devComSecond.strategyOpen(prp);
    break;
  case STRATEGY_CLOSE:
    devComSecond.strategyClose(prp);
    break;
  case STRATEGY_READ:
    devComSecond.strategyRead(prp);
    break;
  case STRATEGY_WRITE:
    devComSecond.strategyWrite(prp);
    break;
  case STRATEGY_GENIOCTL:
    logPrintf("Strategy Second IOCTL enter. Category=0x%02X Code=0x%02X\n",prp->s.ioctl.bCategory,prp->s.ioctl.bCode);
    devComSecond.strategyIOCtl(prp);
    logPrintf("Strategy Second IOCTL exit.\n");
    break;
  default:
    prp->usStatus = RPDONE | RPERR | RPBADCMD;
  }
}
