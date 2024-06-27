#define CLOVE_SUITE_NAME VectorTest
#include "clove-unit.h"
#include "vector.h"

CLOVE_TEST(CreateNewEmptyVector)
{
    vector_t* vector = vector_new();
    CLOVE_NOT_NULL(vector);

    bool is_empty = vector_is_empty(vector);
    CLOVE_IS_TRUE(is_empty);

    vector_destroy(vector);
}

CLOVE_TEST(AddOneElement)
{
    vector_t* vector = vector_new();
    int element = 1;
    vector_add(vector, &element);
    CLOVE_PTR_EQ(&element, vector->items[0]);
    CLOVE_SIZET_EQ(1, vector->count);
    vector_destroy(vector);
}

CLOVE_TEST(GetElement)
{
    vector_t* vector = vector_new();
    int element = 1;
    vector_add(vector, &element);
    size_t index = 0;
    void* item = vector_get(vector, index);
    CLOVE_PTR_EQ(&element, item);
}

