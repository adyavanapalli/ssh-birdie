CC=gcc
CCFLAGS=-Wall -Wextra -Wpedantic
INCLUDE=-I include/
OPT=-O3

all: bin/ssh-birdie

bin/ssh-birdie: src/ssh-birdie.c obj/daemon.o
	$(CC) $(CCFLAGS) $(INCLUDE) $(OPT) src/ssh-birdie.c -o bin/ssh-birdie obj/daemon.o

obj/daemon.o: src/daemon.c include/daemon.h
	$(CC) $(CCFLAGS) $(INCLUDE) $(OPT) -c src/daemon.c -o obj/daemon.o

clean:
	rm -f bin/* obj/*

.PHONY=all clean
