all: task1

task1: myPipeLine.o
	gcc -g -m32 -Wall -o task1 myPipeLine.o

myPipeLine.o: myPipeLine.c
	gcc -m32 -Wall -c myPipeLine.c -o myPipeLine.o

.PHONY: clean

clean:
	rm -f *.o task1
