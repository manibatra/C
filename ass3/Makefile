FLAGS = gcc -g -Wall -pedantic -std=gnu99

all : handler fromfile waller simple slow slow2

handler : handler.c shared.o
	$(FLAGS) shared.o $@.c -o handler

slow2 : slow2.c shared.o
	$(FLAGS) shared.o $@.c -o slow2

slow : slow.c shared.o
	$(FLAGS) shared.o $@.c -o slow

waller : waller.c shared.o
	$(FLAGS) shared.o $@.c -o waller

simple : simple.c shared.o
	$(FLAGS) shared.o $@.c -o simple

fromfile : fromFile.c shared.o
	$(FLAGS) shared.o fromFile.c -o fromfile

shared.o : shared.c shared.h
	$(FLAGS) -c shared.c -o shared.o 
