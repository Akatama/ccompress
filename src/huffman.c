#include<wchar.h>
#include"huffman.h"

Heap* createHeap(int capacity, LetterWeight **letterWeights)
{
    Heap* h = (Heap*)malloc(sizeof(Heap));

    if (h == NULL)
    {
        printf("Memory error initializing heap");
        exit(EXIT_FAILURE);
    }

    h->size = 0;
    h->capacity = capacity;

    h->arr = (Node **)malloc(capacity * sizeof(Node **));

    if(h->arr == NULL)
    {
        printf("Memory error initializing array for the heap");
        exit(EXIT_FAILURE);
    }
    LetterWeight *letterWeight;
    int i = 0;
    for(letterWeight = *letterWeights; letterWeight != NULL; letterWeight = (LetterWeight *)letterWeight->hh.next)
    {
        Node *temp = (Node *)malloc(sizeof(Node));
        temp->weight = letterWeight->weight;
        temp->letter = letterWeight->letter;
        temp->left = NULL;
        temp->right = NULL;
        h->arr[i] = temp;
        i++;
    }

    h->size = i;
    i = (h->size - 2) / 2;
    while (i >= 0)
    {
        heapify(h, i);
        i--;
    }
    return h;
}

void heapify(Heap *h, int index)
{
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int min = index;

    if(left >= h->size || left < 0)
        left = -1;
    if (right >= h->size || right < 0)
        right = -1;

    if(left != -1 && h->arr[left]->weight < h->arr[index]->weight)
        min = left;
    if(right != -1 && h->arr[right]->weight < h->arr[min]->weight)
        min = right;

    if (min != index)
    {
        Node *temp = h->arr[min];
        h->arr[min] = h->arr[index];
        h->arr[index] = temp;
        heapify(h, min);
    }
}

Node *popMin(Heap *h)
{
    Node* deleteNode;

    if(h->size == 0)
    {
        printf("\nHead is empty.\n");
        return NULL;
    }

    deleteNode = h->arr[0];
    // Replace deleted node with the last node
    h->arr[0] = h->arr[h->size - 1];

    h->size--;
    heapify(h, 0);
    return deleteNode;
}

void insert(Heap *h, Node *data)
{
    if (h->size < h->capacity)
    {
        h->arr[h->size] = data;
        insertHelper(h, h->size);
        h->size++;
    }
}

void insertHelper(Heap *h, int index)
{
    int parent = (index - 1) / 2;

    if(h->arr[parent]->weight > h->arr[index]->weight)
    {
        Node *temp = h->arr[parent];
        h->arr[parent] = h->arr[index];
        h->arr[index] = temp;

        insertHelper(h, parent);
    }
}

void buildHuffmanTree(Node **tree, Heap *h)
{
    Node * tmp1, *tmp2, *tmp3 = NULL;

    while(h->size > 1)
    {
        tmp1 = popMin(h);
        tmp2 = popMin(h);
        tmp3 = (Node *)malloc(sizeof(Node));
        tmp3->weight = tmp1->weight + tmp2->weight;
        tmp3->letter = 0;
        tmp3->left = tmp1;
        tmp3->right = tmp2;
        insert(h, tmp3);
    }
    *tree = tmp3;
}

void freeHuffmanTree(Node *tree)
{
    if(tree->left != NULL)
        freeHuffmanTree(tree->left);
    if(tree->right != NULL)
        freeHuffmanTree(tree->right);
    free(tree);
}
