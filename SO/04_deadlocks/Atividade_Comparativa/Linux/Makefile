CC = gcc
CFLAGS = -std=c11 -O0 -Wall -Wextra

all: 01_flock_threads 02_fcntl_processos

01_flock_threads: 01_flock_threads.c
	$(CC) $(CFLAGS) -pthread $< -o $@

02_fcntl_processos: 02_fcntl_processos.c
	$(CC) $(CFLAGS) $< -o $@

teste_flock: 01_flock_threads
	@timeout 18s ./01_flock_threads; r=$$?; \
	if [ $$r -eq 124 ]; then echo "Tempo de observacao encerrado pelo timeout."; \
	elif [ $$r -ne 0 ]; then exit $$r; fi

teste_fcntl: 02_fcntl_processos
	@timeout 18s sh -c './02_fcntl_processos A.txt B.txt & ./02_fcntl_processos B.txt A.txt & wait'; r=$$?; \
	if [ $$r -eq 124 ]; then echo "Tempo de observacao encerrado pelo timeout."; \
	elif [ $$r -ne 0 ]; then exit $$r; fi

clean:
	rm -f 01_flock_threads 02_fcntl_processos

.PHONY: all teste_flock teste_fcntl clean
