#ifndef LETTERWEIGHT_H
#define LETTERWEIGHT_H
#include <uthash.h>
#include <stdio.h>

typedef struct letterWeight
{
    wchar_t letter;
    unsigned long weight;
    UT_hash_handle hh;
} LetterWeight;


void countLetters(FILE *fptr, LetterWeight **letterWeights);
void printLetterWeights(LetterWeight **letterWeights);

void freeLetterWeights(LetterWeight **letterWeights);
#endif
