# IPK projekt 2 - Makefile
# Vojtech Orava (xorava02)

CFLAGS = -std=gnu99 -Wall -Wextra -Werror
CC = gcc
BIN = ipk-sniffer
DEPS = arg_parser.h filter.h packet.h
OBJ = arg_parser.o filter.o packet.o


$(BIN): $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) $^ main.c -o $(BIN) -lpcap

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(BIN)
	./$(BIN)

.PHONY clean:
	rm *.o $(BIN)
