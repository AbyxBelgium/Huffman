#ifndef EXPORT_H
#define EXPORT_H

#ifdef __cpluplus
extern "C" {
#endif
	__declspec(dllexport) void encode_file(char* inputFile, char* outputFile, unsigned int blockSize);
	__declspec(dllexport) void decode_file(char* inputFile, char* outputFile);
#ifdef __cplusplus
}
#endif

#endif
