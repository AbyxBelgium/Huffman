#include "BlockReader.h"
#include "MemoryAllocator.h"
#include "Error.h"

// These defines are needed by GCC to use fseek64 and stat64
#define __USE_LARGEFILE64
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
//#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include <sys/stat.h>

// Macro defines the correct stat-function for reading large files (2+GiB)
// length. fseek only works until 2GiB filesize
#if _WIN32 || _WIN64
#define STAT _stat64
#elif __GNUC__
#define STAT stat64
#endif

// This macro defines the correct fseek-function for reading large files
#if _WIN32 || _WIN64
#define FSEEK _fseeki64
#elif __GNUC__
#define FSEEK fseek
#endif

unsigned long long get_total_file_size(char* filename) {
	struct STAT buffer;
	STAT(filename, &buffer);
	unsigned long long size = (unsigned long long) buffer.st_size;
	return size;
}

BlockReader* blockreader_init(char* fileName, unsigned int blockSize) {
	BlockReader* output = (BlockReader*) mem_alloc(sizeof(BlockReader));
	output->fileName = fileName;
	output->block = 0;
	output->blockSize = blockSize;
	FILE* file = fopen(fileName, "rb");

	if (!file) {
		handle_error(error_init("The requested file was not found!", FILE_NOT_FOUND_ERROR));
	}

	fclose(file);

	output->fileSize = get_total_file_size(fileName);
	if (output->blockSize != 0) {
		output->blocks = (unsigned int) (output->fileSize / output->blockSize);
	} else {
		output->blocks = 0;
	}

	if (output->blockSize != 0 && output->fileSize % output->blockSize != 0) {
		output->blocks++;
	}
	return output;
}

bool blockreader_has_next(BlockReader* reader) {
	return reader->block != reader->blocks;
}

unsigned char* blockreader_read_block(BlockReader* reader, unsigned int* blocksize) {
	FILE* file = fopen(reader->fileName, "rb");
	
	unsigned int size;
	if (reader->block == reader->blocks - 1 && reader->fileSize % reader->blockSize != 0) {
		size = reader->fileSize % reader->blockSize;
	} else {
		size = reader->blockSize;
	}

	*blocksize = size;

	unsigned char* buffer = (unsigned char*) mem_alloc(sizeof(unsigned char) * (size));

	FSEEK(file, reader->blockSize * reader->block, 0);
	size_t bytesRead = fread(buffer, sizeof(unsigned char), size, file);
	fclose(file);
	reader->block++;
	return buffer;
}

unsigned char* blockreader_read_raw_block(BlockReader* reader, long startPos, unsigned int length, size_t* readLength) {
	FILE* file = fopen(reader->fileName, "rb");

	unsigned char* buffer = (unsigned char*) mem_alloc(sizeof(unsigned char) * (length + 1));

	FSEEK(file, startPos, 0);
	*readLength = fread(buffer, sizeof(unsigned char), length, file);
	fclose(file);

	// Terminate buffer
	buffer[length] = '\0';
	return buffer;
}

void blockreader_free(BlockReader* reader) {
	free(reader);
}

unsigned long long blockreader_filesize(BlockReader* reader) {
	return reader->fileSize;
}

unsigned int blockreader_current_block(BlockReader* reader) {
	return reader->block;
}

unsigned int blockreader_total_blocks(BlockReader* reader) {
	return reader->blocks;
}