all: task1b

task1b: myshell.o LineParser.o
	gcc -g -m32 -Wall -o task1b myshell.o LineParser.o

myshell.o: myshell.c
	gcc -m32 -Wall -c myshell.c -o myshell.o

LineParser.o: LineParser.c
	gcc -m32 -Wall -c LineParser.c -o LineParser.o
	
.PHONY: clean
clean:
	rm -f *.o task1b