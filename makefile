all:run

BIN_DIR=bin
EXEC_NAME=Pacman
EXEC=$(BIN_DIR)/$(EXEC_NAME)

CC=g++
CFLAGS=-Wall -Wextra -pedantic -std=c++23 -g
LDFLAGS=-L/usr/local/lib -lraylib

SRC_DIR=src
LANG=cpp
SRC=$(wildcard $(SRC_DIR)/*.$(LANG))

bin_dir:
	mkdir -p $(BIN_DIR)

build:$(SRC) bin_dir
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LDFLAGS)

run:build
	./$(EXEC)
