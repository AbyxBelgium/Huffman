/**
 * The ArrayList datastructure is a dynamically managed list (it grows automatically)/
 *
 * @author Pieter Verschaffelt
 */
#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#define INITIAL_LIST_SIZE 5;

#include <stdbool.h>

typedef struct ArrayList {
	// The length of the enclosed array
	unsigned int size;
	// The amount of items that are filled in the array
	unsigned int length;
	// The array that contains all data
	unsigned long long* elements;
} ArrayList;

/**
 * Create a new ArrayList.
 */
ArrayList* list_init();

/**
 * Add a new element to a list.
 */
void list_add(ArrayList* list, unsigned long long element);

/**
 * Overwrite a value at a certain position in the list.
 */
void list_set(ArrayList* list, unsigned long long element, unsigned int position);

/**
 * Add a new element to a list at a given position.
 */
void list_add_position(ArrayList* list, unsigned long long element, unsigned int position);

/**
 * Get the element at the given position from the list.
 */
unsigned long long list_get(ArrayList* list, unsigned int position);

/**
 * Returns true when the given list contains the given element.
 */
bool list_has(ArrayList* list, unsigned long long element);

/**
 * Free all memory that's in use by the given list.
 */
void list_free(ArrayList* list);

/**
 * Returns the amount of elements that are contained in this list.
 */
unsigned int list_length(ArrayList* list);

/**
 * Converts the given list to an array and frees up all space used by
 * the list-struct. The size of the returned array is equal to the length
 * returned by list_length() that's called just before this function.
 */
unsigned long long* list_to_array(ArrayList* list);

#endif