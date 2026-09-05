#include "functions.h"

#include <stdlib.h>
#include <string.h>


#define ST_FUNCTION_INITIAL_CAPACITY 16


static char* st_function_strdup(
    const char* text
)
{
    char* result;
    size_t length;

    if (text == NULL)
        return NULL;

    length = strlen(text);

    result = malloc(length + 1);

    if (result == NULL)
        return NULL;

    memcpy(result, text, length + 1);

    return result;
}


static int st_functions_grow(
    STFunctionTable* table
)
{
    STFunction* new_functions;
    int new_capacity;

    if (table == NULL)
        return 0;

    if (table->capacity == 0)
        new_capacity = ST_FUNCTION_INITIAL_CAPACITY;
    else
        new_capacity = table->capacity * 2;

    new_functions = realloc(
        table->functions,
        sizeof(STFunction) * new_capacity
    );

    if (new_functions == NULL)
        return 0;

    table->functions = new_functions;
    table->capacity = new_capacity;

    return 1;
}


static void st_function_free(
    STFunction* function
)
{
    if (function == NULL)
        return;

    free(function->body);

    function->body = NULL;
    function->body_size = 0;
}


void st_functions_init(
    STFunctionTable* table
)
{
    if (table == NULL)
        return;

    table->functions = NULL;
    table->count = 0;
    table->capacity = 0;
}


void st_functions_free(
    STFunctionTable* table
)
{
    int i;

    if (table == NULL)
        return;

    for (i = 0; i < table->count; i++)
    {
        st_function_free(
            &table->functions[i]
        );
    }

    free(table->functions);

    table->functions = NULL;
    table->count = 0;
    table->capacity = 0;
}


STFunction* st_function_find(
    STFunctionTable* table,
    const char* name
)
{
    int i;

    if (table == NULL || name == NULL)
        return NULL;

    for (i = 0; i < table->count; i++)
    {
        if (strcmp(
            table->functions[i].name,
            name
        ) == 0)
        {
            return &table->functions[i];
        }
    }

    return NULL;
}


STFunction* st_function_create(
    STFunctionTable* table,
    const char* name,
    STFunctionReturnType return_type
)
{
    STFunction* function;

    if (table == NULL || name == NULL)
        return NULL;

    /* Don't allow duplicate function names. */
    if (st_function_find(table, name) != NULL)
        return NULL;

    if (table->count >= table->capacity)
    {
        if (!st_functions_grow(table))
            return NULL;
    }

    function = &table->functions[table->count];

    memset(
        function,
        0,
        sizeof(STFunction)
    );

    strncpy(
        function->name,
        name,
        ST_FUNCTION_MAX_NAME - 1
    );

    function->name[
        ST_FUNCTION_MAX_NAME - 1
    ] = '\0';

    function->return_type = return_type;

    table->count++;

    return function;
}


int st_function_add_parameter(
    STFunction* function,
    const char* name,
    STFunctionReturnType type
)
{
    STFunctionParameter* parameter;

    if (function == NULL || name == NULL)
        return 0;

    if (
        function->parameter_count
        >= ST_FUNCTION_MAX_PARAMS
    )
    {
        return 0;
    }

    parameter =
        &function->parameters[
            function->parameter_count
        ];

    memset(
        parameter,
        0,
        sizeof(STFunctionParameter)
    );

    strncpy(
        parameter->name,
        name,
        ST_FUNCTION_MAX_NAME - 1
    );

    parameter->name[
        ST_FUNCTION_MAX_NAME - 1
    ] = '\0';

    parameter->type = type;

    function->parameter_count++;

    return 1;
}


int st_function_set_body(
    STFunction* function,
    const char* body
)
{
    char* new_body;

    if (function == NULL)
        return 0;

    new_body = st_function_strdup(body);

    if (body != NULL && new_body == NULL)
        return 0;

    free(function->body);

    function->body = new_body;

    if (body != NULL)
        function->body_size = (int)strlen(body);
    else
        function->body_size = 0;

    return 1;
}


int st_function_delete(
    STFunctionTable* table,
    const char* name
)
{
    int i;

    if (table == NULL || name == NULL)
        return 0;

    for (i = 0; i < table->count; i++)
    {
        if (strcmp(
            table->functions[i].name,
            name
        ) == 0)
        {
            int remaining =
                table->count - i - 1;

            st_function_free(
                &table->functions[i]
            );

            if (remaining > 0)
            {
                memmove(
                    &table->functions[i],
                    &table->functions[i + 1],
                    sizeof(STFunction) * remaining
                );
            }

            table->count--;

            return 1;
        }
    }

    return 0;
}


const char* st_function_type_name(
    STFunctionReturnType type
)
{
    switch (type)
    {
        case ST_FUNCTION_VOID:
            return "void";

        case ST_FUNCTION_INT:
            return "int";

        case ST_FUNCTION_FLOAT:
            return "float";

        case ST_FUNCTION_STRING:
            return "string";

        case ST_FUNCTION_BOOL:
            return "bool";

        case ST_FUNCTION_CHAR:
            return "char";

        default:
            return "unknown";
    }
}