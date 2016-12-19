#include "Error.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Error* error_init(char* message, int errorCode) {
	Error* error = (Error*) mem_alloc(sizeof(Error));
	error->message = message;
	error->errorCode = errorCode;
	return error;
}

void error_free(Error* error) {
	free(error);
}

void handle_error(Error* error) {
	int code = error->errorCode;
	if (code == OUT_OF_MEMORY_ERROR) {
		fprintf(stderr, "Out of memory!\n");
	} else if (code == OUT_OF_BOUNDS_ERROR) {
		fprintf(stderr, "Out of bounds! You tried to access a list's element that does not exist!\n");
	} else if (code == FILE_NOT_FOUND_ERROR) {
		fprintf(stderr, "The requested file could not be found...\n");
	} else if (code == FILE_NOT_CREATED_ERROR) {
		fprintf(stderr, "The requested file could not be created...\n");
	} else {
		fprintf(stderr, "An unknown error occurred!\n");
	}
	fprintf(stderr, "%s", error->message);
	fprintf(stderr, "\n");
	printf("Press any key to continue...\n");
	getchar();
	// Clean up memory used by error...
	error_free(error);
	exit(error->errorCode);
}