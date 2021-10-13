CC=gcc
CFLAGS=-Wall -c
BINS=threadslib libmythreads

all: $(BINS)

threadslib: threads_main.c threads_data.c
	$(CC) $(CFLAGS) threads_main.c threads_data.c

libmythreads:
	rm -rf *.a
	ar -cvrs libmythreads.a threads_main.o threads_data.o
	rm -rf *.o

clean:
	rm *.a