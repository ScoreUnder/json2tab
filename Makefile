CC=gcc
CFLAGS=-s -Os -std=c11 -Wall -Wextra -pedantic

junescape: junescape.c
	$(CC) -o $@ $^ $(CFLAGS)
