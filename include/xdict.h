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