CC=gcc
CCFLAGS=-Wall -Wextra -Wpedantic
HEADER=include/constants.h include/daemon.h include/telegram-bot-api-client.h include/logger.h
INCLUDE=-I include/
LDFLAGS=-lsystemd -lcurl
OBJ=obj/daemon.o obj/telegram-bot-api-client.o obj/logger.o
OPT=-O3

all: bin/ssh-birdie

bin/ssh-birdie: src/ssh-birdie.c $(HEADER) $(OBJ)
	$(CC) $(CCFLAGS) $(INCLUDE) $(OPT) src/ssh-birdie.c -o bin/ssh-birdie $(OBJ) $(LDFLAGS)

obj/daemon.o: src/daemon.c $(HEADER)
	$(CC) $(CCFLAGS) $(INCLUDE) $(OPT) -c src/daemon.c -o obj/daemon.o

obj/telegram-bot-api-client.o: src/telegram-bot-api-client.c $(HEADER)
	$(CC) $(CCFLAGS) $(INCLUDE) $(OPT) -c src/telegram-bot-api-client.c -o obj/telegram-bot-api-client.o -lcurl

obj/logger.o: src/logger.c $(HEADER)
	$(CC) $(CCFLAGS) $(INCLUDE) $(OPT) -c src/logger.c -o obj/logger.o

clean:
	rm -f bin/* obj/*

.PHONY=all clean
