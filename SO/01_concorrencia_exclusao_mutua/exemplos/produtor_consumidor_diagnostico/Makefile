CC = gcc
CFLAGS = -std=c11 -O0 -Wall -Wextra -pthread
TARGET = produtor_consumidor

all: $(TARGET)

$(TARGET): produtor_consumidor.c
	$(CC) $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
