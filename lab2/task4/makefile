all: task4 looper mypipe

task4: myshell.o LineParser.o
	gcc -g -m32 -Wall -o task4 myshell.o LineParser.o

myshell.o: myshell.c
	gcc -m32 -Wall -c myshell.c -o myshell.o

LineParser.o: LineParser.c
	gcc -m32 -Wall -c LineParser.c -o LineParser.o

mypipe: mypipe.c
	gcc -m32 -Wall mypipe.c -o mypipe

looper: looper.c
	gcc -m32 -Wall looper.c -o looper
	
.PHONY: clean
clean:
	rm -f *.o task4
	rm -f *.o looper
	rm -f *.o mypipe