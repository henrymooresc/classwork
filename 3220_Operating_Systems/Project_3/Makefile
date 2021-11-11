CC=gcc
CFLAGS=-g -Wall

BINS=allocator simple

all: $(BINS)

allocator: allocator.c
	$(CC) $(CFLAGS) -fPIC -shared allocator.c -o libmyalloc.so

allocator2: allocator.c
	$(CC) $(CFLAGS) allocator.c -o all

test: test.c
	$(CC) $(CFLAGS) test.c -o test

simple: simple_test.c
	$(CC) $(CFLAGS) simple_test.c -o simple