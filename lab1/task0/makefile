all: clean task0 

task0: count-words.o
	gcc -m32 -g -Wall -o task0 count-words.o

count-words.o: count-words.c 
	gcc -g -Wall -m32 -c -o count-words.o count-words.c 

.PHONY: clean 

clean: 
	rm -f *.o task0 

