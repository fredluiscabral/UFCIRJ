CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O2 -fopenmp
all: spinlock
spinlock: spinlock.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f spinlock
.PHONY: all clean
