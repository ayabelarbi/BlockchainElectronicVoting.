CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic

PROGRAMS = main

all : $(PROGRAMS)

main : main.o Partie1.o Partie2.o Partie2_4.o Partie3.o Partie4.à 
	$(CC) -o $@ $(CFLAGS) $^ -lm

main.o : main.c
	gcc -c main.c

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
