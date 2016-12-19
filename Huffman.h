/**
 * This file contains all functions that are associated with the Huffman-algorithm.
 * A chunk of data can be encoded or decoded with the Huffman encoding using this function.
 *
 * @author Pieter Verschaffelt
 */
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "BitStream.h"
#include "BlockReader.h"

#define ARRAY_LENGTH 256
#define CODE_LENGTH 256

typedef struct HuffmanNode {
	struct HuffmanNode* leftChild;
	struct HuffmanNode* rightChild;
	unsigned char value;
	unsigned int frequency;
	unsigned int depth;
	bool code[CODE_LENGTH];
} HuffmanNode;

/**
 * Encode data from the given array using the Huffman encoding-algorithm. This function
 * returns an array containing the encoded data.
 */
BitStream* huffman_encode(unsigned char* data, unsigned int length);

/**
 * Decide data from the given array using the Huffman decoding-algorithm. This function
 * returns an array containing the decoded data.
 */
BitStream* huffman_decode(unsigned char* data, unsigned int length);

/**
 * Returns the size of the chunck of Huffman-data pointed to by data.
 */
unsigned int huffman_get_block_length(unsigned char* data);

#endif