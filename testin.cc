#include <stdio.h>
#include <string.h>

int main(int argc,char** argv)
{
  if (argc<2)
  {
    puts("filename required");
    return 1;
  }

  FILE* file=fopen(argv[1], "rb");
  if (!file)
  {
    puts("open error");
    return 1;
  }

  setvbuf(file,NULL,_IONBF,0);

  while(1)
  {
    int c=getc(file);
    printf("Got code=%02X symbol=%c\n",c,(c<32)?'?':c);
  }

  fclose(file);
  return 0;
}
