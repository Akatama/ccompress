#include"compressTools.h"
#include "../huffman/huffman.h"
#include "../letterWeight/letterWeight.h"
#include <complex.h>
#include <uthash.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>

void writeHeader(FILE *outputFilePtr, PrefixCode **prefixCodeTable)
{
    wchar_t starter = L';'; 
    wchar_t middle = L'-';
    wchar_t ender = L'!';
    wchar_t one = L'1';
    wchar_t zero = L'0';

    PrefixCode * prefixCode;
    for (prefixCode = *prefixCodeTable; prefixCode != NULL; prefixCode = (PrefixCode *)prefixCode->hh.next)
    {
        fwrite(&starter, sizeof(wchar_t), 1, outputFilePtr);
        fwrite(&prefixCode->letter, sizeof(wchar_t), 1, outputFilePtr);
        fwrite(&middle, sizeof(wchar_t), 1, outputFilePtr);
        char * temp;
        for(temp = prefixCode->code; *temp != '\0'; temp++)
        {
            if(*temp == '1')
            {
                fwrite(&one,sizeof(wchar_t), 1, outputFilePtr);
            }
            else if(*temp == '0')
            {
                fwrite(&zero, sizeof(wchar_t), 1, outputFilePtr);
            }
        }
        fwrite(&ender, 1, sizeof(wchar_t), outputFilePtr);

    }
    fwrite(&ender, 1, sizeof(wchar_t), outputFilePtr);
}

void compress(FILE *inputFilePtr, FILE *outputFilePtr, PrefixCode **prefixCodeTable)
{    
    char buffer = 0;
    int length = 8;
    int bitsLeft = 7;
    wchar_t wc;

    while ((wc = fgetwc(inputFilePtr)) != WEOF)
    {
        PrefixCode *wc_node;
        HASH_FIND_INT(*prefixCodeTable, &wc, wc_node);

        length = strlen(wc_node->code);
        char * temp = wc_node->code;
        while(length > 0)
        {
            //if the character is a one, then flip the bit at location bitsLeft
            if(*temp == '1')
            {
                buffer |= 1<<bitsLeft;
            }
            bitsLeft--;
            length--;
            temp++;
            if(bitsLeft < 0)
            {

                fputc(buffer, outputFilePtr);
                buffer = 0;
                bitsLeft = 7;
            }
        }
    }
    //flip one last bit to 1 unless we have no extra space left in a byte at the end)
    if(bitsLeft >= 0)
    {
        buffer |= 1<<bitsLeft;
    }
    fputc(buffer, outputFilePtr);
}

PrefixCode *getPrefixCodes(FILE *inputFilePtr, FILE *outputFilePtr, bool quietFlag)
{
    LetterWeight *letterWeights = NULL;
    PrefixCode *prefixCodeTable = NULL;

    countLetters(inputFilePtr, &letterWeights);
    if(!quietFlag)
        printLetterWeights(&letterWeights);

    //build the heap
    Heap *minHeap = createHeap(HASH_COUNT(letterWeights), &letterWeights);
    freeLetterWeights(&letterWeights);

    //build the HuffMan tree
    Node * tree;
    buildHuffmanTree(&tree, minHeap);

    //build the prefix table
    buildCompressPrefixTable(tree, &prefixCodeTable, "\0", 1);
    if(!quietFlag)
        printPrefixTable(&prefixCodeTable);
    freeHuffmanTree(tree);

    return prefixCodeTable;
}
