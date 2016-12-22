/**
 * This file contains the main-function for the first generic compression-algorithm.
 *
 * @author Pieter Verschaffelt
 */

#include "HuffmanCompression.h"
#include "ArgumentChecker.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	check_arguments(argc, argv);

	char* inputFile = argv[2];
	char* outputFile = argv[3];

	unsigned int blockSize = 0;
	if (argc == 6) {
		blockSize = atoi(argv[5]);
	}

	if (blockSize <= 0) {
		// Default block size
		blockSize = 16;
	}

	time_t start = time(NULL);
	unsigned long long totalSize;
	// Check if the user wants to encode or decode a file. Because we checked the arguments previously
	// we can rely on the fact that exactly one of decode or encode is specified by the user
	if (strcmp(argv[1], "-c") == 0) {
		printf("Started encoding file \"%s\" with blocks of %d MiB\n", inputFile, blockSize);
		huffman_encode_file(inputFile, outputFile, blockSize, &totalSize, 8);
		printf("Encoding finished. Results were written to file \"%s\".\n", outputFile);
	} else {
		printf("Started decoding file \"%s\"\n", inputFile);
		huffman_decode_file(inputFile, outputFile, &totalSize), 4;
		printf("Decoding finished. Results were written to file \"%s\".\n", outputFile);
	}
	time_t end = time(NULL);

	// We can only print speed results when elapsed time is more than 0 seconds!
	if (end - start != 0) {
		double mbPerSec = (totalSize / (1024 * 1024)) / ((double) (end - start));
		printf("Average speed was %.2f MB/sec\n", mbPerSec);
	}
	printf("Run took %d sec \n", (unsigned int) (end - start));
	getchar();
	return 0;
}
