#ifndef PREFIXCODE_H
#define PREFIXCODE_H

#include <stdio.h>
#include <uthash.h>
#include"../huffman//huffman.h"

typedef struct prefixCode
{
    wchar_t letter;
    char *code;
    UT_hash_handle hh;
} PrefixCode;

void buildCompressPrefixTable(Node *tree, PrefixCode **prefixCodeTable, char *code, size_t codeLength);
void printPrefixTable(PrefixCode **prefixCodeTable);
void freePrefixTable(PrefixCode **prefixCodeTable);
char *concatOne(char *currentCode, size_t currentCodeLength);
char *concatZero(char *currentCode, size_t currentCodeLength);
char *concatCode(char *currentCode, size_t currentCodeLength, char *concatChars, size_t concatCharsLength);
#endif
