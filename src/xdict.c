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