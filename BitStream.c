#include "BitStream.h"
#include "MemoryAllocator.h"

#include <stdlib.h>
#include <string.h>

BitStream* bitstream_init_from_data(unsigned char* data, unsigned int length, unsigned int size) {
	BitStream* stream = (BitStream*) mem_alloc(sizeof(BitStream));
	stream->data = data;
	stream->currentByte = length;
	stream->size = size;
	stream->buffer = 0;
	stream->bitsInBuffer = 0;
	return stream;
}

BitStream* bitstream_init() {
	BitStream* stream = (BitStream*) mem_alloc(sizeof(BitStream));
	stream->size = INITIAL_SIZE;
	stream->data = (unsigned char*) mem_calloc(stream->size, sizeof(unsigned char));
	stream->currentByte = 0;
	stream->bitsInBuffer = 0;
	stream->buffer = 0;
	return stream;
}

void bitstream_write_byte(BitStream* stream, const unsigned char byte) {
	if (stream->size == stream->currentByte) {
		// Increase data-size
		stream->size = stream->size * 2;
		stream->data = mem_realloc(stream->data, sizeof(unsigned char) * stream->size);
	}
	stream->data[stream->currentByte] = byte;
	stream->currentByte++;
}

void bitstream_write_buffer(BitStream* stream) {
	bitstream_write_byte(stream, stream->buffer);
	// Clear buffer
	stream->buffer = 0;
	stream->bitsInBuffer = 0;
}

void bitstream_write_bit(BitStream* stream, const bool bit) {
	stream->buffer ^= (-bit ^ stream->buffer) & (1 << stream->bitsInBuffer);
	++stream->bitsInBuffer;

	if (stream->bitsInBuffer == 8) {
		// Write byte when 8 bits have been collected.
		bitstream_write_buffer(stream);
	}
}

unsigned char bitstream_read_bit(BitStream* stream, const unsigned int bytePos, const unsigned char bitPos) {
	unsigned int byteVal = stream->data[bytePos];
	return (byteVal >> bitPos) & 1;
}

unsigned char bitstream_read_bit_2(BitStream* stream, const unsigned long long bitPos) {
	unsigned int byte = (unsigned int) (bitPos / 8);
	unsigned char byteVal = stream->data[byte];
	unsigned char mod = bitPos & 7;
	return (byteVal & (1 << mod)) >> mod;
}

void bitstream_flush(BitStream* stream) {
	if (stream->bitsInBuffer != 0) {
		bitstream_flush_force(stream);
	}
}

void bitstream_flush_force(BitStream* stream) {
	for (unsigned char i = stream->bitsInBuffer; i < 8; i++) {
		bitstream_write_bit(stream, 0);
	}
	/*stream->buffer = stream->buffer << (8 - stream->bitsInBuffer);
	bitstream_write_buffer(stream);*/
	stream->bitsInBuffer = 0;
	stream->buffer = 0;
}

unsigned char* bitstream_convert_to_array(BitStream* stream) {
	unsigned char* output = stream->data;
	free(stream);
	return output;
}

unsigned char bitstream_get(BitStream* stream, const unsigned int position) {
	return stream->data[position];
}

void bitstream_free(BitStream* stream) {
	free(stream->data);
	free(stream);
}

void bitstream_free_leave_data(BitStream* stream) {
	free(stream);
}

unsigned int bitstream_length(BitStream* stream) {
	return stream->currentByte;
}

void bitstream_set_value(BitStream* original, BitStream* other) {
	original->currentByte = other->currentByte;
	original->size = other->size;
	original->buffer = other->buffer;
	original->bitsInBuffer = other->bitsInBuffer;
	original->data = mem_realloc(original->data, other->size);
	memcpy(original->data, other->data, other->size);
}