#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  printf("The value of num is %d\n", argc);
  for(int i=1; i<argc; i++){
    fputs(argv[i],stdout);
    fprintf(stdout,"\n");
  }
  return 0;
}
