#ifndef COMPRESSTOOLS_H
#define COMPRESSTOOLS_H

#include <stdio.h>
#include"prefixCode.h"
void writeHeader(FILE *outputFilePtr, PrefixCode **prefixCodeTable);
void compress(FILE *inputFilePtr, FILE *outputFilePtr, PrefixCode **prefixCodeTable);
PrefixCode *getPrefixCodes(FILE *inputFilePtr, FILE *outputFilePtr);
#endif
