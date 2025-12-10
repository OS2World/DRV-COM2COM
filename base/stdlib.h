#ifdef __cplusplus
extern "C" {
#endif

char __far * _itoa(int value,char __far *buffer,int radix);
char __far * _utoa(unsigned int value,char __far *buffer,int radix);
char __far * _ltoa(long value,char __far *buffer,int radix);
char __far * _ultoa(unsigned long value,char __far *buffer,int radix);

#ifdef __cplusplus
};
#endif
