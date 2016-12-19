#include "BinaryHeap.h"
#include "MemoryAllocator.h"

#include <stdlib.h>

Heap* heap_init() {
	Heap* output = (Heap*) mem_alloc(sizeof(Heap));
	// First element is placed at index 1 (easier implementation)
	output->length = 1;
	output->size = INITIAL_ARRAY_SIZE;
	output->elements = (HuffmanNode**) mem_alloc(sizeof(HuffmanNode*) * INITIAL_ARRAY_SIZE);
	return output;
}

/**
 * Doubles the amount of nodes that could be stored in the given heap.
 */
void increase_heap_elements(Heap* heap) {
	heap->size = heap->size * 2;
	heap->elements = mem_realloc(heap->elements, sizeof(HuffmanNode*) * heap->size);
}

/**
 * Apply the fixup algorithm to a heap who just had a new element
 * inserted at it's last elements-position.
 */
void fixup(Heap* heap) {
	// Element to add is situated at last position in the array
	int i = heap->length - 1;
	while (i > 1 && heap->elements[i / 2]->frequency > heap->elements[i]->frequency) {
		HuffmanNode* temp = heap->elements[i / 2];
		heap->elements[i / 2] = heap->elements[i];
		heap->elements[i] = temp;
		i = i / 2;
	}
}

void fixdown(Heap* heap, unsigned int i) {
	unsigned int leftChildIndex = 2 * i;
	unsigned int rightChildIndex = 2 * i + 1;
	int smallest;
	if (leftChildIndex < heap->length && heap->elements[leftChildIndex]->frequency < heap->elements[i]->frequency) {
		smallest = leftChildIndex;
	} else {
		smallest = i;
	}

	if (rightChildIndex < heap->length && heap->elements[rightChildIndex]->frequency < heap->elements[smallest]->frequency) {
		smallest = rightChildIndex;
	}

	if (smallest != i) {
		// Swap current Node with it's smallest child (if that one's smaller than the current top)
		HuffmanNode* temp = heap->elements[smallest];
		heap->elements[smallest] = heap->elements[i];
		heap->elements[i] = temp;
		fixdown(heap, smallest);
	}
}

void heap_add(Heap* heap, HuffmanNode* element) {
	// Elements of the heap stored in the array start at index
	// 1 (this makes implementation much easier)
	if (heap->length == heap->size) {
		increase_heap_elements(heap);
	}
	heap->elements[heap->length] = element;
	heap->length++;
	fixup(heap);
}

HuffmanNode* heap_pop_smallest(Heap* heap) {
	HuffmanNode* output = heap->elements[1];
	heap->length--;
	// Place last element in first position
	heap->elements[1] = heap->elements[heap->length];
	// Fix heap ordering by using the fixdown-algorithm
	fixdown(heap, 1);
	return output;
}

unsigned int heap_length(Heap* heap) {
	return heap->length - 1;
}

void heap_free(Heap* heap) {
	free(heap->elements);
	free(heap);
}