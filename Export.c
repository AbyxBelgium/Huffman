#include "Export.h"
#include "HuffmanCompression.h"

__declspec(dllexport) void encode_file(char* inputFile, char* outputFile, unsigned int blockSize) {
	unsigned long long fileSize = 0;
	huffman_encode_file(inputFile, outputFile, blockSize, &fileSize, 4);
}

__declspec(dllexport) void decode_file(char* inputFile, char* outputFile) {
	unsigned long long fileSize = 0;
	huffman_decode_file(inputFile, outputFile, &fileSize);
}