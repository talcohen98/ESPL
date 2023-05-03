#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus { // copied from assignment
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link link; // copied from assignment
struct link{
    link *nextVirus;
    virus *vir;
};

link *virus_list = NULL; // list init
FILE *virus_file = NULL; // sus file init
char *buffer = NULL; // buffer init
char *susFileName = NULL; // susFileName init

link *list_append(link *virus_list, virus * data){
    link *currLink = (link*)malloc(sizeof(link)); //making a new link using the virus we read 
    (*currLink).nextVirus = NULL;
    (*currLink).vir = data;
    if(virus_list == NULL)
        virus_list = currLink;
    else{
        link *lastLink = virus_list;
        while((*lastLink).nextVirus != NULL) // finding the last link in virus_list
            lastLink = (*lastLink).nextVirus;
        (*lastLink).nextVirus = currLink; // adding the curr link
    }
    return virus_list;
}

virus* readVirus(FILE *file){
    if(ftell(file) == 0){ // (= if this is the beginning of the file) checking file type
        char fileType[4];
        fread(fileType, sizeof(char), 4, file);
        if(strcmp("VISL",fileType) != 0){
            printf("%s","WRONG FILE TYPE. \n");
            exit(1);
        }
    }
    virus *currVirus = malloc(sizeof(virus));
    memset(currVirus, 0, sizeof(virus)); // initialize SigSize to zero
    fread(&((*currVirus).SigSize),sizeof(unsigned short),1,file); //read sigSize from file and update sigSize field of currVirus
    if((*currVirus).SigSize == 0){ // if we read the last virus we terminate
        free(currVirus);
        return NULL;
    }
    fread(&((*currVirus).virusName),16,1,file); //read virusName from file and update virusName field of currVirus
    (*currVirus).sig = (unsigned char*)calloc((*currVirus).SigSize, sizeof(unsigned char));

    if ((*currVirus).sig == NULL) { // make sure calloc was successful
        printf("%s","FAILED TO ALLOCATE MEMORY FOR VIRUS SIGNATURE. \n");
        exit(1);
    }
    
    fread((*currVirus).sig,sizeof(unsigned char),(*currVirus).SigSize,file); //read sig from file and update sig field of currVirus
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
    char fileType[4]; // checking file type
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

void list_print(link *virus_list, FILE *file){
    link *currLink = virus_list;
    while (currLink)
    {
        virus *currVirus = (*currLink).vir;
        printVirus(currVirus,file);
        currLink = (*currLink).nextVirus;
    }
}

void list_free(link *virus_list){
    link *currLink = virus_list;
    while (currLink){
        link *nextLink = (*currLink).nextVirus;
        free((*(*currLink).vir).sig);
        free((*currLink).vir);
        free(currLink);
        currLink = nextLink;
    }
}

//copied from lab1 with small change
struct fun_desc{
  char *name;
  void (*fun)();
};

void loadSignatures(){
    printf("Enter file name: ");
    char fileName[256];
    fgets(fileName, 256 * sizeof(char), stdin);
    strtok(fileName, "\n"); // removing the \n from fileName

    FILE *fileCheck = fopen(fileName, "rb"); // checking if file exists
    if(fileCheck == NULL){
        printf("FILE -%s- DOESNT EXIST. \n",fileName);
        return;
    }

    char* fileType = malloc(5 * sizeof(char)); // checking file type CHANGED TO 5
    memset(fileType,0,5); // initializing fileType with 0
    if(fileType == NULL){
        printf("FAILED TO ALLOCATE MEMORY FOR FILE TYPE\n");
        fclose(fileCheck);
        return; 
    }

    if(fread(fileType, sizeof(char), 4, fileCheck) != 4){
        printf("%s","FAILED READING FILE TYPE. \n");
        free(fileType);
        fclose(fileCheck);
        return; 
    }

    if(strcmp("VISL",fileType) != 0){
        printf("%s","WRONG FILE TYPE. \n");
        free(fileType);
        fclose(fileCheck);
        return; 
    }

    //loading viruses from file to virus_list
    virus *currVirus = readVirus(fileCheck); //reading the first virus
    while(currVirus != NULL){
        if((*currVirus).SigSize == 0)
            break;
        virus_list = list_append(virus_list,currVirus);
        currVirus = readVirus(fileCheck);
    }

    free(fileType);
    fclose(fileCheck);
}


void printSignatures(){
    list_print(virus_list,stdout);
}

unsigned int getFileSize(FILE* file){
    unsigned int size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

void detectVirus(char *buffer,unsigned int size, link *virus_list){
    if(virus_list == NULL){
        return;
    }
    printf("\nViruses in file:\n");
    link *currLink = virus_list;
    while(currLink != NULL){
        for(int i=0; i< size; i++){
            if(memcmp((*(*currLink).vir).sig, buffer+i, (*(*currLink).vir).SigSize) ==0){ // checking if they are equal
                virus *virusFound = (*currLink).vir;
                printf("Starting byte location in the suspected file: %d\n", i);
                printf("Virus name: %s\n", (*virusFound).virusName);
                printf("Size of the virus signature: %d\n\n", (*virusFound).SigSize);
            }
        }
        currLink = (*currLink).nextVirus;
    }
}

void detectVirusWRAPPER(){ // helper func. reads new file to buffer, extracts size, calls detectVirus
    strtok(susFileName, "\n"); // removing the \n from fileName
    FILE *fileCheck = fopen(susFileName, "r"); // checking if file exists
    if(fileCheck == NULL){
        printf("FILE -%s- DOESNT EXIST. \n",susFileName);
        fclose(fileCheck);
        return;
    }else{ // we read from file to buffer
        buffer = malloc(10000);
        memset(buffer,0,10000); // initializing buffer with 0
        unsigned int scanSize; 
        if(getFileSize(fileCheck) > 10000)
            scanSize = 10000;
        else
            scanSize = getFileSize(fileCheck);
        fread(buffer,sizeof(unsigned char),scanSize,fileCheck);
        detectVirus(buffer,scanSize , virus_list);
        free(buffer);
    }
    fclose(fileCheck);
}

void neutralize_virus(char *fileName, int signatureOffset){
    FILE *fileToFix = fopen(fileName,"w+");
    char new_val = 0x3c;
    if(fseek(fileToFix, signatureOffset, SEEK_SET) !=0){
        printf("failed seeking for virus in file.\n");
        fclose(fileToFix);
        return;
    }else if(fwrite(&new_val,1,1,fileToFix) !=1){ // updating file
        printf("failed seeking for virus in file.\n");
        fclose(fileToFix);
        return;
    }
    fclose(fileToFix);   
}

void fixFile(){
    strtok(susFileName, "\n"); // removing the \n from fileName
    FILE *fileCheck = fopen(susFileName, "r"); // checking if file exists
    if(fileCheck == NULL){
        printf("FILE -%s- DOESNT EXIST. \n",susFileName);
    }else{ // we read from file to buffer
        buffer = malloc(10000);
        memset(buffer,0,10000); // initializing buffer with 0
        unsigned int scanSize; 
        if(getFileSize(fileCheck) > 10000)
            scanSize = 10000;
        else
            scanSize = getFileSize(fileCheck);
        fread(buffer,sizeof(unsigned char),scanSize,fileCheck);
        link *currLink = virus_list;
        while(currLink != NULL){
            for(int i=0; i< scanSize; i++){
                if(memcmp((*(*currLink).vir).sig, buffer+i, (*(*currLink).vir).SigSize) ==0){ // checking if they are equal
                    int startingByte = i;
                    neutralize_virus(susFileName, startingByte);
                }
            }
            currLink = (*currLink).nextVirus;
        }

    }
    fclose(fileCheck);
    free(buffer);
    printf("\nRemoved all viruses from file - %s - successfully. \n", susFileName);

}

void quit(){
    list_free(virus_list);
    free(susFileName);
    exit(0);
}

int main(int argc, char **argv){
    //copied from lab 1
    struct fun_desc menu[] = {
        {"load signatures", loadSignatures},
        {"print signatures", printSignatures},
        {"detect viruses", detectVirusWRAPPER},
        {"fix file", fixFile},
        {"quit", quit},
        {NULL, NULL}
    };

    if(argc >1){
        susFileName = malloc(strlen(argv[1]) + 1);
        strcpy(susFileName, argv[1]); 
    }

    int numOfElements = sizeof(menu)/sizeof(menu[0]);
    int upperBound = sizeof(menu) / sizeof(struct fun_desc) -2 + 48; // -1 because we start with 0, -1 because of null, added 48 to change value to ascii
    while(1){
        printf("Choose a function from the list below:\n");
        for(int i=0; i<numOfElements -1; i++){
            printf("(%d) %s\n",i+1,menu[i].name); // added +1 so it starts with 1
        }
        char input[50];
        printf("option: ");
        fgets(input,sizeof(input),stdin);
        if (feof(stdin)) { // if ctrl+d entered we terminate without printing anything.
            printf("\n");
            break;
        }
        else if(input[0] >= 49 && input[0] <= upperBound +1){
            printf("Within bounds\n");
            int usersChoice = input[0] -48 -1; // -48 to change from char to int
            menu[usersChoice].fun();
            printf("\n");
        }else{
            printf("Not Within bounds\n");
        }
    }
    list_free(virus_list);
    return 0;
}