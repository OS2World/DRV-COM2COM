#include "base/stdarg.h"
#include "base/stdlib.h"

/*
static void lead( char c, int num, char *buff )
{
  int len,diff,i;

  len = strlen( buff );
  diff = num-len;
  if( diff <= 0 )
  {
    return;
  }
  for( i=len;i>=0;i-- )
  {
    buff[diff+i] = buff[i];
  }
  for( i=0;i<diff;i++ )
  {
    buff[i] = c;
  }
}
*/
static char __far *cStr;

#ifdef FILEIO_PRESENT
static FILE __far *cFile;
__inline void inline barfChar( char ch )
{
  if( cStr!=NULL )
  {
    *cStr=ch;
    ++cStr;
  }
  else
  {
    fputc( ch, cFile );
  }
}
#else
__inline void barfChar( char ch )
{
  *cStr=ch;
  ++cStr;
}
#endif

__inline void barfString( char __far * str )
{
  while(*str)
  {
    barfChar(*str);
    ++str;
  }
}


static void basePrintf( const char __far *in, va_list al )
{
  char cin;
  int  i,j;
  long l;
  char buff[256],*tmp;
  int formatError=0;

  while(!formatError && *in)
  {
    int width=0;
    char pad=' ';
    char typeModifier=0;

    if( *in != '%' )
    {
      barfChar(*in);
      ++in;
      continue;
    }

    ++in;
    if (*in=='%')
    {
      barfChar( '%' );
      ++in;
      continue;
    }

    /* control flags are not supported */
    if (*in=='0')
    {
      pad='0';
      ++in;
    }

    while (*in>='0' && *in<='9')
    {
      width=width*10+(*in-'0');
      ++in;
    }

    switch (*in)
    {
    case 'h':
    case 'l':
    case 'L':
    case 'w':
      typeModifier=*in;
      ++in;
    }

    switch( *in )
    {
    case 'c':
      barfChar( va_arg( al, char ) );
      ++in;
      break;
    case 'd':
    case 'i':
      if (typeModifier=='l')
      {
        l = va_arg( al, long );
        _ltoa( l, buff, 10 );
      }
      else
      {
        i = va_arg( al, int );
        _itoa( i, buff, 10 );
      }
      barfString(buff);
      ++in;
      break;
    case 's':
      tmp = va_arg( al, char * );
      barfString(tmp);
      ++in;
      break;
    case 'u':
      if (typeModifier=='l')
      {
        l = va_arg( al, long );
        _ultoa( (unsigned long)l, buff, 10 );
      }
      else
      {
        i = va_arg( al, int );
        _utoa( (unsigned int)i, buff, 10 );
      }
      barfString(buff);
      ++in;
      break;
    case 'x':
    case 'X':
      if (typeModifier=='l')
      {
        l = va_arg( al, long );
        _ultoa( (unsigned long)l, buff, 16 );
      }
      else
      {
        i = va_arg( al, int );
        _utoa( (unsigned int)i, buff, 16 );
      }
      barfString(buff);
      ++in;
      break;
    default:
      formatError=1;
    }
  }

  barfChar(0);
}

void dev_sprintf( char __far *out, const char __far *str, ... )
{
  va_list     al;

  va_start( al, str );
  cStr = out;
  basePrintf( str, al );
  *cStr = 0;
}

void dev_vsprintf( char __far *out, const char __far *str, va_list al )
{
  cStr = out;
  basePrintf( str, al );
  *cStr = 0;
}

#ifdef FILEIO_PRESENT
void dev_printf( const char __far *str, ... )
{
  va_list     al;

  cStr = NULL;
  cFile = stdout;
  va_start( al, str );
  basePrintf( str, al );
}

void dev_vprintf( const char __far *str, va_list al )
{
  cStr = NULL;
  cFile = stdout;
  basePrintf( str, al );
}

void dev_fprintf( FILE __far *fp, const char __far *str, ... )
{
  va_list al;

  cStr = NULL;
  cFile = fp;
  va_start( al, str );
  basePrintf( str, al );
}
#endif
