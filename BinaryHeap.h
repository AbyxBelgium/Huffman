/**
 * BinaryHeap is an implementation of a PriorityQueue. This implementation has the following
 * complexities (n is the number of elements in the BinaryHeap):
 * -> Insertion: O(log(n))
 * -> Deletion: O(log(n))
 * -> Retrieval of smallest element: O(1)
 *
 * The BinaryHeap's size will be managed automatically (it grows automatically).
 *
 * @author Pieter Verschaffelt
 */
#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "Huffman.h"

#define INITIAL_ARRAY_SIZE 7

typedef struct Heap {
	unsigned int size;
	unsigned int length;
	HuffmanNode** elements;
} Heap;


/**
 * Initialize a new empty heap.
 */
Heap* heap_init();

/**
 * Add a node to the heap in a structured way (meaning that the
 * node will be added to the heap without breaking it's structure)
 */
void heap_add(Heap* heap, HuffmanNode* element);

/**
 * Returns the node with the lowest frequency from the heap.
 */
HuffmanNode* heap_pop_smallest(Heap* heap);

/**
 * Returns the amount of nodes that are currently contained in the given heap.
 */
unsigned int heap_length(Heap* heap);

/**
 * Free all memory that's been taken by this heap. This will free the
 * memory taken by the array of pointers to enclosed Nodes, but not
 * the memory of the Nodes itself.
 */
void heap_free(Heap* heap);

#endif
