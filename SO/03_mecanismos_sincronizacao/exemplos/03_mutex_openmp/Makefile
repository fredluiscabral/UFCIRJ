CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O0 -fopenmp -pthread
all: mutex
mutex: mutex.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f mutex
.PHONY: all clean
