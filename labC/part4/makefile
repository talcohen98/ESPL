all: task4 looper

task4: myshell.o LineParser.o
	gcc -g -m32 -Wall -o task4 myshell.o LineParser.o

myshell.o: myshell.c
	gcc -g -m32 -Wall -c myshell.c -o myshell.o

LineParser.o: LineParser.c
	gcc -g -m32 -Wall -c LineParser.c -o LineParser.o

looper: looper.c
	gcc -g -m32 -Wall looper.c -o looper
	
.PHONY: clean
clean:
	rm -f *.o task4
	rm -f *.o looper