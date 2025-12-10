#include <stdio.h>
#include <string.h>

int main(int argc,char** argv)
{
  FILE* file=fopen("com2com$", "rb");
  if (!file)
  {
    puts("open log device error");
    return 1;
  }

  setvbuf(file,NULL,_IONBF,0);
  setvbuf(stdout,NULL,_IONBF,0);

  while(1)
  {
    int c=getc(file);
    putchar(c);
  }

  fclose(file);
  return 0;
}
