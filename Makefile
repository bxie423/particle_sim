CC=gcc
CFLAGS=-std=c99 -Wall
LDFLAGS=-lm
SRC=parseinput.c collision.c simulation.c main.c
OBJ=parseinput.o collision.o simulation.o main.o
LEX=flex
YACC=bison
YFLAGS=-d

all: configparse initparse main

configparse: configparse.y configparse.l
	bison -d configparse.y
	flex -o configparse.c configparse.l
	gcc -o configparse configparse.c configparse.tab.c -lfl

initparse: initparse.y initparse.l
	bison -d initparse.y
	flex -o initparse.c initparse.l
	gcc -o initparse initparse.c initparse.tab.c -lfl

main: ${OBJ}

parseinput.o: main.h
collision.o: main.h
simulation.o: main.h
main.o: main.h

clean:
	rm -f configparse configparse.tab.{c,h} configparse.c initparse initparse.tab.{c,h} initparse.c main ${OBJ}
