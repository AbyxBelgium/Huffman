#include "ArrayList.h"
#include "MemoryAllocator.h"
#include "Error.h"

#include <stdlib.h>
#include <string.h>

ArrayList* list_init() {
	ArrayList* list = (ArrayList*) mem_alloc(sizeof(ArrayList));
	list->size = INITIAL_LIST_SIZE;
	list->length = 0;
	list->elements = (unsigned long long*) mem_alloc(sizeof(unsigned long long) * list->size);
	return list;
}

/**
 * This function will double the given list's size so that it can store
 * more data.
 */
void increase_list_size(ArrayList* list) {
	list->size = list->size * 2;
	list->elements = mem_realloc(list->elements, sizeof(unsigned long long) * list->size);
}

void list_add(ArrayList* list, unsigned long long element) {
	// Check if there's still room in the ArrayList and double it's size
	// when this isn't the case.
	if (list->length == list->size) {
		increase_list_size(list);
	}
	list->elements[list->length] = element;
	list->length++;
}

void list_set(ArrayList* list, unsigned long long element, unsigned int position) {
	list->elements[position] = element;
}

void list_add_position(ArrayList* list, unsigned long long element, unsigned int position) {
	if (position > list->length || position < 0) {
		handle_error(error_init("List position must be between 0 (inclusive) and list length (exclusive)!", OUT_OF_BOUNDS_ERROR));
	}

	if (list->length == list->size) {
		increase_list_size(list);
	}

	// Shift all elements by one
	for (unsigned int i = list->length; i > position; i--) {
		list->elements[i - 1] = list->elements[i];
	}
	list->elements[position] = element;
}

unsigned long long list_get(ArrayList* list, unsigned int position) {
	if (position < 0 || position >= list->length) {
		handle_error(error_init("You tried to access a non-exisiting element in an ArrayList!", OUT_OF_BOUNDS_ERROR));
	}
	return list->elements[position];
}

bool list_has(ArrayList* list, unsigned long long element) {
	for (unsigned int i = 0; i < list->length; i++) {
		if (list->elements[i] == element) {
			return true;
		}
	}
	return false;
}

void list_free(ArrayList* list) {
	free(list->elements);
	free(list);
}

unsigned int list_length(ArrayList* list) {
	return list->length;
}

unsigned long long* list_to_array(ArrayList* list) {
	unsigned long long* data= list->elements;
	// Shorten the array to the size that's really being used
	unsigned long long* correct_size = mem_realloc(data, list->length * sizeof(void*));
	// Free the list struct
	free(list);
	return correct_size;
}