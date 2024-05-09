#include"decompressTools.h"
#include "prefixCode.h"
#include <stdio.h>
#include <string.h>
#include <uthash.h>
#include<wchar.h>
#include<stdbool.h>

size_t buildDecompressPrefixTable(FILE *inputFilePtr, PrefixCode **prefixCodeTable)
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
    size_t numOfWideCharacters = 0;

    char *buffer = (char *)malloc(sizeof(char));
    strncpy(buffer, "/0", 1);
    PrefixCode * prefixCode = (PrefixCode *)malloc(sizeof(PrefixCode));
    wchar_t wc;
    printf("starting to build the decompress prefix table\n");
    while (!feof(inputFilePtr))
    {
        fread(&wc, sizeof(wc), 1, inputFilePtr);
        numOfWideCharacters++;
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
                strncpy(buffer, "/0", 1);
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
    return sizeof(wchar_t) * numOfWideCharacters;
}
