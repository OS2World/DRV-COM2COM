#include "base/strategy.h"

#ifdef __cplusplus
extern "C" {
#endif

void __far StrategyHandlerControl(PREQPACKET prp);
#pragma aux StrategyHandlerControl parm [es bx];

void __far StrategyHandlerComFirst(PREQPACKET prp);
#pragma aux StrategyHandlerComFirst parm [es bx];

void __far StrategyHandlerComSecond(PREQPACKET prp);
#pragma aux StrategyHandlerComSecond parm [es bx];

#ifdef __cplusplus
};
#endif
