CC = gcc
CFLAGS = -O0 -Wall -Wextra -pthread
TARGET = produtor_consumidor

.PHONY: all run clean
all: $(TARGET)
$(TARGET): produtor_consumidor.c
	$(CC) $(CFLAGS) $< -o $@
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f $(TARGET)
