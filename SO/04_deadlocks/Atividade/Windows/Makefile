CC = gcc
CFLAGS = -std=c11 -O0 -Wall -Wextra

all: 01_lockfileex_threads.exe

01_lockfileex_threads.exe: 01_lockfileex_threads.c
	$(CC) $(CFLAGS) $< -o $@

demo: 01_lockfileex_threads.exe
	.\01_lockfileex_threads.exe

.PHONY: all demo
