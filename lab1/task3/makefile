all: clean task3 

task3: menu.o
	gcc -m32 -g -Wall -o task3 menu.o

menu.o: menu.c 
	gcc -g -Wall -m32 -c -o menu.o menu.c 

.PHONY: clean 

clean: 
	rm -f *.o task3

