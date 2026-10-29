CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O0 -fopenmp
all: barreira
barreira: barreira.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f barreira
.PHONY: all clean
