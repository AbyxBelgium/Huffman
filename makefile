# We're using the GCC-compiler
CC=gcc
# These are all the compiler-flags we're going to use
CFLAGS=-std=c99 -O3 -flto -lm -Wall -pthread
# Base files (present in all compilations)
BASE=ArrayList.c ArgumentChecker.c BinaryHeap.c BitStream.c BlockReader.c BlockWriter.c Error.c Huffman.c HuffmanCompression.c MemoryAllocator.c

main: $(BASE) Main.c
	$(CC) $(CFLAGS) -o Huffman.out $(BASE) Main.c

library: $(BASE) Main.c
	$(CC) $(CFLAGS) -shared -o huffman.dll $(BASE) Main.c

debug: $(BASE) Main.c
	$(CC) -std=c99 -pthread -g -o Debug.out $(BASE) Main.c
