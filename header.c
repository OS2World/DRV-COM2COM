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
 * The Device Driver Header
 * @version %I%
 * @context Unless otherwise noted, all interfaces are Ring-0, 16-bit,
 *  <stack context>.
 * @history
 *
 */

#pragma code_seg ("_inittext");
#pragma data_seg ("_header","data");

#define INCL_NOPMAPI
#include <os2.h>

#include "strategy_d.h"

#include "base/header.h"

void __far StrategyHandler(void);

DEV_HEADER header[3] = {
  {sizeof(DEV_HEADER),
   DA_CHAR | DA_NEEDOPEN | DA_USESCAP,
   (PFNENTRY)StrategyHandlerControl,
   (PFNENTRY)0,
   {'C','O','M','2','C','O','M','$'},
   0,0,
   DC_INITCPLT | DC_IOCTL2 | DC_32BIT
  },
  {2*sizeof(DEV_HEADER),
   DA_CHAR | DA_NEEDOPEN | DA_USESCAP,
   (PFNENTRY)StrategyHandlerComFirst,
   (PFNENTRY)0,
   {'C','O','M','3',' ',' ',' ',' '},
   0,0,
   DC_INITCPLT | DC_IOCTL2 | DC_32BIT
  },
  {-1,
   DA_CHAR | DA_NEEDOPEN | DA_USESCAP,
   (PFNENTRY)StrategyHandlerComSecond,
   (PFNENTRY)0,
   {'C','O','M','4',' ',' ',' ',' '},
   0,0,
   DC_INITCPLT | DC_IOCTL2 | DC_32BIT
  }
};
