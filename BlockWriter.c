#include "BlockWriter.h"
#include "MemoryAllocator.h"
#include "Error.h"

// These defines are needed by GCC to use fseek64 and stat64
#define __USE_LARGEFILE64
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <pthread.h>

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

BlockWriter* blockwriter_init(char* fileName) {
	BlockWriter* writer = (BlockWriter*) mem_alloc(sizeof(BlockWriter));
	writer->fileName = fileName;
	// Create the file if it doesn't exist or erase it if it allready exists
	FILE* file = fopen(fileName, "wb");

	if (!file) {
		handle_error(error_init("Couldn't open file for saving... Maybe you have no write permissions? \n", FILE_NOT_CREATED_ERROR));
	}

	fclose(file);
	return writer;
}

void blockwriter_append(BlockWriter* writer, unsigned char* data, unsigned int size) {
	FILE* file = fopen(writer->fileName, "ab");
	fwrite(data, sizeof(unsigned char), size, file);
	fclose(file);
}

void blockwriter_write_raw(BlockWriter* writer, unsigned char* data, unsigned int size, unsigned long long position) {
	FILE* file = fopen(writer->fileName, "rb+");
	FSEEK(file, position, SEEK_SET);
	fwrite(data, sizeof(unsigned char), size, file);
	fclose(file);
}

unsigned long long blockwriter_filesize(BlockWriter* writer) {
	struct STAT buffer;
	STAT(writer->fileName, &buffer);
	unsigned long long size = (unsigned long long) buffer.st_size;
	return size;
}

void blockwriter_free(BlockWriter* writer) {
	free(writer);
}