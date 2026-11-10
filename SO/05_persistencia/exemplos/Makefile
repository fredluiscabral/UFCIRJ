CC = gcc
CFLAGS = -std=c11 -O0 -Wall -Wextra -pedantic
PROGRAMAS = 01_arquivo 02_lseek 03_metadados 04_fsync

.PHONY: all clean
all: $(PROGRAMAS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMAS)

# Os arquivos .txt gerados nas demonstracoes NAO sao apagados por clean.
