#include "ArgumentChecker.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void print_help() {
	printf("Usage:\n");
	printf("\texecutable arg1 input output [arg2] [arg2 value]\n");
	printf("\targ1 should be one of -c (coding) and -d (decoding)\n");
	printf("\targ2 is optional and could be -b (blocksize) with arg2 value the blocksize in MiB.\n");
}

void wrong_argument(char* message) {
	printf("%s", message);
	print_help();
	exit(-1);
}

void check_arguments(int argc, char** argv) {
	if (argc < 4) {
		wrong_argument("Not enough arguments were given!\n");
	}

	if (strcmp(argv[1], "-c") != 0 && strcmp(argv[1], "-d") != 0) {
		wrong_argument("You have to specificy whether you wan't to encode or decode!\n");
	}

	if (argc == 6) {
		if (strcmp(argv[4], "-b") != 0) {
			wrong_argument("Only blocksize (-b) can be specified as the final argument!\n");
		}
	}
}
