#include "MemoryAllocator.h"
#include "Error.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * This function checks if the given pointer points to a valid memory block. If this is
 * not the case, the program will be exited and an out of memory message will be shown.
 */
void check_mem(void* memory) {
	if (!memory) {
		handle_error(error_init("Unable to allocate memory!", OUT_OF_MEMORY_ERROR));
	}
}

void* mem_alloc(size_t size) {
	void* allocated = malloc(size);
	check_mem(allocated);
	return allocated;
}

void* mem_realloc(void* original, size_t size) {
	void* output = realloc(original, size);
	check_mem(output);
	return output;
}

void* mem_calloc(size_t elements, size_t elementSize) {
	void* output = calloc(elements, elementSize);
	check_mem(output);
	return output;
}