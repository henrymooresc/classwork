CC=gcc
CFLAGS=-Wall -g 
BINS=filerecovery test

filerecovery: filerecovery.c
	$(CC) $(CFLAGS) filerecovery.c -o filerecovery 

test: test.c
	$(CC) $(CFLAGS) test.c -o test

clean:
	rm -rf $(BINS)