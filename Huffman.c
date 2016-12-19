#include "Huffman.h"
#include "MemoryAllocator.h"
#include "BinaryHeap.h"

#include <stdlib.h>
#include <string.h>

HuffmanNode* huffman_node_init(unsigned char value, unsigned int frequency) {
	HuffmanNode* node = (HuffmanNode*) mem_alloc(sizeof(HuffmanNode));
	node->leftChild = NULL;
	node->rightChild = NULL;
	node->value = value;
	node->frequency = frequency;
	node->depth = 0;
	return node;
}

/**
 * Free all memory allocated by this node and all of it's children.
 */
void free_node(HuffmanNode* node) {
	if (node->leftChild != NULL) {
		free_node(node->leftChild);
	}

	if (node->rightChild != NULL) {
		free_node(node->rightChild);
	}

	free(node);
}

Heap* get_data_statistics(unsigned char* data, unsigned int length) {
	// We are simply going to use the byte's value as the array-index! (This way we can sort
	// of emulate a very simple HashMap)
	unsigned int* freq = (unsigned int*) mem_calloc(256, sizeof(unsigned int));

	for (unsigned int i = 0; i < length; i++) {
		unsigned char current_value = data[i];
		freq[current_value]++;
	}

	Heap* output = heap_init();
	// Filter non-empty byte-values from the freq-array and add them to the heap
	for (int i = 0; i < 256; i++) {
		// Only add non-empty frequencies
		if (freq[i] != 0) {
			HuffmanNode* toAdd = huffman_node_init(i, freq[i]);
			heap_add(output, toAdd);
		}
	}

	free(freq);
	return output;
}

void increase_node_depth(HuffmanNode* node) {
	node->depth++;

	if (node->leftChild != NULL) {
		increase_node_depth(node->leftChild);
	}

	if (node->rightChild != NULL) {
		increase_node_depth(node->rightChild);
	}
}

/**
 * Build a Huffman tree from the given raw data. Returns the root node of the created
 * Huffman tree. Returns NULL when the given data is empty.
 */
HuffmanNode* huffman_build_tree(unsigned char* data, unsigned int length) {
	Heap* stats = get_data_statistics(data, length);
	HuffmanNode* parent = NULL;
	if (heap_length(stats) == 1) {
		HuffmanNode* node = heap_pop_smallest(stats);
		parent = huffman_node_init(node->value, node->frequency);
		increase_node_depth(parent);
	}
	while (heap_length(stats) > 1) {
		HuffmanNode* first = heap_pop_smallest(stats);
		HuffmanNode* second = heap_pop_smallest(stats);
		parent = huffman_node_init(0, first->frequency + second->frequency);
		increase_node_depth(first);
		parent->leftChild = first;
		parent->rightChild = second;
		increase_node_depth(second);
		heap_add(stats, parent);
	}
	heap_free(stats);
	return parent;
}

void traverse_in_order(HuffmanNode* root, HuffmanNode** encoded) {
	if (root->leftChild == NULL && root->rightChild == NULL) {
		// Root is a leaf
		encoded[root->value] = root;
	} else {
		if (root->leftChild != NULL) {
			memcpy(root->leftChild->code, root->code, CODE_LENGTH);
			root->leftChild->code[root->leftChild->depth - 1] = false;
			traverse_in_order(root->leftChild, encoded);
		}

		if (root->rightChild != NULL) {
			memcpy(root->rightChild->code, root->code, CODE_LENGTH);
			root->rightChild->code[root->rightChild->depth - 1] = true;
			traverse_in_order(root->rightChild, encoded);
		}
	}
}

HuffmanNode** huffman_encode_tree(HuffmanNode* root) {
	HuffmanNode** encoded = (HuffmanNode**) mem_calloc(ARRAY_LENGTH, sizeof(HuffmanNode*));
	traverse_in_order(root, encoded);
	return encoded;
}

void huffman_write_code(bool* code, unsigned int length, BitStream* stream) {
	for (unsigned char i = 0; i < length; i++) {
		bitstream_write_bit(stream, code[i]);
	}
}

/**
 * Returns a BitStream that contains the Huffman-header. That is a part that contains
 * the Huffman codes and there translation. The first parameter for this function is an
 * array that contains all HuffmanNode's that have a specific value. The second parameter
 * defines this array of codes length.
 *
 * The Huffman header has the following format:
 *  -> First 2 bytes define the total header length in bytes
 *  -> Next 4 bytes define the total amount of encoded bytes (thus encoded block length)
 *	-> Next 4 bytes define the total bytes encoded
 *  -> After length one byte defines the value
 *  -> After value one byte defines the following code length in bits
 *  -> After value the code itself appears (with trailing zeroes to make it's length a multiple of 8)
 *  -> Some trailing zeroes can be present, but these are counted in the total header length
 */
