#ifndef ST_MEMORY_H
#define ST_MEMORY_H

#include <stddef.h>


/*
 * Allocate memory.
 */
void* st_memory_alloc(
    size_t size
);


/*
 * Allocate zeroed memory.
 */
void* st_memory_calloc(
    size_t count,
    size_t size
);


/*
 * Resize allocated memory.
 */
void* st_memory_realloc(
    void* memory,
    size_t size
);


/*
 * Free allocated memory.
 */
void st_memory_free(
    void* memory
);


/*
 * Copy memory.
 */
void st_memory_copy(
    void* destination,
    const void* source,
    size_t size
);


/*
 * Set memory to a value.
 */
void st_memory_set(
    void* memory,
    int value,
    size_t size
);


/*
 * Compare two memory blocks.
 */
int st_memory_compare(
    const void* left,
    const void* right,
    size_t size
);

#endif /* ST_MEMORY_H */