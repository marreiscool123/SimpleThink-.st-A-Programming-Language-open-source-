#include "variables.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ST_VARIABLE_INITIAL_CAPACITY 16


static char* st_variable_strdup(
    const char* text
)
{
    char* copy;
    size_t length;

    if (text == NULL)
        return NULL;

    length = strlen(text);

    copy = malloc(length + 1);

    if (copy == NULL)
        return NULL;

    memcpy(copy, text, length + 1);

    return copy;
}


static int st_variables_grow(
    STVariableTable* table
)
{
    STVariable* new_variables;
    int new_capacity;

    if (table == NULL)
        return 0;

    if (table->capacity == 0)
        new_capacity = ST_VARIABLE_INITIAL_CAPACITY;
    else
        new_capacity = table->capacity * 2;

    new_variables = realloc(
        table->variables,
        sizeof(STVariable) * new_capacity
    );

    if (new_variables == NULL)
        return 0;

    table->variables = new_variables;
    table->capacity = new_capacity;

    return 1;
}


static void st_variable_free(
    STVariable* variable
)
{
    if (variable == NULL)
        return;

    free(variable->name);
    variable->name = NULL;

    if (variable->type == ST_VARIABLE_STRING)
    {
        free(variable->value.string_value);
        variable->value.string_value = NULL;
    }
}


void st_variables_init(
    STVariableTable* table
)
{
    if (table == NULL)
        return;

    table->variables = NULL;
    table->count = 0;
    table->capacity = 0;
}


void st_variables_free(
    STVariableTable* table
)
{
    int i;

    if (table == NULL)
        return;

    for (i = 0; i < table->count; i++)
    {
        st_variable_free(
            &table->variables[i]
        );
    }

    free(table->variables);

    table->variables = NULL;
    table->count = 0;
    table->capacity = 0;
}


STVariable* st_variable_find(
    STVariableTable* table,
    const char* name
)
{
    int i;

    if (table == NULL || name == NULL)
        return NULL;

    for (i = 0; i < table->count; i++)
    {
        if (strcmp(
            table->variables[i].name,
            name
        ) == 0)
        {
            return &table->variables[i];
        }
    }

    return NULL;
}


static STVariable* st_variable_create(
    STVariableTable* table,
    const char* name,
    STVariableType type
)
{
    STVariable* variable;

    if (table == NULL || name == NULL)
        return NULL;

    /*
     * Don't allow duplicate variable names.
     */
    if (st_variable_find(table, name) != NULL)
        return NULL;

    if (table->count >= table->capacity)
    {
        if (!st_variables_grow(table))
            return NULL;
    }

    variable = &table->variables[table->count];

    memset(
        variable,
        0,
        sizeof(STVariable)
    );

    variable->name = st_variable_strdup(name);

    if (variable->name == NULL)
        return NULL;

    variable->type = type;

    table->count++;

    return variable;
}


int st_variable_create_int(
    STVariableTable* table,
    const char* name,
    int value
)
{
    STVariable* variable;

    variable = st_variable_create(
        table,
        name,
        ST_VARIABLE_INT
    );

    if (variable == NULL)
        return 0;

    variable->value.int_value = value;

    return 1;
}


int st_variable_create_float(
    STVariableTable* table,
    const char* name,
    float value
)
{
    STVariable* variable;

    variable = st_variable_create(
        table,
        name,
        ST_VARIABLE_FLOAT
    );

    if (variable == NULL)
        return 0;

    variable->value.float_value = value;

    return 1;
}


int st_variable_create_string(
    STVariableTable* table,
    const char* name,
    const char* value
)
{
    STVariable* variable;

    variable = st_variable_create(
        table,
        name,
        ST_VARIABLE_STRING
    );

    if (variable == NULL)
        return 0;

    variable->value.string_value =
        st_variable_strdup(value);

    if (value != NULL &&
        variable->value.string_value == NULL)
    {
        st_variable_free(variable);
        table->count--;

        return 0;
    }

    return 1;
}


int st_variable_create_bool(
    STVariableTable* table,
    const char* name,
    int value
)
{
    STVariable* variable;

    variable = st_variable_create(
        table,
        name,
        ST_VARIABLE_BOOL
    );

    if (variable == NULL)
        return 0;

    variable->value.bool_value =
        value ? 1 : 0;

    return 1;
}


int st_variable_create_char(
    STVariableTable* table,
    const char* name,
    char value
)
{
    STVariable* variable;

    variable = st_variable_create(
        table,
        name,
        ST_VARIABLE_CHAR
    );

    if (variable == NULL)
        return 0;

    variable->value.char_value = value;

    return 1;
}


int st_variable_set_int(
    STVariableTable* table,
    const char* name,
    int value
)
{
    STVariable* variable;

    variable = st_variable_find(
        table,
        name
    );

    if (variable == NULL)
        return 0;

    if (variable->type != ST_VARIABLE_INT)
        return 0;

    variable->value.int_value = value;

    return 1;
}


int st_variable_set_float(
    STVariableTable* table,
    const char* name,
    float value
)
{
    STVariable* variable;

    variable = st_variable_find(
        table,
        name
    );

    if (variable == NULL)
        return 0;

    if (variable->type != ST_VARIABLE_FLOAT)
        return 0;

    variable->value.float_value = value;

    return 1;
}


int st_variable_set_string(
    STVariableTable* table,
    const char* name,
    const char* value
)
{
    STVariable* variable;
    char* new_value;

    variable = st_variable_find(
        table,
        name
    );

    if (variable == NULL)
        return 0;

    if (variable->type != ST_VARIABLE_STRING)
        return 0;

    new_value = st_variable_strdup(value);

    if (value != NULL && new_value == NULL)
        return 0;

    free(variable->value.string_value);

    variable->value.string_value = new_value;

    return 1;
}


int st_variable_delete(
    STVariableTable* table,
    const char* name
)
{
    int i;

    if (table == NULL || name == NULL)
        return 0;

    for (i = 0; i < table->count; i++)
    {
        if (strcmp(
            table->variables[i].name,
            name
        ) == 0)
        {
            int remaining =
                table->count - i - 1;

            st_variable_free(
                &table->variables[i]
            );

            if (remaining > 0)
            {
                memmove(
                    &table->variables[i],
                    &table->variables[i + 1],
                    sizeof(STVariable) * remaining
                );
            }

            table->count--;

            return 1;
        }
    }

    return 0;
}


STVariableType st_variable_get_type(
    STVariableTable* table,
    const char* name
)
{
    STVariable* variable;

    variable = st_variable_find(
        table,
        name
    );

    if (variable == NULL)
        return ST_VARIABLE_INT;

    return variable->type;
}


const char* st_variable_type_name(
    STVariableType type
)
{
    switch (type)
    {
        case ST_VARIABLE_INT:
            return "int";

        case ST_VARIABLE_FLOAT:
            return "float";

        case ST_VARIABLE_STRING:
            return "string";

        case ST_VARIABLE_BOOL:
            return "bool";

        case ST_VARIABLE_CHAR:
            return "char";

        default:
            return "unknown";
    }
}