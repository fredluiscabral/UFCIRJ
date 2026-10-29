CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -O0 -fopenmp -pthread
all: buffer
buffer: buffer.c
	$(CC) $(CFLAGS) $< -o $@
correto: buffer
	./buffer
errado: buffer
	@echo "Encerrando demonstracao apos 2 segundos (deadlock esperado)..."
	@timeout 2s ./buffer errado; rc=$$?; if [ $$rc -eq 124 ]; then echo "Deadlock demonstrado (timeout)."; else exit $$rc; fi
clean:
	rm -f buffer
.PHONY: all correto errado clean
