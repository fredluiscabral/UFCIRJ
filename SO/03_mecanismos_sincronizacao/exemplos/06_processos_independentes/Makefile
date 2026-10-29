CC=gcc
CFLAGS=-std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -O0 -pthread
all: iniciar incrementar consultar limpar
%: %.c comum.h
	$(CC) $(CFLAGS) $< -o $@
demo: all
	./iniciar
	./incrementar & ./incrementar & wait; ./consultar
	./limpar
clean:
	rm -f iniciar incrementar consultar limpar
.PHONY: all demo clean
