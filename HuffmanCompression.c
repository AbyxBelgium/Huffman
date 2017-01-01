#include "HuffmanCompression.h"
#include "BlockReader.h"
#include "BlockWriter.h"
#include "BitStream.h"
#include "Huffman.h"
#include "MemoryAllocator.h"

#include <stdlib.h>
#include <pthread.h>

typedef struct ParallelInput {
	unsigned char* inputBlock;
	unsigned int blockLength;
	pthread_mutex_t* mutex;
	unsigned char threadNum;
} ParallelInput;

BitStream** thread_output;
unsigned char* threads_done;
pthread_cond_t condition;

void* huffman_process_parallel(void* args) {
	ParallelInput* input = (ParallelInput*) args;
	BitStream* encodedStream = huffman_encode(input->inputBlock, input->blockLength);
	thread_output[input->threadNum] = encodedStream;
	free(input->inputBlock);
	pthread_mutex_lock(input->mutex);
	threads_done[input->threadNum] = 1;
	pthread_cond_broadcast(&condition);
	pthread_mutex_unlock(input->mutex);
	return NULL;
}

void huffman_encode_file(char* inputFile, char* outputFile, unsigned int blockSize, unsigned long long* fileSize, unsigned char threads) {
	BlockReader* reader = blockreader_init(inputFile, 1024 * 1024 * blockSize);
	BlockWriter* writer = blockwriter_init(outputFile);

	thread_output = (BitStream**) mem_alloc(sizeof(BitStream*) * threads);

	// Create a thread condition variable per thread to synchronize
	threads_done = (unsigned char*) mem_alloc(sizeof(unsigned char) * threads);
	pthread_mutex_t* thread_mutex = (pthread_mutex_t*) mem_alloc(sizeof(pthread_mutex_t) * threads);
	pthread_t* thread_objects = (pthread_t*) mem_alloc(sizeof(pthread_t) * threads);
	ParallelInput* inputs = (ParallelInput*) mem_alloc(sizeof(ParallelInput) * threads);
	pthread_cond_init(&condition, NULL);

	for (unsigned char i = 0; i < threads; i++) {
		threads_done[i] = 0;
		pthread_mutex_init(&thread_mutex[i], NULL);
	}

	printf("Huffman encoding file: 0%% done");
	while (blockreader_has_next(reader)) {
		unsigned char threadsDone = 0;
		// Read blocks of data and start new threads...
		for (unsigned char i = 0; i < threads && blockreader_has_next(reader); i++) {
			unsigned int blockLength;
			unsigned char* block = blockreader_read_block(reader, &blockLength);

			ParallelInput* input = &inputs[i];
			input->inputBlock = block;
			input->blockLength = blockLength;
			input->mutex = &thread_mutex[i];
			input->threadNum = i;

			threads_done[i] = 0;
			// Encoding is performed by separate threads to speed up execution time
			pthread_create(&thread_objects[i], NULL, &huffman_process_parallel, input);

			threadsDone = i;
		}

		// Wait for the threads to finish and write the encoded data to a file...
		for (unsigned char i = 0; i <= threadsDone; i++) {
			pthread_mutex_lock(&thread_mutex[i]);
			while (threads_done[i] == 0) {
				pthread_cond_wait(&condition, &thread_mutex[i]);
			}
			pthread_mutex_unlock(&thread_mutex[i]);

			double progress = (double) blockreader_current_block(reader) / (double) blockreader_total_blocks(reader);
			printf("\rHuffman encoding file: %2.f%% done", progress * 100);
			// Force flush to print progress (Linux only flushes after newline by default)
			fflush(stdout);

			BitStream* encodedStream = thread_output[i];

			unsigned int encodedLength = bitstream_length(encodedStream);
			unsigned char* encodedData = bitstream_convert_to_array(encodedStream);
			blockwriter_append(writer, encodedData, encodedLength);
			free(encodedData);
		}
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
