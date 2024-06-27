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


