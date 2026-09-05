#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ST_ERROR_INITIAL_CAPACITY 16


static int st_error_grow(
    STErrorList* list
)
{
    STError* new_errors;
    int new_capacity;

    if (list == NULL)
        return 0;

    new_capacity =
        list->capacity == 0
        ? ST_ERROR_INITIAL_CAPACITY
        : list->capacity * 2;

    new_errors = realloc(
        list->errors,
        sizeof(STError) * new_capacity
    );

    if (new_errors == NULL)
        return 0;

    list->errors = new_errors;
    list->capacity = new_capacity;

    return 1;
}


void st_error_init(
    STErrorList* list
)
{
    if (list == NULL)
        return;

    list->errors = NULL;
    list->count = 0;
    list->capacity = 0;
}


void st_error_free(
    STErrorList* list
)
{
    if (list == NULL)
        return;

    free(list->errors);

    list->errors = NULL;
    list->count = 0;
    list->capacity = 0;
}


void st_error_add(
    STErrorList* list,
    STErrorType type,
    int line,
    int column,
    const char* message
)
{
    STError* error;

    if (list == NULL)
        return;

    if (list->count >= list->capacity)
    {
        if (!st_error_grow(list))
            return;
    }

    error = &list->errors[list->count];

    error->type = type;
    error->line = line;
    error->column = column;

    if (message != NULL)
    {
        strncpy(
            error->message,
            message,
            ST_ERROR_MAX_MESSAGE - 1
        );

        error->message[
            ST_ERROR_MAX_MESSAGE - 1
        ] = '\0';
    }
    else
    {
        error->message[0] = '\0';
    }

    list->count++;
}


const char* st_error_type_name(
    STErrorType type
)
{
    switch (type)
    {
        case ST_ERROR_LEXICAL:
            return "lexical error";

        case ST_ERROR_SYNTAX:
            return "syntax error";

        case ST_ERROR_SEMANTIC:
            return "semantic error";

        case ST_ERROR_COMPILE:
            return "compile error";

        case ST_ERROR_INTERNAL:
            return "internal compiler error";

        default:
            return "error";
    }
}


void st_error_print(
    const STError* error
)
{
    if (error == NULL)
        return;

    if (error->line > 0)
    {
        printf(
            "stc: %d:%d: %s: %s\n",
            error->line,
            error->column,
            st_error_type_name(error->type),
            error->message
        );
    }
    else
    {
        printf(
            "stc: %s: %s\n",
            st_error_type_name(error->type),
            error->message
        );
    }
}


void st_error_print_all(
    const STErrorList* list
)
{
    int i;

    if (list == NULL)
        return;

    for (i = 0; i < list->count; i++)
    {
        st_error_print(&list->errors[i]);
    }
}


int st_error_count(
    const STErrorList* list
)
{
    if (list == NULL)
        return 0;

    return list->count;
}


int st_error_has_errors(
    const STErrorList* list
)
{
    if (list == NULL)
        return 0;

    return list->count > 0;
}


void st_error_clear(
    STErrorList* list
)
{
    if (list == NULL)
        return;

    list->count = 0;
}