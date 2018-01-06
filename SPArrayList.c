#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPArrayList.h"


SPArrayList *spArrayListCreate(int maxSize)
{
    if (maxSize == 0)
    {
        return NULL;
    }
    
    SPArrayList *array;
    array = malloc(sizeof(SPArrayList));
    if (array == NULL)
    {
        printf("Error: %s has failed", "spArrayListCreate");
        return NULL;
    }
    array->elements = malloc(maxSize * sizeof(int));
    if (array->elements == NULL)
    {
        printf("Error: %s has failed", "spArrayListCreate");
        return NULL;
    }
    array->actualSize = 0;
    array->maxSize = maxSize;
    
    return array;
}

SPArrayList *spArrayListCopy(SPArrayList *src)
{
    if (src == NULL)
    {
        return NULL;
    }
    
    SPArrayList *array;
    array = malloc(sizeof(SPArrayList));
    if (array == NULL)
    {
        printf("Error: %s has failed", "spArrayListCopy");
        return NULL;
    }
    array->elements = malloc(src->maxSize * sizeof(int));
    if (array->elements == NULL)
    {
        printf("Error: %s has failed", "spArrayListCopy");
        return NULL;
    }
    array->actualSize = src->actualSize;
    array->maxSize = src->maxSize;
    
    for (int i = 0; i < src->actualSize; ++i)
    {
        array->elements[i] = src->elements[i];
    }
    
    return array;
}

void spArrayListDestroy(SPArrayList *src)
{
    if (src == NULL)
    {
        return;
    }
    
    free(src->elements);
    free(src);
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList *src)
{
    if (src == NULL)
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    
    src->actualSize = 0;
    
    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList *src, int elem, int index)
{
    if (src == NULL || index > src->actualSize) // Short-Circuit evaluation helps here
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    
    if (src->actualSize == src->maxSize)
    {
        return SP_ARRAY_LIST_FULL;
    }
    
    //Push elements to the right, in reverse order
    for (int i = src->actualSize; i > index; --i)
    {
        src->elements[i] = src->elements[i - 1];
    }
    src->elements[index] = elem;
    src->actualSize++;
    
    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList *src, int elem)
{
    return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList *src, int elem)
{
    if (src == NULL)
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList *src, int index)
{
    if (src == NULL || index >= src->actualSize) // Short-Circuit evaluation helps here
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    
    if (src->actualSize == 0)
    {
        return SP_ARRAY_LIST_EMPTY;
    }
    
    //Push elements to the left, in order
    for (int i = index; i < src->actualSize - 1; i++)
    {
        src->elements[i] = src->elements[i + 1];
    }
    src->actualSize--;
    
    //
    //NOTE: This does not fill the new empty space with zeroes!
    //
    
    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList *src)
{
    return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList *src)
{
    if (src == NULL)
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    
    if (src->actualSize == 0)
    {
        return SP_ARRAY_LIST_EMPTY;
    }
    
    src->actualSize--;
    
    //
    //NOTE: This does not fill the new empty space with zeroes!
    //
    
    return SP_ARRAY_LIST_SUCCESS;
}

int spArrayListGetAt(SPArrayList *src, int index)
{
    //Undefined means it's not my job to test for correctness
    return src->elements[index];
}

int spArrayListGetFirst(SPArrayList *src)
{
    return src->elements[0];
}

int spArrayListGetLast(SPArrayList *src)
{
    return src->elements[src->actualSize - 1];
}

int spArrayListMaxCapacity(SPArrayList *src)
{
    return src->maxSize;
}

int spArrayListSize(SPArrayList *src)
{
    return src->actualSize;
}

bool spArrayListIsFull(SPArrayList *src)
{
    if (src == NULL)
    {
        return false;
    }
    return src->actualSize == src->maxSize;
}

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool spArrayListIsEmpty(SPArrayList *src)
{
    if (src == NULL)
    {
        return false;
    }
    return src->actualSize == 0;
}