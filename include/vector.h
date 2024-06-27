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