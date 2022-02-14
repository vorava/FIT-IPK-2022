# IPK projekt 1 - Makefile
# Vojtech Orava (xorava02)

CFLAGS = -std=gnu99 -Wall -Wextra -Werror
CC = gcc
BIN = hinfosvc
DEPS = system_info.h
OBJ = system_info.o


$(BIN): $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) $^ main.c -o $(BIN)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(BIN)
	./$(BIN) 12345

.PHONY clean:
	rm *.o $(BIN)
