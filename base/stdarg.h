#ifndef _VA_LIST_DEFINED
typedef char __far *va_list;
#define _VA_LIST_DEFINED
#endif

#define va_start(ap,v) ap = (va_list)&v + sizeof(v)
#define va_arg(ap,t) ((t __far *)(ap += sizeof(t)))[-1]
#define va_end(ap) ap = 0
