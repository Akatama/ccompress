#include <locale.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <uthash.h>
#include <utstring.h>
#include <wchar.h>
#include <unistd.h>
#include"compressTools.h"
#include "decompressTools.h"
#include "prefixCode.h"

FILE *openFile(char *fileName, char *mode);

int main(int argc, char **argv)
{
    FILE *inputFilePtr;
    char *inputFileName;
    FILE *outputFilePtr;
    char *outputFileName;
    bool compressFlag = false;
    bool decompressFlag = false;

    setlocale(LC_ALL, "");
    inputFileName = argv[1];

    int options;
    while (( options = getopt(argc, argv, "c:d:")) != -1)
        switch(options)
        {
            case 'c':
                compressFlag = true;
                outputFileName = optarg;
                break;
            case 'd':
                decompressFlag = true;
                outputFileName = optarg;
                break;
            case '?':
                printf("Error: unknown option %c\n", optopt);
                exit(EXIT_FAILURE);
        }

    if(compressFlag && decompressFlag)
    {
        printf("Error: ccompressTool can't both compress and decompress a file at the same time. Please pick -c to compress OR -d to decompress\n");
        exit(EXIT_FAILURE);
    }
    else if(compressFlag)
    {

        inputFilePtr = openFile(inputFileName, "r");
        if(inputFilePtr == NULL)
            exit(EXIT_FAILURE);
        outputFilePtr = openFile(outputFileName, "wb");
        if(outputFilePtr == NULL)
        {
            fclose(inputFilePtr);
            exit(EXIT_FAILURE);
        }

        //get prefixCodes, then write the header
        PrefixCode *prefixCodeTable = getPrefixCodes(inputFilePtr, outputFilePtr);
        writeHeader(outputFilePtr, &prefixCodeTable);
        
        //move input file to the start
        fseek(inputFilePtr, 0, SEEK_SET);

        compress(inputFilePtr, outputFilePtr, &prefixCodeTable);
        freePrefixTable(&prefixCodeTable);

        fclose(inputFilePtr);
        fclose(outputFilePtr);
    }
    else if(decompressFlag)
    {
        inputFilePtr = openFile(inputFileName, "rb");
        if(inputFilePtr == NULL)
        {
            exit(EXIT_FAILURE);
        }

        outputFilePtr = openFile(outputFileName, "w");
        if(outputFilePtr == NULL)
        {

            fclose(inputFilePtr);
            exit(EXIT_FAILURE);
        }

        
        PrefixCode *prefixCodeTable = NULL;
        readHeader(inputFilePtr, &prefixCodeTable);
        printPrefixTable(&prefixCodeTable);

        decompress(inputFilePtr, outputFilePtr, &prefixCodeTable);

        freePrefixTable(&prefixCodeTable);
        fclose(inputFilePtr);
        fclose(outputFilePtr);
    }
    else
    {
        printf("I don't know what to do. Please select -c to compress or -d to decompress.\n");
        exit(EXIT_FAILURE);
    }
}


FILE *openFile(char *fileName, char *mode)
{
    FILE *filePtr = fopen(fileName, mode);
    if(filePtr == NULL)
    {
        printf("Not able to open file %s\n", fileName);
    }
    return filePtr;
}
