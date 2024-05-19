#include"letterWeight.h"
#include<wchar.h>

void countLetters(FILE *fptr, LetterWeight **letterWeights)
{
    wchar_t wc;
    while ((wc = fgetwc(fptr)) != WEOF)
    {
        LetterWeight *weight;
        HASH_FIND_INT(*letterWeights, &wc, weight);
        if(weight == NULL)
        {
            weight = (LetterWeight *)malloc(sizeof(LetterWeight));
            weight->letter = wc;
            weight->weight = 1;
            HASH_ADD_INT(*letterWeights, letter, weight);
        }
        else
        {
            weight->weight++;
        }
    }
}


void printLetterWeights(LetterWeight **letterWeights)
{
    LetterWeight *letterWeight;
    for (letterWeight = *letterWeights; letterWeight != NULL; letterWeight = (LetterWeight *)letterWeight->hh.next)
    {
        printf("%c %ld\n", letterWeight->letter, letterWeight->weight);
    }

}


void freeLetterWeights(LetterWeight **letterWeights)
{
    LetterWeight * current, *tmp;
    HASH_ITER(hh, *letterWeights, current, tmp)
    {
        HASH_DEL(*letterWeights, current);
        free(current);
    }
}
