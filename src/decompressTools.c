#include"decompressTools.h"
#include "prefixCode.h"
#include <stdio.h>
#include <string.h>
#include <uthash.h>
#include<wchar.h>
#include<stdbool.h>

void readHeader(FILE *inputFilePtr, PrefixCode **prefixCodeTable)
{
    bool endOfPrefixCode = false;
    bool getLetterNext = false;
    bool getPrefixCodeNext = false;
    wchar_t starter = L';';
    wchar_t middle = L'-';
    wchar_t ender = L'!';
    wchar_t one = L'1';
    wchar_t zero = L'0';
    size_t codeLength = 1;

    char *buffer = (char *)malloc(sizeof(char));
    strncpy(buffer, "\0", 1);
    PrefixCode * prefixCode = (PrefixCode *)malloc(sizeof(PrefixCode));
    wchar_t wc;
    printf("starting to build the decompress prefix table\n");
    while (!feof(inputFilePtr))
    {
        fread(&wc, sizeof(wc), 1, inputFilePtr);
        if(wc == starter)
        {
            getLetterNext = true;
            endOfPrefixCode = false;
        }
        else if(wc == middle)
        {
            getLetterNext = false;
            getPrefixCodeNext = true;
        }
        else if(wc == ender && !getLetterNext)
        {
            if(endOfPrefixCode)
            {
                break;
            }
            else
            {
                getPrefixCodeNext = false;
                prefixCode->code = buffer;
                HASH_ADD_KEYPTR(hh, *prefixCodeTable, prefixCode->code, codeLength, prefixCode);
                codeLength = 1;
                prefixCode = (PrefixCode *)malloc(sizeof(PrefixCode));
                buffer = (char *)malloc(sizeof(char));
                strncpy(buffer, "\0", 1);
                endOfPrefixCode = true;
            }
        }
        else
        {
            if(getLetterNext)
            {
                prefixCode->letter = wc;
            }
            else if(getPrefixCodeNext)
            {
                codeLength++;
                if(wc == zero) 
                {
                    char *tempString = (char *)malloc((codeLength + 1)*sizeof(char));
                    strncpy(tempString, buffer, codeLength);
                    strncat(tempString, "0\0", 2);
                    free(buffer);
                    buffer = tempString;

                }
                else if (wc == one) 
                {
                    char *tempString = (char *)malloc((codeLength + 1)*sizeof(char));
                    strncpy(tempString, buffer, codeLength);
                    strncat(tempString, "1\0", 2);
                    free(buffer);
                    buffer = tempString;
                }

                else
                {
                    printf("Error, invalid character: %c\n", wc);
                }
            }
        }
    }
}


char *concatCode(char *currentCode, size_t currentCodeLength, char *concatChars, size_t concatCharsLength)
{

    //substract one from currentCodeLength and concatCharsLength because  both char ptrs end with /0
    char *newCode = (char*)malloc((currentCodeLength+concatCharsLength - 1)*sizeof(char));
    strncpy(newCode, currentCode, currentCodeLength);
    strncat(newCode, concatChars, concatCharsLength);
    return newCode;
     
}

void decompress(FILE *inputFilePtr, FILE *outputFilePtr, PrefixCode **prefixCodeTable)
{
    PrefixCode *foundPrefixCode = NULL;
    char readBuffer;
    int shiftVal = 0;
    char *currentCode = "\0";
    size_t currentCodeLength = 1;
    int bitsLeft = 7;

    while (!feof(inputFilePtr))
    {
        fread(&readBuffer, sizeof(readBuffer), 1, inputFilePtr);
        bitsLeft = 7;
        while(bitsLeft >= 0)
        {
            shiftVal = readBuffer & 1<<bitsLeft;
            bitsLeft--;
            if(shiftVal > 0)
            {
                char *temp = concatCode(currentCode, currentCodeLength, "1\0", 2);
                free(currentCode);
                currentCode = temp;
            }
            else
            {
                char *temp = concatCode(currentCode, currentCodeLength, "0\0", 2);
                free(currentCode);
                currentCode = temp;
            }
            currentCodeLength++;

            HASH_FIND_STR(*prefixCodeTable, currentCode, foundPrefixCode);
            //if we have found a code that matches
            if(foundPrefixCode)
            {
                fwrite(&foundPrefixCode->letter, sizeof(wchar_t), 1, outputFilePtr);
                prefixCodeTable = NULL;
                currentCodeLength = 1;
                free(currentCode);
                currentCode = (char *)malloc(sizeof(char));
                strncpy(currentCode, "\0", 1);

            }

        }
    }


}
