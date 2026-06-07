CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17

build: main.c
	@mkdir -p bin
	@$(CC) main.c -o bin/main $(CFLAGS)

run: build
	@./bin/main