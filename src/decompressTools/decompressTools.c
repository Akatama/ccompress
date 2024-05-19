#include"decompressTools.h"
#include <stdio.h>
#include <string.h>
#include <uthash.h>
#include<wchar.h>
#include<stdbool.h>
#include <stdlib.h>

void readHeader(FILE *inputFilePtr, PrefixCode **prefixCodeTable, bool quietFlag)
{
    bool endOfPrefixCode = false;
    bool getLetterNext = false;
    bool getPrefixCodeNext = false;

    wchar_t starter = L';';
    wchar_t middle = L'-';
    wchar_t ender = L'!';
    wchar_t one = L'1';
    wchar_t zero = L'0';

    size_t bufferLength = 1;
    char *buffer = (char *)malloc(sizeof(char));
    strncpy(buffer, "\0", 1);

    PrefixCode * prefixCode = (PrefixCode *)malloc(sizeof(PrefixCode));
    
    wchar_t wc;

    if(!quietFlag)
        printf("starting to build the decompress prefix table\n");
    fread(&wc, sizeof(wc), 1, inputFilePtr);
    while (!feof(inputFilePtr))
    {

        if(getLetterNext)
        {
            prefixCode->letter = wc;
            getLetterNext = false;
        }
        //need to be able to check if we are done getting the prefixCode
        else if(wc == ender)
        {
            if(endOfPrefixCode)
            {
                break;
            }
            else
            {
                getPrefixCodeNext = false;

                prefixCode->code = buffer;
                HASH_ADD_KEYPTR(hh, *prefixCodeTable, prefixCode->code, strlen(prefixCode->code), prefixCode);
                
                //reset everything back for the next look
                bufferLength = 1;
                prefixCode = (PrefixCode *)malloc(sizeof(PrefixCode));
                buffer = (char *)malloc(sizeof(char));
                strncpy(buffer, "\0", 1);
                endOfPrefixCode = true;
            }
        }
        else if(getPrefixCodeNext)
        {
            bufferLength++;
            char *temp = (char *)malloc((bufferLength)*sizeof(char));
            if(wc == zero) 
            {
                temp = concatZero(buffer, bufferLength);
            }
            else if (wc == one) 
            {
                temp = concatOne(buffer, bufferLength);
            }
            free(buffer);
            buffer = temp;
        }
        else if(wc == starter)
        {
            getLetterNext = true;
            endOfPrefixCode = false;
        }
        else if(wc == middle)
        {
            getLetterNext = false;
            getPrefixCodeNext = true;
        }
        
        fread(&wc, sizeof(wc), 1, inputFilePtr);
    }
}

void decompress(FILE *inputFilePtr, FILE *outputFilePtr, PrefixCode **prefixCodeTable, bool quietFlag)
{
    if(!quietFlag)
        printf("Starting decompression\n");

    PrefixCode *foundPrefixCode = NULL;

    char *currentCode = (char *)malloc(sizeof(char));
    strncpy(currentCode, "\0", 1);
    size_t currentCodeLength = 1;
    
    char readBuffer;
    int shiftVal = 0;
    int bitsLeft = 7;

    fread(&readBuffer, sizeof(readBuffer), 1, inputFilePtr);
    while (!feof(inputFilePtr))
    {
        while(bitsLeft >= 0)
        {
            // see if bit at location bitsLeft is 1 or a zero
            shiftVal = readBuffer & 1<<bitsLeft;
            bitsLeft--;

            //if shiftVal is not zero, then it was a 1
            if(shiftVal > 0)
            {
                char *temp = concatOne(currentCode, currentCodeLength);
                free(currentCode);
                currentCode = temp;
            }
            else
            {
                char *temp = concatZero(currentCode, currentCodeLength);
                free(currentCode);
                currentCode = temp;
            }

            currentCodeLength++;
            HASH_FIND_STR(*prefixCodeTable, currentCode, foundPrefixCode);
            //if we have found a code that matches then add it
            if(foundPrefixCode != NULL)
            {
                fputwc(foundPrefixCode->letter, outputFilePtr);
                foundPrefixCode = NULL;
                currentCodeLength = 1;
                free(currentCode);
                currentCode = (char *)malloc(sizeof(char));
                strncpy(currentCode, "\0", 1);

            }
        }
        fread(&readBuffer, sizeof(readBuffer), 1, inputFilePtr);
        bitsLeft = 7;
    }


}
