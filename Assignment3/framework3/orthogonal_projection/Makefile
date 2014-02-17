CC=gcc
CFLAGS=$(shell sdl-config --cflags) -Wall -std=c99 -pedantic-errors
LDFLAGS=$(shell sdl-config --libs) -lm -lGL -lGLU -lglut

all: main

main: main.o ortho.o
	$(CC) -o main main.o ortho.o $(CFLAGS) $(LDFLAGS)

main.o: main.c ortho.h
	$(CC) $(CFLAGS) -c -o main.o main.c

ortho.o: ortho.h ortho.c
	$(CC) $(CFLAGS) -c -o ortho.o ortho.c

realclean: clean
	rm -f *~

clean:
	rm -f *.o
	rm -f main
