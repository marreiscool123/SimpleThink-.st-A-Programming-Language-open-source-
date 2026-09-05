#include "memory.h"

#include <stdlib.h>
#include <string.h>


void* st_memory_alloc(
    size_t size
)
{
    if (size == 0)
        return NULL;

    return malloc(size);
}


void* st_memory_calloc(
    size_t count,
    size_t size
)
{
    if (
        count == 0 ||
        size == 0
    )
    {
        return NULL;
    }

    return calloc(
        count,
        size
    );
}


void* st_memory_realloc(
    void* memory,
    size_t size
)
{
    if (size == 0)
    {
        free(memory);
        return NULL;
    }

    return realloc(
        memory,
        size
    );
}


void st_memory_free(
    void* memory
)
{
    if (memory == NULL)
        return;

    free(memory);
}


void st_memory_copy(
    void* destination,
    const void* source,
    size_t size
)
{
    if (
        destination == NULL ||
        source == NULL ||
        size == 0
    )
    {
        return;
    }

    memcpy(
        destination,
        source,
        size
    );
}


void st_memory_set(
    void* memory,
    int value,
    size_t size
)
{
    if (
        memory == NULL ||
        size == 0
    )
    {
        return;
    }

    memset(
        memory,
        value,
        size
    );
}


int st_memory_compare(
    const void* left,
    const void* right,
    size_t size
)
{
    if (
        left == NULL ||
        right == NULL
    )
    {
        return 0;
    }

    return memcmp(
        left,
        right,
        size
    );
}