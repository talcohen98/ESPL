all: clean task1 

task1: addresses.o
	gcc -m32 -g -Wall -o task1 addresses.o

addresses.o: addresses.c 
	gcc -g -Wall -m32 -c -o addresses.o addresses.c 

.PHONY: clean 

clean: 
	rm -f *.o task1 

