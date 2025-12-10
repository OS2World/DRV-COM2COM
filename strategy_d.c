/****************************************************************************
 *                                                                          *
 * Copyright (c) IBM Corporation 1994 - 1997.                               *
 *                                                                          *
 * The following IBM OS/2 source code is provided to you solely for the     *
 * the purpose of assisting you in your development of OS/2 device drivers. *
 * You may use this code in accordance with the IBM License Agreement       *
 * provided in the IBM Device Driver Source Kit for OS/2.                   *
 *                                                                          *
 ****************************************************************************/
/**@internal %W%
 * @notes
 * The Strategy entry point lives here !!
 * @version %I%
 * @context Unless otherwise noted, all interfaces are Ring-0, 16-bit,
 *  kernel stack.
 * @history
 *
 */

#define INCL_NOPMAPI
#include <os2.h>

#include "base/strategy.h"
#include "strategy_d.h"
#include "resident.h"
#include "end.h"

#include "dbgos2.h"

void __far StrategyHandlerControl(PREQPACKET prp)
{
  switch (prp->bCommand)
  {
  case STRATEGY_INIT:
    StrategyInitControl(prp);
    break;
  case STRATEGY_INITCOMPLETE:
  case STRATEGY_OPEN:
  case STRATEGY_CLOSE:
  case STRATEGY_WRITE:
  case STRATEGY_GENIOCTL:
    prp->usStatus = RPDONE;
    break;
  case STRATEGY_READ:
    devLog_read(prp);
    break;
  default:
    prp->usStatus = RPDONE | RPERR | RPGENFAIL;
  }
}

void __far StrategyHandlerComFirst(PREQPACKET prp)
{
  if (prp->bCommand!=STRATEGY_INIT && prp->bCommand!=STRATEGY_INITCOMPLETE)
    logPrintf("Strategy First enter. Command=%0X\n",prp->bCommand);

  switch (prp->bCommand)
  {
  case STRATEGY_INIT:
    StrategyInitCom(prp);
    break;
  default:
    devFirst_strategy(prp);
  }

  if (prp->bCommand!=STRATEGY_INIT && prp->bCommand!=STRATEGY_INITCOMPLETE)
    logPrintf("Strategy First exit. Command=%0X Status=%0X\n",prp->bCommand,prp->usStatus);
}

void __far StrategyHandlerComSecond(PREQPACKET prp)
{
  if (prp->bCommand!=STRATEGY_INIT && prp->bCommand!=STRATEGY_INITCOMPLETE)
    logPrintf("Strategy Second enter. Command=%0X\n",prp->bCommand);

  switch (prp->bCommand)
  {
  case STRATEGY_INIT:
    StrategyInitCom(prp);
    break;
  default:
    devSecond_strategy(prp);
  }

  if (prp->bCommand!=STRATEGY_INIT && prp->bCommand!=STRATEGY_INITCOMPLETE)
    logPrintf("Strategy Second exit. Command=%0X Status=%0X\n",prp->bCommand,prp->usStatus);
}
