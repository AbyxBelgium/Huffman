#ifndef HUFFMAN_COMPRESSION_H
#define HUFFMAN_COMPRESSION_H

#include "BlockReader.h"

/**
 * Encode the given inputFile with the Huffman-algorithm and write the result to outputFile. The parameter blockSize
 * defines the length of each block read from the inputFile in MiB. The parameter fileSize will be filled in with the
 * total size in bytes of the input file.
 */
void huffman_encode_file(char* inputFile, char* outputFile, unsigned int blockSize, unsigned long long* fileSize, unsigned char threads);

/**
 * Returns the length of the huffmanblock starting at the given startPos in the given BlockReader in bytes.
 */
unsigned int huffman_block_length(BlockReader* reader, long startPos);

/**
 * Decode the given inputFile with the Huffman-algorithm and write the result to outputFile. The parameter fileSize will
 * be filled in with the total size in bytes of the output file.
 */
void huffman_decode_file(char* inputFile, char* outputFile, unsigned long long* fileSize);

#endif
