#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv) {
    int debugMode =0; //0 == false
    int i;
    for(i=1; i<argc; i++){
        if(strcmp(argv[i],"+D")==0){
            debugMode = 1;
        }
        else if(strcmp(argv[i],"-D")==0){
            debugMode = 0;
        }
        if(debugMode == 1 && i < argc-1 && !(strcmp(argv[i+1],"-D")==0)) // printing info without +-D
            fprintf(stderr,"%s\n",argv[i+1]);
    }
}
