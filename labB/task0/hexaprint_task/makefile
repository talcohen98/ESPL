all:hexaprint

hexaprint:hexaPrint.o
	gcc -m32 -g -Wall -o hexaprint hexaPrint.o

hexaPrint.o:hexaPrint.c
	gcc -g -Wall -m32 -c -o hexaPrint.o hexaPrint.c

.PHONY: clean 

clean: 
	rm -f *.o hexaPrint 