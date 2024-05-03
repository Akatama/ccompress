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

        inputFilePtr = fopen(inputFileName, "r");
        if(inputFilePtr == NULL)
        {
            printf("Not able to open file %s\n", inputFileName);
            exit(EXIT_FAILURE);
        }

        outputFilePtr = fopen(outputFileName, "wb");

        if(outputFilePtr == NULL)
        {
            printf("Not able to open file %s\n", outputFileName);
            exit(EXIT_FAILURE);
        }

        compressFile(inputFilePtr, outputFilePtr);
    }
    else if(decompressFlag)
    {
    }
    else
    {
        printf("I don't know what to do. Please select -c to compress or -d to decompress.\n");
        exit(EXIT_FAILURE);
    }
}

