/**
 * A BlockReader can be used to read a specific file in, in blocks of a fixed size.
 *
 * @author Pieter Verschaffelt
 */
#ifndef BLOCK_READER_H
#define BLOCK_READER_H

#include <stdio.h>
#include <stdbool.h>

typedef struct BlockReader {
	char* fileName;
	// Total filesize of the file with name fileName in bytes.
	unsigned long long fileSize;
	// Size of one block in bytes.
	unsigned int blockSize;
	// Defines which block was last read.
	unsigned int block;
	// Defines the total amount of blocks
	unsigned int blocks;
} BlockReader;

/**
 * Create a new BlockReader that reads a file with fileName and uses blocks of
 * size blockSize (in bytes).
 */
BlockReader* blockreader_init(char* fileName, unsigned int blockSize);

/**
 * Returns true when yet another block of data could be read from the given BlockReader.
 */
bool blockreader_has_next(BlockReader* reader);

/**
 * Returns the block of the file as a chunck of data. The second parameter blockSize will be
 * filled in by this function and defines the total size of the just read block (in bytes).
 */
unsigned char* blockreader_read_block(BlockReader* reader, unsigned int* blockSize);

/**
 * Read a block that starts and ends at a specific byte position and has a specific length. The readLength
 * parameter defines the actual amount of bytes read.
 */
unsigned char* blockreader_read_raw_block(BlockReader* reader, long startPos, unsigned int length, size_t* readLength);

/**
 * Free all memory taken by the given BlockReader.
 */
void blockreader_free(BlockReader* reader);

/**
 * Returns the total size of the file pointed to by the given BlockReader in bytes.
 */
unsigned long long blockreader_filesize(BlockReader* reader);

/**
 * Returns the number of the block that was last read from the given BlockReader.
 */
unsigned int blockreader_current_block(BlockReader* reader);

/**
 * Returns the total amount of blocks that can be read from this BlockReader.
 */
unsigned int blockreader_total_blocks(BlockReader* reader);
#endif
