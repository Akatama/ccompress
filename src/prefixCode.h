#ifndef PREFIXCODE_H
#define PREFIXCODE_H

#include <stdio.h>
#include <uthash.h>
#include"huffman.h"

typedef struct prefixCode
{
    wchar_t letter;
    char *code;
    UT_hash_handle hh;
} PrefixCode;

void buildCompressPrefixTable(Node *tree, PrefixCode **prefixCodeTable, char *code, size_t codeLength);
void printPrefixTable(PrefixCode **prefixCodeTable);
void freePrefixTable(PrefixCode **prefixCodeTable);
#endif
