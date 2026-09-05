#include "strings.h"

#include <stdlib.h>
#include <string.h>


#define ST_STRING_INITIAL_CAPACITY 32


static int st_string_reserve(
    STString* string,
    int required
)
{
    char* new_data;
    int new_capacity;

    if (string == NULL)
        return 0;

    if (required <= string->capacity)
        return 1;

    new_capacity = string->capacity;

    while (new_capacity < required)
    {
        new_capacity *= 2;
    }

    new_data = realloc(
        string->data,
        (size_t)new_capacity
    );

    if (new_data == NULL)
        return 0;

    string->data = new_data;
    string->capacity = new_capacity;

    return 1;
}


STString* st_string_create(void)
{
    STString* string;

    string = malloc(
        sizeof(STString)
    );

    if (string == NULL)
        return NULL;

    string->data = malloc(
        ST_STRING_INITIAL_CAPACITY
    );

    if (string->data == NULL)
    {
        free(string);
        return NULL;
    }

    string->data[0] = '\0';

    string->length = 0;
    string->capacity =
        ST_STRING_INITIAL_CAPACITY;

    return string;
}


STString* st_string_create_from(
    const char* text
)
{
    STString* string;

    string = st_string_create();

    if (string == NULL)
        return NULL;

    if (!st_string_set(string, text))
    {
        st_string_free(string);
        return NULL;
    }

    return string;
}


int st_string_set(
    STString* string,
    const char* text
)
{
    int length;

    if (string == NULL)
        return 0;

    if (text == NULL)
    {
        st_string_clear(string);
        return 1;
    }

    length = (int)strlen(text);

    if (!st_string_reserve(
        string,
        length + 1
    ))
    {
        return 0;
    }

    memcpy(
        string->data,
        text,
        (size_t)length + 1
    );

    string->length = length;

    return 1;
}


int st_string_append(
    STString* string,
    const char* text
)
{
    int text_length;

    if (
        string == NULL ||
        text == NULL
    )
    {
        return 0;
    }

    text_length = (int)strlen(text);

    if (!st_string_reserve(
        string,
        string->length +
        text_length +
        1
    ))
    {
        return 0;
    }

    memcpy(
        string->data + string->length,
        text,
        (size_t)text_length + 1
    );

    string->length += text_length;

    return 1;
}


int st_string_append_char(
    STString* string,
    char character
)
{
    if (string == NULL)
        return 0;

    if (!st_string_reserve(
        string,
        string->length + 2
    ))
    {
        return 0;
    }

    string->data[
        string->length
    ] = character;

    string->length++;

    string->data[
        string->length
    ] = '\0';

    return 1;
}


int st_string_length(
    const STString* string
)
{
    if (string == NULL)
        return 0;

    return string->length;
}


const char* st_string_data(
    const STString* string
)
{
    if (string == NULL)
        return NULL;

    return string->data;
}


int st_string_equals(
    const STString* left,
    const STString* right
)
{
    if (
        left == NULL ||
        right == NULL
    )
    {
        return 0;
    }

    if (left->length != right->length)
        return 0;

    return strcmp(
        left->data,
        right->data
    ) == 0;
}


int st_string_find(
    const STString* string,
    const char* text
)
{
    const char* found;

    if (
        string == NULL ||
        text == NULL
    )
    {
        return -1;
    }

    found = strstr(
        string->data,
        text
    );

    if (found == NULL)
        return -1;

    return (int)(
        found - string->data
    );
}


void st_string_clear(
    STString* string
)
{
    if (string == NULL)
        return;

    string->length = 0;

    if (string->data != NULL)
        string->data[0] = '\0';
}


void st_string_free(
    STString* string
)
{
    if (string == NULL)
        return;

    free(string->data);
    string->data = NULL;

    free(string);
}