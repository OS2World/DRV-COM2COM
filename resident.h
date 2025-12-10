#include "base/strategy.h"

#ifdef __cplusplus
extern "C" {
#endif

void ResidentInit();

void logPrintRaw(char* pStr,unsigned length);
void logPrintString(char* pStr);
void logPrintf(char* pFormat, ...);

void devLog_read(PREQPACKET prp);
void devFirst_strategy(PREQPACKET prp);
void devSecond_strategy(PREQPACKET prp);

#ifdef __cplusplus
};
#endif
