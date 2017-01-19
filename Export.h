#ifndef EXPORT_H
#define EXPORT_H

#ifdef __cpluplus
extern "C" {
#endif
	/**
	 * Encode a file using the Huffman encoding.
	 *
	 * @param char* inputFile: Location of the file to be encoded.
	 * @param char* outputFile: Location to store the encoded results.
	 * @param unsigned int blockSize: The size in MiB of one block (This size defines the amount of data a thread consumes
	 *		per run).
	 * @param unsigned int threads: The amount of threads that should be used to encode the file.
	 */
	__declspec(dllexport) void encode_file(char* inputFile, char* outputFile, unsigned int blockSize, unsigned int threads);

	/**
	 * Decode a file that was previously encoded with the Huffman encoding. Only files that were previously encoded with the
	 * encode_file-function can be decoded! The used blocksize that was used for encoding will be automatically detected by the
	 * decoder.
	 *
	 * @param char* inputFile: Location of the file to be decoded.
	 * @param char* outputFile: Location where the decoded file should be saved.
	 */
	__declspec(dllexport) void decode_file(char* inputFile, char* outputFile);
#ifdef __cplusplus
}
#endif

#endif
