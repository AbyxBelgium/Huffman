/**
 * A BitStream is a stream that supports the reading and writing of individual bits. This datastructure's
 * size will be managed dynamically.
 *
 * @author Pieter Verschaffelt
 */
#ifndef BIT_STREAM_H
#define BIT_STREAM_H

#define INITIAL_SIZE 97;

#include <stdbool.h>

typedef struct BitStream {
	unsigned char* data;
	unsigned int size;
	unsigned int currentByte;
	unsigned char buffer;
	unsigned char bitsInBuffer;
} BitStream;

/**
 * Initialize a new BitStream from some datasource. Length defines the amount of elements in the
 * given array and size defines the total length of the array (the number of elements that can fit in
 * the array)
 */
BitStream* bitstream_init_from_data(unsigned char* data, unsigned int length, unsigned int size);

/**
 * Initialize a new empty BitStream.
 */
BitStream* bitstream_init();

/**
 * Write one byte to the bitstream. Note that no flushing is performed before writing this byte,
 * so when the contents of the bit-buffer should precede this byte, then flushing should be done
 * manually.
 */
void bitstream_write_byte(BitStream* stream, const unsigned char byte);

/**
 * Write one bit to the bitstream. A one will be written if "bit" is true, a zero otherwise.
 */
void bitstream_write_bit(BitStream* stream, const bool bit);

/**
 * Read the value of the bit at bitPos in the byte with number bytePos from the given BitStream.
 * Note bitPos should be smaller than 8!
 */
unsigned char bitstream_read_bit(BitStream* stream, const unsigned int bytePos, const unsigned char bitPos);

/**
 * Read the value of the bit at bitPos in the BitStream. This function accepts a bitPos that's larger
 * than 8.
 */
unsigned char bitstream_read_bit_2(BitStream* stream, const unsigned long long bitPos);

/**
 * Flush the bitbuffer. This operation writes data to the array that's still in the buffer and
 * clears the buffer afterwards.
 */
void bitstream_flush(BitStream* stream);

/**
 * Force flush the bitbuffer. This function will also flush the buffer even if it's value is 0.
 * The buffer will be cleared afterwards.
 */
void bitstream_flush_force(BitStream* stream);

/**
 * This function converts the given BitStream to an array. This function automatically
 * frees all memory taken by the BitStream. DO NOT call bitstream_free on the same BitStream
 * after this function!
 */
unsigned char* bitstream_convert_to_array(BitStream* stream);

/**
 * Returns the byte at the given position from the given BitStream. Position must be smaller than
 * the length of the stream!
 */
unsigned char bitstream_get(BitStream* stream, const unsigned int position);

/**
 * Free all memory taken by this BitStream.
 */
void bitstream_free(BitStream* stream);

/**
 * Free the BitStream struct, but do not free memory taken by the internal
 * char* array. This function can be used to free a BitStream that was created
 * from an existing array and this array should not be cleared.
 */
void bitstream_free_leave_data(BitStream* stream);

/**
 * Returns the amount of bytes that are put in this BitStream.
 */
unsigned int bitstream_length(BitStream* stream);

/**
 * Copy the value of the original BitStream to the other BitStream.
 */
void bitstream_set_value(BitStream* original, BitStream* other);

#endif