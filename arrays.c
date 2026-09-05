#include "arrays.h"

#include <stdlib.h>
#include <string.h>


#define ST_ARRAY_INITIAL_CAPACITY 16


STArray* st_array_create(
    STType element_type,
    int element_size
)
{
    STArray* array;

    if (element_size <= 0)
        return NULL;

    array = malloc(sizeof(STArray));

    if (array == NULL)
        return NULL;

    memset(
        array,
        0,
        sizeof(STArray)
    );

    array->element_type = element_type;
    array->element_size = element_size;
    array->capacity =
        ST_ARRAY_INITIAL_CAPACITY;

    array->data = malloc(
        (size_t)array->capacity *
        (size_t)element_size
    );

    if (array->data == NULL)
    {
        free(array);
        return NULL;
    }

    return array;
}


static int st_array_grow(
    STArray* array
)
{
    void* new_data;
    int new_capacity;

    if (array == NULL)
        return 0;

    new_capacity =
        array->capacity * 2;

    new_data = realloc(
        array->data,
        (size_t)new_capacity *
        (size_t)array->element_size
    );

    if (new_data == NULL)
        return 0;

    array->data = new_data;
    array->capacity = new_capacity;

    return 1;
}


int st_array_push(
    STArray* array,
    const void* element
)
{
    void* destination;

    if (
        array == NULL ||
        element == NULL
    )
    {
        return 0;
    }

    if (
        array->count >= array->capacity
    )
    {
        if (!st_array_grow(array))
            return 0;
    }

    destination =
        (char*)array->data +
        (
            (size_t)array->count *
            (size_t)array->element_size
        );

    memcpy(
        destination,
        element,
        (size_t)array->element_size
    );

    array->count++;

    return 1;
}


void* st_array_get(
    STArray* array,
    int index
)
{
    if (array == NULL)
        return NULL;

    if (
        index < 0 ||
        index >= array->count
    )
    {
        return NULL;
    }

    return (
        (char*)array->data +
        (
            (size_t)index *
            (size_t)array->element_size
        )
    );
}


int st_array_set(
    STArray* array,
    int index,
    const void* element
)
{
    void* destination;

    if (
        array == NULL ||
        element == NULL
    )
    {
        return 0;
    }

    if (
        index < 0 ||
        index >= array->count
    )
    {
        return 0;
    }

    destination =
        (char*)array->data +
        (
            (size_t)index *
            (size_t)array->element_size
        );

    memcpy(
        destination,
        element,
        (size_t)array->element_size
    );

    return 1;
}


int st_array_remove(
    STArray* array,
    int index
)
{
    char* data;
    size_t move_size;

    if (array == NULL)
        return 0;

    if (
        index < 0 ||
        index >= array->count
    )
    {
        return 0;
    }

    data = (char*)array->data;

    move_size =
        (size_t)(
            array->count - index - 1
        ) *
        (size_t)array->element_size;

    if (move_size > 0)
    {
        memmove(
            data +
            (
                (size_t)index *
                (size_t)array->element_size
            ),

            data +
            (
                (size_t)(index + 1) *
                (size_t)array->element_size
            ),

            move_size
        );
    }

    array->count--;

    return 1;
}


int st_array_count(
    const STArray* array
)
{
    if (array == NULL)
        return 0;

    return array->count;
}


void st_array_clear(
    STArray* array
)
{
    if (array == NULL)
        return;

    array->count = 0;
}


void st_array_free(
    STArray* array
)
{
    if (array == NULL)
        return;

    free(array->data);
    array->data = NULL;

    free(array);
}