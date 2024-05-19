#ifndef DECOMPRESSTOOLS_H
#define DECOMPRESSTOOLS_H
#include"../prefixCode/prefixCode.h"
void readHeader(FILE *inputFilePtr, PrefixCode **prefixCodeTable);
void decompress(FILE *inputFilePtr, FILE *outputFilePtr, PrefixCode **prefixCodeTable);
#endif
