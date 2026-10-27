CC = gcc
CFLAGS = -O0 -Wall -Wextra -fopenmp -pthread
SOURCES = $(wildcard [0-9][0-9]_*.c)
TARGETS = $(SOURCES:.c=)
.PHONY: all clean run
all: $(TARGETS)
%: %.c
	$(CC) $(CFLAGS) $< -o $@
run: all
	@for p in $(TARGETS); do echo "=== $$p ==="; ./$$p; done
clean:
	rm -f $(TARGETS)
