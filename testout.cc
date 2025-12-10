#include <stdio.h>
#include <string.h>

int main(int argc,char** argv)
{
  if (argc<2)
  {
    puts("filename required");
    return 1;
  }

  FILE* file=fopen(argv[1], "w");
  if (!file)
  {
    puts("open error");
    return 1;
  }

  setvbuf(stdin,NULL,_IONBF,0);
  setvbuf(file,NULL,_IONBF,0);

  int count=0;
  while(1)
  {
    char line[30];
    char numStr[30];
    strcpy(line,"_test");
    sprintf(numStr,"%d",count);
    strcat(line,numStr);
    strcat(line,"line_\n");
    fprintf(stderr,"%s",line);
    fprintf(file,"%s",line);
    int inChar=getchar();
    if (inChar=='q') {break;}
    ++count;
  }

  fclose(file);
  return 0;
}