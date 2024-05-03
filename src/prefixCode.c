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
        //printf("letter: %c, code: %s\n", prefixCode->letter, prefixCode->code);
        HASH_ADD_INT(*prefixCodeTable, letter, prefixCode);
    }
    else
    {
        char *leftString = (char *)malloc((codeLength + 1)*sizeof(char));
        strncpy(leftString, code, codeLength);
        strncat(leftString, "0\0", 2);
        buildCompressPrefixTable(tree->left, prefixCodeTable, leftString, codeLength+1);

        char *rightString = (char *)malloc((codeLength + 1)*sizeof(char));
        strncpy(rightString, code, codeLength);
        strncat(rightString, "1\0", 2);
        buildCompressPrefixTable(tree->right, prefixCodeTable, rightString, codeLength+1);
        
        free(leftString);
        free(rightString);
    }
}

void printPrefixTable(PrefixCode **prefixCodeTable)
{

    PrefixCode * prefixCode;
    for (prefixCode = *prefixCodeTable; prefixCode != NULL; prefixCode = (PrefixCode *)prefixCode->hh.next)
    {
        char * c;
        printf("char: %c, %d, code: %s\n", prefixCode->letter, prefixCode->letter, prefixCode->code);
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
