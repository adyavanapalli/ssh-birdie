CC=gcc
CCFLAGS=-Wall -Wextra -Wpedantic
DEBUG=-O0 -ggdb
INCLUDE=-I include/
OPT=-O3

all: bin/ssh-birdie

bin/ssh-birdie: src/ssh-birdie.c obj/daemon.o
	$(CC) $(CCFLAGS) $(INCLUDE) $(DEBUG) src/ssh-birdie.c -o bin/ssh-birdie obj/daemon.o

obj/daemon.o: src/daemon.c include/daemon.h
	$(CC) $(CCFLAGS) $(INCLUDE) $(DEBUG) -c src/daemon.c -o obj/daemon.o

clean:
	rm -f bin/* obj/*

.PHONY=all clean
