#include"prefixCode.h"

void buildCompressPrefixTable(Node *tree, PrefixCode **prefixCodeTable, char *code, size_t codeLength)
{
    if (tree->letter != 0)
    {
        // need to copy the C string because we need an easy way to free all the intermediate codes
        PrefixCode * prefixCode = (PrefixCode *)malloc(sizeof(PrefixCode));
        prefixCode->code = (char *)malloc(codeLength+1 * sizeof(char));
        strncpy(prefixCode->code, code, codeLength);
        prefixCode->letter = tree->letter;

        HASH_ADD_INT(*prefixCodeTable, letter, prefixCode);
    }
    else
    {
        char *leftCode = concatZero(code, codeLength);
        buildCompressPrefixTable(tree->left, prefixCodeTable, leftCode, codeLength+1);

        char *rightCode = concatOne(code, codeLength);
        buildCompressPrefixTable(tree->right, prefixCodeTable, rightCode, codeLength+1);
        
        free(leftCode);
        free(rightCode);
    }
}

void printPrefixTable(PrefixCode **prefixCodeTable)
{

    PrefixCode * prefixCode;
    for (prefixCode = *prefixCodeTable; prefixCode != NULL; prefixCode = (PrefixCode *)prefixCode->hh.next)
    {
        char * c;
        printf("char: %c intVal: %d code: %s\n", prefixCode->letter, prefixCode->letter, prefixCode->code);
    }
}

void freePrefixTable(PrefixCode **prefixCodeTable)
{
    PrefixCode *current, *tmp;
    HASH_ITER(hh, *prefixCodeTable, current, tmp)
    {
        HASH_DEL(*prefixCodeTable, current);
        free(current);
    }
}


char *concatCode(char *currentCode, size_t currentCodeLength, char *concatChars, size_t concatCharsLength)
{

    //substract one from currentCodeLength and concatCharsLength because  both char ptrs end with /0
    char *newCode = (char*)malloc((currentCodeLength+concatCharsLength)*sizeof(char));
    strncpy(newCode, currentCode, currentCodeLength);
    strncat(newCode, concatChars, concatCharsLength);
    return newCode;
     
}

char zero = '0';
char one = '1';
size_t concatSize = 1;

char *concatOne (char *currentCode, size_t currentCodeLength)
{
    return concatCode(currentCode, currentCodeLength, &one, concatSize);
}

char *concatZero (char *currentCode, size_t currentCodeLength)
{
    return concatCode(currentCode, currentCodeLength, &zero, concatSize);
}