void huffman_get_header(HuffmanNode** codes, unsigned int length, BitStream* stream) {
	// Reserve 6 bytes at the start of the bitstream for defining the header length
	// and the total amount of encoded words later on.
	for (char i = 0; i < 10; i++) {
		bitstream_write_byte(stream, 0);
	}

	for (unsigned int i = 0; i < length; i++) {
		if (codes[i] != NULL) {
			bitstream_write_byte(stream, codes[i]->value);
			bitstream_write_byte(stream, codes[i]->depth);
			huffman_write_code(codes[i]->code, codes[i]->depth, stream);
			// Fill code to byte
			if (codes[i]->depth % 8 != 0) {
				bitstream_flush_force(stream);
			}
		}
	}
	// Write header length to header (in bytes we reserved at start)
	unsigned short headerLength = bitstream_length(stream);
	memcpy(stream->data, &headerLength, sizeof(unsigned short));
}

/**
 * This function writes a certain length to a BitStream. It's used for writing a code's
 * length to the Huffman-header.
 */
void huffman_write_length(unsigned int length, BitStream* stream) {
	for (int i = 0; i < 8; i++) {
		bitstream_write_bit(stream, length & (1 << i));
	}
}

BitStream* huffman_encode(unsigned char* data, unsigned int length) {
	HuffmanNode* root = huffman_build_tree(data, length);
	HuffmanNode** codes = huffman_encode_tree(root);
	BitStream* stream = bitstream_init();
	huffman_get_header(codes, ARRAY_LENGTH, stream);
	for (unsigned int i = 0; i < length; i++) {
		huffman_write_code(codes[data[i]]->code, codes[data[i]]->depth, stream);
	}
	bitstream_flush(stream);
	// Write length of encoded block to header
	unsigned int streamLength = bitstream_length(stream);
	memcpy(&(stream->data[2]), &streamLength, sizeof(unsigned int));
	memcpy(&(stream->data[6]), &length, sizeof(unsigned int));
	free_node(root);
	free(codes);
	return stream;
}

/**
 * Returns the root HuffmanNode that from the tree that contains all Huffman-codes.
 */
HuffmanNode* huffman_read_header(BitStream* stream, unsigned short* headerLength, unsigned int* totalBytes, unsigned int* encodedBytes) {
	memcpy(headerLength, stream->data, sizeof(unsigned short));
	memcpy(totalBytes, &(stream->data[2]), sizeof(unsigned int));
	memcpy(encodedBytes, &(stream->data[6]), sizeof(unsigned int));

	// Done starts from the real header (skipping all the bytes that define block length)
	unsigned int done = 10;

	HuffmanNode* root = huffman_node_init(0, 0);
	HuffmanNode* top = root;
	if (done + 3 >= *headerLength) {
		root->value = stream->data[done];
		done++;
		root->depth = stream->data[done];
		done++;
	} else {
		while (done < *headerLength) {
			unsigned char value = stream->data[done];
			done++;
			// Length of the code in bits
			unsigned char length = stream->data[done];
			done++;
			for (unsigned char i = 0; i < length; i++) {
				if (bitstream_read_bit_2(stream, done * 8 + i) == 0) {
					if (top->leftChild == NULL) {
						top->leftChild = huffman_node_init(0, 0);
					}
					top = top->leftChild;
				} else {
					if (top->rightChild == NULL) {
						top->rightChild = huffman_node_init(0, 0);
					}
					top = top->rightChild;
				}
			}
			// Get nearest multiple of 8 for length
			unsigned int inc = (length + 7) & ~7;
			done += inc / 8;
			top->value = value;
			top->depth = length;
			top = root;
		}
	}
	return root;
}

BitStream* huffman_decode(unsigned char* data, unsigned int length) {
	BitStream* stream = bitstream_init_from_data(data, length, length);
	BitStream* output = bitstream_init();
	unsigned short headerLength = 0;
	unsigned int blockLength = 0;
	unsigned int encodedBytes = 0;
	HuffmanNode* root = huffman_read_header(stream, &headerLength, &blockLength, &encodedBytes);
	unsigned int done = 0;
	HuffmanNode* top = root;
	for (unsigned long long i = 8 * headerLength; done < encodedBytes && i < 8 * (unsigned long long) blockLength; i++) {
		if (bitstream_read_bit_2(stream, i) == 0) {
			if (top->leftChild != NULL) {
				top = top->leftChild;
			}
		} else {
			if (top->rightChild != NULL) {
				top = top->rightChild;
			}
		}
		if (top->leftChild == NULL && top->rightChild == NULL) {
			// TODO: implement bytestream that can write full bytes more efficiently
			bitstream_write_byte(output, top->value);
			top = root;
			done++;
		}
	}
	free_node(root);
	bitstream_free_leave_data(stream);
	return output;
}

unsigned int huffman_get_block_length(unsigned char* data) {
	unsigned int output = 0;
	memcpy(&output, &data[2], 4);
	return output;
}