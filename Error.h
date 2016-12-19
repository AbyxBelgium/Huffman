/**
 * This file contains all error-handling functions.
 *
 * @author Pieter Verschaffelt
 */
#ifndef ERROR_H
#define ERROR_H

#include "MemoryAllocator.h"

#define OUT_OF_MEMORY_ERROR 1
#define OUT_OF_BOUNDS_ERROR 2
#define FILE_NOT_FOUND_ERROR 3
#define FILE_NOT_CREATED_ERROR 4

typedef struct Error {
	char* message;
	int errorCode;
} Error;

/**
 * Initialize a new Error. The first argument message must be a string that's allocated on the stack!
 */
Error* error_init(char* message, int errorCode);

void error_free(Error* error);

/**
 * This function prints the error message corresponding with the given error code
 * and terminates the program with the appropriate error code.
 */
void handle_error(Error* error);

#endif