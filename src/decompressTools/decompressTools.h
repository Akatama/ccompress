#ifndef DECOMPRESSTOOLS_H
#define DECOMPRESSTOOLS_H
#include"../prefixCode/prefixCode.h"
#include<stdbool.h>
void readHeader(FILE *inputFilePtr, PrefixCode **prefixCodeTable, bool quietFlag);
void decompress(FILE *inputFilePtr, FILE *outputFilePtr, PrefixCode **prefixCodeTable, bool quietFlag);
#endif
