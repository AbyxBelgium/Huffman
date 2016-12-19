/**
 * The BlockWriter can be used to write specific chuncks of data to a file.
 * When this file allready contains data, the new data will be appended.
 *
 * @author Pieter Verschaffelt
 */
#ifndef BLOCK_WRITER_H
#define BLOCK_WRITER_H

typedef struct BlockWriter {
	char* fileName;
} BlockWriter;

BlockWriter* blockwriter_init(char* fileName);

void blockwriter_append(BlockWriter* writer, unsigned char* data, unsigned int size);

/**
 * (Over)write a specific part of the file pointed by given blockwriter.
 */
void blockwriter_write_raw(BlockWriter* writer, unsigned char* data, unsigned int size, unsigned long long position);

unsigned long long blockwriter_filesize(BlockWriter* writer);

void blockwriter_free(BlockWriter* writer);

#endif
