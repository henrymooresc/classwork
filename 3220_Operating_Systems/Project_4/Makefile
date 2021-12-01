CC=gcc
CFLAGS=-Wall -g 

OBJ= filerecovery test

filerecovery: filerecovery.c
	$(CC) $(CFLAGS) filerecovery.c -o filerecovery 

test: test.c
	$(CC) $(CFLAGS) test.c -o test

clean:
	rm -rf  $(BINS) *.o $(objects)