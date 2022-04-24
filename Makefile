CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic

PROGRAMS = mainP1 mainP2 mainP3 mainP4 mainP2_4

all : $(PROGRAMS)

mainP1 : mainP1.o Partie1.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

mainP2 : mainP2.o Partie1.o Partie2.o Partie2_4.o 
	$(CC) -g -o $@ $(CFLAGS) $^ -lm

mainP2_4 : mainP2_4.o Partie2_4.o Partie1.o Partie2.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

mainP3 : mainP3.o Partie1.o Partie2.o Partie3.o
	$(CC) -ggdb -o $@ $(CFLAGS) $^ -lm
	
mainP4 : mainP4.o Partie1.o Partie2.o Partie3.o Partie4.o 
	$(CC) -o $@ $(CFLAGS) $^ -lm -lssl -lcrypto

mainP1.o : mainP1.c
	gcc -c mainP1.c

mainP2.o : mainP2.c
	gcc -c mainP2.c

mainP3.o : mainP3.c
	gcc -c mainP3.c

mainP4.o : mainP4.c
	gcc -c mainP4.c

Partie1.o : Partie1.c Partie1.h
	gcc -c Partie1.c 
	
Partie2.o : Partie2.c Partie2.h
	gcc -c Partie2.c 
	
Partie2_4.o : Partie2_4.c Partie2_4.h
	gcc -c Partie2_4.c

Partie3.o : Partie3.c Partie3.h
	gcc -c Partie3.c

Partie4.o : Partie4.c Partie4.h
	gcc -c Partie4.c
	
clean: 
	rm -f *.o* ~$(PROGRAMS)
