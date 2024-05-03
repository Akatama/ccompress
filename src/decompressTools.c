#include"decompressTools.h"
#include<wchar.h>
#include<stdbool.h>

void buildDecompressPrefixTable(FILE *inputFilePtr, PrefixCode **prefixCodeTable)
{
    bool endOfPrefixCode = false;
    bool getLetterNext = false;
    bool getPrefixCodeNext = false;
    wchar_t starter = L';';
    wchar_t middle = L'-';
    wchar_t ender = L'!';

    wchar_t *buffer;
    PrefixCode * prefixCode = (PrefixCode *)malloc(sizeof(PrefixCode));
    wchar_t wc;
    while ((wc = fgetwc(inputFilePtr)) != WEOF)
    {
        if(wc == starter)
            getLetterNext = true;
        else if(wc == middle)
        {
            getLetterNext = false;
            getPrefixCodeNext = true;
        }
        else if(wc == ender)
        {
            if(endOfPrefixCode)
            {
                break;
            }
            endOfPrefixCode = true;
            getPrefixCodeNext = false;
            //prefixCode->code = buffer;
        }
        else
        {
            if(getLetterNext)
            {
                prefixCode->letter = wc;
            }
            else if(getPrefixCodeNext)
            {
                wcsncat(buffer, &wc, 1);
            }
        }
    }
}
