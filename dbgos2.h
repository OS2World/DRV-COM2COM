#ifndef __DBGOS2_H__
#define __DBGOS2_H__

#ifdef DEBUG_BREAKPOINT

#define DebugInt3()	_asm int 3;

#else

#define DebugInt3()     ;

#endif

#endif //__DBGOS2_H__
