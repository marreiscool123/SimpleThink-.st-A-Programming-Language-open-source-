#ifndef ST_ARRAYS_H
#define ST_ARRAYS_H

#include "types.h"

typedef struct STArray
{
    STType element_type;

    void* data;

    int count;
    int capacity;

    int element_size;

} STArray;


/*
 * Create an array.
 */
STArray* st_array_create(
    STType element_type,
    int element_size
);


/*
 * Add an element to the array.
 */
int st_array_push(
    STArray* array,
    const void* element
);


/*
 * Get an element from the array.
 */
void* st_array_get(
    STArray* array,
    int index
);


/*
 * Set an element in the array.
 */
int st_array_set(
    STArray* array,
    int index,
    const void* element
);


/*
 * Remove an element.
 */
int st_array_remove(
    STArray* array,
    int index
);


/*
 * Get number of elements.
 */
int st_array_count(
    const STArray* array
);


/*
 * Clear all elements.
 */
void st_array_clear(
    STArray* array
);


/*
 * Free the array.
 */
void st_array_free(
    STArray* array
);

#endif /* ST_ARRAYS_H */