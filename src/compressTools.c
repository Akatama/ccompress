#include"compressTools.h"
#include <complex.h>
#include <uthash.h>
#include <stdio.h>
#include <wchar.h>

void writeHeader(FILE *outputFilePtr, PrefixCode **prefixCodeTable)
{
    wchar_t *starter = malloc(sizeof(wchar_t));
    *starter = L';';
    wchar_t *middle = malloc(sizeof(wchar_t));
    *middle = L'-';
    wchar_t *ender = malloc(sizeof(wchar_t));
    *ender = L'!';
    PrefixCode * prefixCode;
    for (prefixCode = *prefixCodeTable; prefixCode != NULL; prefixCode = (PrefixCode *)prefixCode->hh.next)
    {
        fwrite(starter, sizeof(wchar_t), 1, outputFilePtr);
        fwrite(&prefixCode->letter, sizeof(wchar_t), 1, outputFilePtr);
        fwrite(middle, sizeof(wchar_t), 1, outputFilePtr);
        char * temp;
        for(temp = prefixCode->code; *temp != '\0'; temp++)
        {
            if(*temp == '1' || *temp == '0')
            {
                fputc(*temp, outputFilePtr);
            }
        }
        fwrite(ender, 1, sizeof(wchar_t), outputFilePtr);

    }
    fwrite(ender, 1, sizeof(wchar_t), outputFilePtr);
}

void compress(FILE *inputFilePtr, FILE *outputFilePtr, PrefixCode **prefixCodeTable)
{    
    char buffer = 0;
    int length,bitsLeft = 8;
    wchar_t wc;
    while ((wc = fgetwc(inputFilePtr)) != WEOF)
    {
        PrefixCode *wc_node;
        HASH_FIND_INT(*prefixCodeTable, &wc, wc_node);
        length = strlen(wc_node->code);
        char * temp = wc_node->code;
        while(length > 0)
        {
            if(*temp == '1')
            {
                buffer |= (1 << (7 - bitsLeft));
            }
            bitsLeft--;
            length--;
            temp++;
            if(bitsLeft == 0)
            {
                fputc(buffer, outputFilePtr);
                buffer = 0;
                bitsLeft = 8;
            }
        }
    }
    if (bitsLeft != 8)
    {
        buffer = buffer << (bitsLeft - 1);
        fputc(buffer, outputFilePtr);
    }
}

void compressFile(FILE *inputFilePtr, FILE *outputFilePtr)
{
    LetterWeight *letterWeights = NULL;
    PrefixCode *prefixCodeTable = NULL;

    countLetters(inputFilePtr, &letterWeights);
    fseek(inputFilePtr, 0, SEEK_SET);


    printLetterWeights(&letterWeights);

    //build the heap
    Heap *h = createHeap(HASH_COUNT(letterWeights), &letterWeights);

    //build the HuffMan tree
    Node * tree;
    buildHuffmanTree(&tree, h);
    //printHuffmanTreeInorder(tree);

    //build the prefix table
    buildCompressPrefixTable(tree, &prefixCodeTable, "\0", 1);
    printPrefixTable(&prefixCodeTable);

    //write the header files to the file
    writeHeader(outputFilePtr, &prefixCodeTable);
    compress(inputFilePtr, outputFilePtr, &prefixCodeTable);

    fclose(inputFilePtr);
    fclose(outputFilePtr);
    //free(outputFilePtr);
    freeLetterWeights(&letterWeights);
    freePrefixTable(&prefixCodeTable);
    freeHuffmanTree(tree);
}
