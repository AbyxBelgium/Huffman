#include "HuffmanCompression.h"
#include "BlockReader.h"
#include "BlockWriter.h"
#include "BitStream.h"
#include "Huffman.h"

#include <stdlib.h>

void huffman_encode_file(char* inputFile, char* outputFile, unsigned int blockSize, unsigned long long* fileSize) {
	BlockReader* reader = blockreader_init(inputFile, 1024 * 1024 * blockSize);
	BlockWriter* writer = blockwriter_init(outputFile);

	while (blockreader_has_next(reader)) {
		double progress = (double) (blockreader_current_block(reader) + 1) / (double) blockreader_total_blocks(reader);
		printf("\rHuffman encoding file: %2.f%% done", progress * 100);
		// Force flush to print progress (Linux only flushes after newline by default)
		fflush(stdout);
		unsigned int blockLength;
		unsigned char* block = blockreader_read_block(reader, &blockLength);
		BitStream* encodedStream = huffman_encode(block, blockLength);
		unsigned int encodedLength = bitstream_length(encodedStream);
		unsigned char* encodedData = bitstream_convert_to_array(encodedStream);
		blockwriter_append(writer, encodedData, encodedLength);
		free(encodedData);
		free(block);
	}

	printf("\n");

	*fileSize = blockreader_filesize(reader);
	blockreader_free(reader);
	blockwriter_free(writer);
}

unsigned int huffman_block_length(BlockReader* reader, long startPos) {
	// Read first bytes of file to get first block's size
	size_t temp = 0;
	unsigned char* data = blockreader_read_raw_block(reader, startPos, 10, &temp);
	unsigned int length = huffman_get_block_length(data);
	free(data);
	return length;
}

void huffman_decode_file(char* inputFile, char* outputFile, unsigned long long* fileSize) {
	// Size of the blocks doesn't really matter here, so we use 0
	BlockReader* reader = blockreader_init(inputFile, 0);
	BlockWriter* writer = blockwriter_init(outputFile);

	size_t temp = 0;
	unsigned long long pos = 0;
	unsigned int length = huffman_block_length(reader, pos);
	// Now that we have the first block's size, we can go on and load the whole Huffman block in memory
	while (pos + length <= blockreader_filesize(reader)) {
		unsigned char* data = blockreader_read_raw_block(reader, pos, length, &temp);
		pos = pos + length;
		double progress = (double) pos / (double) blockreader_filesize(reader);
		printf("\rHuffman decoding file: %2.f%% done", progress * 100);
		// Flush stdout to print progress (because no newline is present)
		fflush(stdout);
		BitStream* decoded = huffman_decode(data, length);
		free(data);
		unsigned int decodedLength = bitstream_length(decoded);
		unsigned char* decodedData = bitstream_convert_to_array(decoded);
		blockwriter_append(writer, decodedData, decodedLength);
		free(decodedData);
		length = huffman_block_length(reader, pos);
	}

	printf("\n");

	*fileSize = blockwriter_filesize(writer);
	blockreader_free(reader);
	blockwriter_free(writer);
}
