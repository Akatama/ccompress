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
    
    char currentBuffer;
    char nextBuffer;
    int shiftVal = 0;
    int bitsLeft = 7;

    fread(&currentBuffer, sizeof(currentBuffer), 1, inputFilePtr);
    fread(&nextBuffer, sizeof(nextBuffer), 1, inputFilePtr);
    while (!feof(inputFilePtr))
    {
        while(bitsLeft >= 0)
        {
            // see if bit at location bitsLeft is 1 or a zero
            shiftVal = currentBuffer & 1<<bitsLeft;
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
        currentBuffer = nextBuffer;
        fread(&nextBuffer, sizeof(nextBuffer), 1, inputFilePtr);
        bitsLeft = 7;
    }

    //currentBuffer now has the last byte in the file
    //first, we need to figure out which index in the byte has the last 1
    //This last 1 signals the end of the compressed stream, any data passed
    //it is junk data and should not be written
    //After we find that out, the rest of this is the same as what we did before
    int lastBit = 0;
    int i;
    for(i = 7; i >= 0; i--)
    {
        shiftVal = currentBuffer & 1<<i;
        if(shiftVal > 0)
            lastBit = i;
        shiftVal = 0;
    }
    //last bit is the end, so we only need to read bits before it
    while(bitsLeft > lastBit)
    {
        shiftVal = currentBuffer & 1<<bitsLeft;
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


}
