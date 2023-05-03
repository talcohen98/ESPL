#include <stdio.h>
#include <stdlib.h>


void printHex(char buffer[], int length){
    for(int i=0; i<length;i++)
        printf("%02X ", (unsigned char)buffer[i]); // printf expects unsigned char so we cast each char
    printf("\n");
}
int main(int argc, char **argv){
    FILE *inputFile = fopen(argv[1],"r");

    fseek(inputFile,0,SEEK_END); // checking how much space we need to assign
    int size = ftell(inputFile);
    fseek(inputFile,0,SEEK_SET);

    char buffer[size * sizeof(char)];
    fread(buffer,size,1,inputFile);
    printHex(buffer,size);
    fclose(inputFile);

    return 0;
}