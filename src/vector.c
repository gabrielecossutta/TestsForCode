#include "vector.h"
#include <stdlib.h>

vector_t* vector_new()
{
    vector_t* vector = (vector_t*)malloc(sizeof(vector_t));
    vector->items = NULL;
    vector->count = 0;
    return vector;
}

void vector_destroy(vector_t* vector)
{
    free(vector->items);
    free(vector);
}

bool vector_is_empty(vector_t* vector)
{
    
    return vector->count == 0;
}

void vector_add(vector_t* vector, void* item)
{
    vector->items = realloc(vector->items, sizeof(void*) * (vector->count + 1));

    vector->items[vector->count] = item;
    vector->count++;
}


void* vector_get(vector_t* vector, size_t index)
{
    return vector->items[index];
}




