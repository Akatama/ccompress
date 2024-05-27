#include <locale.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <uthash.h>
#include <wchar.h>
#include <unistd.h>
#include"compressTools/compressTools.h"
#include "decompressTools/decompressTools.h"
#include "prefixCode/prefixCode.h"

FILE *openFile(char *fileName, char *mode);
void printHelp();

int main(int argc, char **argv)
{
    FILE *inputFilePtr;
    char *inputFileName;

    FILE *outputFilePtr;
    char *outputFileName;

    bool compressFlag = false;
    bool decompressFlag = false;
    bool helpFlag = false;
    bool quietFlag = false;

    setlocale(LC_ALL, "");
    inputFileName = argv[1];

    int options;
    while (( options = getopt(argc, argv, "c:d:hq")) != -1)
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
            case 'h':
                helpFlag = true;
                break;
            case 'q':
                quietFlag = true;
                break;
            case '?':
                printf("Error: unknown option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    if(helpFlag)
    {
        printHelp();
        exit(EXIT_SUCCESS);
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
        PrefixCode *prefixCodeTable = getPrefixCodes(inputFilePtr, outputFilePtr, quietFlag);
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
        readHeader(inputFilePtr, &prefixCodeTable, quietFlag);

        if(!quietFlag)
            printPrefixTable(&prefixCodeTable);

        decompress(inputFilePtr, outputFilePtr, &prefixCodeTable, quietFlag);

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

void printHelp()
{
    printf("Usage: ccompress [INPUT_FILE] [OPTION] ... [OUTPUT_FILE]\n\n");
    printf("Compress or Decompress INPUT_FILE into OUTPUT_FILE.\n\n");
    printf("-c\tcompresses INPUT_FILE and stores the results in OUTPUT_FILE.\n");
    printf("-d\tdecompresses OUTPUT_FILE and stores the results in OUTPUT_FILE.\n");
    printf("-q\t\"quiet mode\" supresses printing of information used during the compression or decompression process\n");
    printf("If both -c and -d options are provided, error.\n\n");

}

