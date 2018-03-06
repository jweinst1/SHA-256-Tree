CC = gcc
C_FILE = shatree.c
TARGET = shatree

cli:
	mkdir -p bin
	gcc $(C_FILE) -o bin/$(TARGET)
