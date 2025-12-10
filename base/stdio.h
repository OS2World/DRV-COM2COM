#ifdef __cplusplus
extern "C" {
#endif

void dev_vsprintf( char __far *, const char __far *, va_list );
void dev_sprintf( char __far *, const char __far *, ... );

#ifdef FILEIO_PRESENT
void dev_printf( const char __far *, ... );
void dev_vprintf( const char __far *, va_list );
void dev_fprintf( FILE __far *,const char __far *, ... );
#endif

#ifdef __cplusplus
};
#endif
