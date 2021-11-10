CC=gcc
CFLAGS=-g -Wall

BINS=allocator

all: $(BINS)

allocator: allocator.c
	$(CC) $(CFLAGS) -fPIC -shared allocator.c -o libmyalloc.so