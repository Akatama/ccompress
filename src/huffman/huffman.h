#ifndef HUFFMAN_H
#define HUFFMAN_H
#include"../letterWeight/letterWeight.h"

typedef struct node
{
    unsigned long weight;
    wchar_t letter;
    struct node *left, *right;
} Node;


typedef struct heap
{
    int size;
    int capacity;
    Node **arr;
} Heap;


Heap* createHeap(int capacity, LetterWeight **nodes);
void heapify(Heap *h, int index);
Node* popMin(Heap *h);
void insert(Heap *h, Node *data);
void insertHelper(Heap *h, int index);
void buildHuffmanTree(Node **tree, Heap *h);
void freeHuffmanTree(Node *tree);
#endif
