# We're using the GCC-compiler
CC=gcc
# These are all the compiler-flags we're going to use
CFLAGS=-std=c99 -O3 -flto -lm -Wall
# Base files (present in all compilations)
BASE=ArrayList.c ArgumentChecker.c BinaryHeap.c BitStream.c BlockReader.c BlockWriter.c Error.c Huffman.c HuffmanCompression.c MemoryAllocator.c

compression1: $(BASE) Main.c
	$(CC) $(CFLAGS) -o compression1.out $(BASE) Main.c