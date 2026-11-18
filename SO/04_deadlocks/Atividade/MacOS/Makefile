CC = cc
CFLAGS = -std=c11 -O0 -Wall -Wextra

all: 01_flock_threads 02_fcntl_processos

01_flock_threads: 01_flock_threads.c
	$(CC) $(CFLAGS) -pthread $< -o $@

02_fcntl_processos: 02_fcntl_processos.c
	$(CC) $(CFLAGS) $< -o $@

demo-flock: 01_flock_threads
	./01_flock_threads

demo-fcntl: 02_fcntl_processos
	./02_fcntl_processos

clean:
	rm -f 01_flock_threads 02_fcntl_processos

.PHONY: all demo-flock demo-fcntl clean
