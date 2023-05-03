all:antiVirus

antiVirus:antiVirus.o
	gcc -m32 -g -Wall -o virusDetector antiVirus.o

antiVirus.o:antiVirus.c
	gcc -g -Wall -m32 -c -o antiVirus.o antiVirus.c

.PHONY: clean 

clean: 
	rm -f *.o antiVirus 
	rm -f *.o virusDetector 