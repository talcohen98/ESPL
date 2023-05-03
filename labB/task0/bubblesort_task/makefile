all:bubblesort

bubblesort:lab3_bubblesort.o
	gcc -m32 -g -Wall -o bubblesort lab3_bubblesort.o

lab3_bubblesort.o:lab3_bubblesort.c
	gcc -g -Wall -m32 -c -o lab3_bubblesort.o lab3_bubblesort.c

.PHONY: clean 

clean: 
	rm -f *.o bubblesort 