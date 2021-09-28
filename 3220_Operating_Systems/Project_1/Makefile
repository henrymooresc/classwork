CC=gcc
CFLAGS=-Wall -g

BINS=leakcount memory_shim.so sctracer 

all: $(BINS)

leakcount: leakcount.c
	$(CC) $(CFLAGS) -o leakcount leakcount.c

memory_shim.so: memory_shim.c
	$(CC) $(CFLAGS) -shared -fPIC memory_shim.c -o memory_shim.so -ldl

sctracer: sctracer.c
	$(CC) $(CFLAGS) -o sctracer sctracer.c

clean:
	rm -rf $(BINS) *.o *.dSYM *.txt