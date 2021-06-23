# ssh-birdie

_🐦 A little birdie told me \<@adyavanapalli\> (SSH'd into | disconnected from) the server 🐦._

## Purpose

A daemon that watches the syslog for SSH connections and sends a message to
Telegram as a bot user detailing the connection.

## Building

To build, simply run:

```bash
$ make

# The output you should see:
gcc -Wall -Wextra -Wpedantic -I include/ -O3 -c src/daemon.c -o obj/daemon.o
gcc -Wall -Wextra -Wpedantic -I include/ -O3 src/ssh-birdie.c -o bin/ssh-birdie obj/daemon.o
```

## Running

The compiled program is output to `bin/` and you can run it using:

```bash
$ bin/ssh-birdie

# You should see no output.
```
