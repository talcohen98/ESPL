#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// copied from part2:

char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  /* TODO: Complete during task 2.a */
  for(int i=0; i<array_length; i++) {
    char input = (*f)(array[i]);
    if(input == '\n'){
      mapped_array[i] = '\0';
      break;
    }
    mapped_array[i] = input;
  }
  return mapped_array;
}

/* Ignores c, reads and returns a character from stdin using fgetc. */
char my_get(char c){
  char input = fgetc(stdin);
  // printf(" && %c\n", input);
  return input;
}

/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns the value of c unchanged. */
char cprt(char c){
  if(c >= 0x20 && c <=0x7E){
    printf("%c\n",c); // if we want the char itself - %c
  }else{
    printf(".\n");
  }
  return c;
}

/* Gets a char c and returns its encrypted form by adding 1 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */
char encrypt(char c){
  if(c >= 0x20 && c <=0x7E){
    c = c+1;
  }
  return c;
}

/* Gets a char c and returns its decrypted form by reducing 1 from its value. If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c){
  if(c >= 0x20 && c <=0x7E){
    c = c-1;
  }
  return c;
}

/* xprt prints the value of c in a hexadecimal representation followed by a new line, and returns c unchanged. */
char xprt(char c){
  if(c >= 0x20 && c <=0x7E){
    printf("%x\n",c); // print hexa value
  }else{
    printf(".\n");
  }
  return c;
}

struct fun_desc{
  char *name;
  char (*fun)(char);
};

int main(int argc, char **argv){
  /* TODO: Test your code */
  char *carray = malloc(5);
  carray[0] = '\0'; //pointer to empty string
  struct fun_desc menu[] = {{"get string",my_get},{"print string",cprt},{"encrypt",encrypt},{"decrypt",decrypt},{"print hex",xprt},{NULL,NULL}};
  int upperBound = sizeof(menu) / sizeof(struct fun_desc) -2 + 48; // -1 because we start with 0, -1 because of null, added 48 to change value to ascii
  while(1){
    printf("Choose a function from the list below:\n");
    for(int i=0; i<5; i++){
      printf("(%d) %s\n",i,menu[i].name);
    }
    char input[50];
    printf("option: ");
    fgets(input,sizeof(input),stdin);

    if (feof(stdin)) { // if ctrl+d entered we terminate wihout printing anything.
      break;
    }
    else if(input[0] >= 48 && input[0] <= upperBound){
      printf("Within bounds\n");
    }else{
      printf("Not Within bounds\n");
      free(carray);
      exit(0);
    }
    carray = map(carray, 5, menu[input[0]-48].fun); //reduced 48 to get from ascii to int val
    printf("DONE.\n");
  }
  return 0;
}

