/**
 * This file contains functions to dynamically allocate memory on the heap.
 * These functions include automatic error-handling in case of failure and are prefered
 * over the default memory-allocation-functions.
 *
 * @author Pieter Verschaffelt
 */
#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stdio.h>

/**
 * Allocate a new piece of memory that has the given size.
 * This function automatically checks if the allocation succeeded and terminates
 * the program when this isn't the case.
 */
void* mem_alloc(size_t size);

/**
 * Make a piece of memory pointed to by original bigger or smaller.
 * This function automatically checks if the reallocation succeeded and terminates
 * the program when this isn't the case.
 */
void* mem_realloc(void* original, size_t size);

void* mem_calloc(size_t elements, size_t elementSize);

#endif