#pragma code_seg ("_inittext");
#pragma data_seg ("_initdata","endds");

#define INCL_NOPMAPI
#include <os2.h>

#include "base/devhelp.h"

#include "base/strategy.h"
#include "init.h"
#include "resident.h"

#include "end.h"                       // end_of_data, end_of_text()

/**
 * Strategy Init
 */
void StrategyInitControl(PREQPACKET prp)
{
   Device_Help = (PFN) prp->s.init_in.ulDevHlp;
   ResidentInit();
   prp->s.init_out.usCodeEnd = (USHORT) &end_of_text;
   prp->s.init_out.usDataEnd = (USHORT) &end_of_heap;
   prp->usStatus = RPDONE;
}

/**
 * Strategy Init
 */
void StrategyInitCom(PREQPACKET prp)
{
   prp->s.init_out.usCodeEnd = (USHORT) &end_of_text;
   prp->s.init_out.usDataEnd = (USHORT) &end_of_heap;
   prp->usStatus = RPDONE;
}
