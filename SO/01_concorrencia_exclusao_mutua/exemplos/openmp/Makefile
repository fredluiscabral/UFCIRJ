CC ?= gcc
CFLAGS ?= -O0 -Wall -Wextra -fopenmp
SOURCES := $(wildcard *.c)
PROGRAMS := $(SOURCES:.c=)
.PHONY: all clean
all: $(PROGRAMS)
%: %.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f $(PROGRAMS)
