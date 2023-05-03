#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus { // copied from assignment
unsigned short SigSize;
char virusName[16];
unsigned char* sig;
} virus;

virus* readVirus(FILE *file){
    virus *currVirus = malloc(sizeof(virus));
    fread(&((*currVirus).SigSize),sizeof(unsigned short),1,file); //read sigSize from file and update sigSize field of currVirus
    if((*currVirus).SigSize == 0) // if we read the last virus we terminate
        return NULL;
    fread((*currVirus).virusName,16,1,file);//read virusName from file and update virusName field of currVirus
    (*currVirus).sig = (unsigned char*)malloc((*currVirus).SigSize * sizeof(unsigned char));
    fread((*currVirus).sig,sizeof(unsigned char),(*currVirus).SigSize,file);//read sig from file and update sig field of currVirus
    return currVirus;
}

void printVirus(virus *currVirus, FILE* output){
    fprintf(output,"Virus name: %s \n", (*currVirus).virusName);
    fprintf(output,"Virus size: %hu \n", (*currVirus).SigSize);
    fprintf(output,"Signature:\n");
    for(int i=0; i < (*currVirus).SigSize ;i++)
        fprintf(output, "%02X ", (*currVirus).sig[i]);
    fprintf(output, "\n\n");
}

void printAllViruses(FILE *file){ // gets file and print all the viruses in file
    char fileType[4];
    fread(fileType, sizeof(char), 4, file);
    if(strcmp("VISL",fileType) != 0){
        printf("%s","WRONG FILE TYPE. \n");
        exit(1);
    }
    virus *currVirus = readVirus(file); //reading the first virus
    if(currVirus &&(*currVirus).SigSize != 0){
        printVirus(currVirus,stdout);
    }
    while(currVirus){ //while we still have viruses to read
        currVirus = readVirus(file);
        if(currVirus && (*currVirus).SigSize != 0)
            printVirus(currVirus,stdout);
    }

}

int main(int argc, char **argv){
    FILE *file = fopen("signatures-L", "rb");
    printAllViruses(file);
    return 0;
}