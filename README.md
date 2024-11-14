# UnitTests

### Main.c
```c
#define CLOVE_IMPLEMENTATION
#include "clove-unit.h"

CLOVE_RUNNER()
```

### Vector.c
```c
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
```
##

### Vector.h
```h
#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

typedef struct vector_t
{
    void** items;
    size_t count;
} vector_t;

/// @brief Create a new empty vector
/// @return a vector_t pointer 
vector_t* vector_new();

/// @brief Destroy a vector
/// @param vector the vector to be destroyed
void vector_destroy(vector_t* vector);

/// @brief Chech if a vector is empty
/// @param vector the vector to be checked
/// @return true if empty, false otherwise
bool vector_is_empty(vector_t* vector);

/// @brief add one item to the vector
/// @param vector pointer to the vector
/// @param item pointer to the item to be added
void vector_add(vector_t* vector, void* item);

/// @brief get the item in the vector at the specified index
/// @param vector pointer to the vector
/// @param index index of the desired item
/// @return item of the vector
void* vector_get(vector_t* vector, size_t index);



#endif //VECTOR_H
```
##

### Vector_test.c
```c
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
```
##
### Dict.c
```c
#include "xdict.h"
#include <stdlib.h>
#include <string.h>

// Bernstein classic hash
static uint32_t djb33x_hash(void *key, uint32_t keylen) {
    int hash = 5381;
    int i;

    unsigned char *key_as_num = (unsigned char *)key;

    for (i = 0; i < keylen; i++) {
        hash = ((hash << 5) + hash) ^ key_as_num[i];
    }
    return hash;
}

xdict* xdict_new() {
    return xdict_new_with_params(10, djb33x_hash);
}

xdict* xdict_new_with_params(uint32_t hashmap_size, uint32_t (*hash_funct)(void *key, uint32_t keylen))
{
    xdict* d = malloc(sizeof(xdict));
    d->hashmap_size = hashmap_size;
    d->hashmap = malloc(d->hashmap_size * sizeof(void*));
    d->count = 0;
    d->hash_funct = hash_funct;
    return d;
}

void xdict_put(xdict* dict, void* key, uint32_t key_size, void* value) {
    //Scenario 1: hash(Key) not present
    uint32_t hash = dict->hash_funct(key, key_size);
    uint32_t hash_index = hash % dict->hashmap_size;
    if (!dict->hashmap[hash_index]) {
        xdict_node* node = malloc(sizeof(xdict_node)); 
        node->key = key;        
        node->key_size = key_size;
        node->value = value;
        node->next = NULL;

        dict->hashmap[hash_index] = node;
        dict->count++;
    } else { //Scenario 2: hash(Key) already present 
        xdict_node* node = dict->hashmap[hash_index];
        xdict_node* last = NULL;
        
        //Scenario 2.1: key already exists
        while(node) {
            last = node;
            if (node->key_size == key_size && !memcmp(node->key, key, key_size)) {
                node->value = value;
                return;
            }
            node = node->next;
        }

        //Scenario 2.2: key not exists
        xdict_node* new_node = malloc(sizeof(xdict_node));
        node->key = key;
        new_node->key_size = key_size;
        
        new_node->value = value;
        new_node->next = NULL;

        last->next = new_node;
        dict->count++;
    }
}

uint32_t xdict_size(xdict* dict)
{
    return dict->count;
}
```
##

### Dict.h
```h
#ifndef XDICT_H
#define XDICT_H
#include <stdbool.h>
#include <stdint.h>

typedef struct xdict_node_t xdict_node;
struct xdict_node_t {
    void*        key;
    uint32_t     key_size;
    void*        value;
    xdict_node*  next;
};

//dictionary = vectory + linkedlist
typedef struct xdict_t {
    int          count;
    void**       hashmap; //[hash] = l-list
    unsigned int hashmap_size;
    uint32_t (*hash_funct)(void *key, uint32_t keylen);
} xdict;

xdict*  xdict_new();
xdict*  xdict_new_with_params(uint32_t hashmap_size, uint32_t (*hash_funct)(void *key, uint32_t keylen));
bool xdict_is_empty(xdict*);
void xdict_put(xdict*, void* key, uint32_t key_size, void* value);
uint32_t xdict_size(xdict*);
void* xdict_get(xdict*, uint32_t index);

#endif //XDICT_H
```
##

### Dict_test.c
```c
#define CLOVE_SUITE_NAME XDictTest
#include "clove-unit.h"
#include "xdict.h"

static uint32_t constant_hash(void *key, uint32_t keylen) 
{
    return 0;
}

CLOVE_TEST(TestSimple)
{
    xdict* dict = xdict_new_with_params(10, constant_hash);

    int key1 = 1;
    int value1 = 11;
    xdict_put(dict, &key1, sizeof(value1), &value1);

    int key2 = 2;
    int value2 = 21;
    xdict_put(dict, &key2, sizeof(value2), &value2);
    
    CLOVE_UINT_EQ(2, xdict_size(dict));
}

```
##
