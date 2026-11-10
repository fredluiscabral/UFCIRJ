CC = gcc
CFLAGS = -std=c11 -O0 -Wall -Wextra -pthread

PROGRAMAS = 01_produtor_consumidor 02_mutexes 03_filosofos 04_banqueiro

all: $(PROGRAMAS)

01_produtor_consumidor: 01_produtor_consumidor.c
	$(CC) $(CFLAGS) $< -o $@

02_mutexes: 02_mutexes.c
	$(CC) $(CFLAGS) $< -o $@

03_filosofos: 03_filosofos.c
	$(CC) $(CFLAGS) $< -o $@

04_banqueiro: 04_banqueiro.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMAS)

.PHONY: all clean
