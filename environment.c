#include "environment.h"

#include <stdlib.h>
#include <string.h>

static char* st_environment_copy_string(const char* text)
{
    size_t length;
    char* result;

    if (text == NULL)
        return NULL;

    length = strlen(text);

    result = (char*)malloc(length + 1);

    if (result == NULL)
        return NULL;

    memcpy(result, text, length + 1);

    return result;
}

static int st_environment_find_index(
    STEnvironment* environment,
    const char* name
)
{
    int i;

    if (environment == NULL || name == NULL)
        return -1;

    for (i = 0; i < environment->count; i++)
    {
        if (environment->entries[i].name != NULL &&
            strcmp(environment->entries[i].name, name) == 0)
        {
            return i;
        }
    }

    return -1;
}

void st_environment_init(
    STEnvironment* environment,
    STEnvironment* parent
)
{
    if (environment == NULL)
        return;

    memset(
        environment,
        0,
        sizeof(STEnvironment)
    );

    environment->parent = parent;
}

void st_environment_free(
    STEnvironment* environment
)
{
    int i;

    if (environment == NULL)
        return;

    for (i = 0; i < environment->count; i++)
    {
        free(environment->entries[i].name);
        environment->entries[i].name = NULL;

        st_value_free(
            &environment->entries[i].value
        );
    }

    environment->count = 0;
    environment->parent = NULL;
}

int st_environment_define(
    STEnvironment* environment,
    const char* name,
    STValue value
)
{
    STEnvironmentEntry* entry;

    if (environment == NULL ||
        name == NULL ||
        name[0] == '\0')
    {
        st_value_free(&value);
        return 0;
    }

    if (environment->count >= ST_ENVIRONMENT_MAX_VALUES)
    {
        st_value_free(&value);
        return 0;
    }

    if (st_environment_find_index(environment, name) >= 0)
    {
        st_value_free(&value);
        return 0;
    }

    entry =
        &environment->entries[environment->count];

    entry->name =
        st_environment_copy_string(name);

    if (entry->name == NULL)
    {
        st_value_free(&value);
        return 0;
    }

    entry->value = value;

    environment->count++;

    return 1;
}

int st_environment_set(
    STEnvironment* environment,
    const char* name,
    STValue value
)
{
    int index;

    if (environment == NULL ||
        name == NULL)
    {
        st_value_free(&value);
        return 0;
    }

    index =
        st_environment_find_index(environment, name);

    if (index >= 0)
    {
        st_value_free(
            &environment->entries[index].value
        );

        environment->entries[index].value = value;

        return 1;
    }

    if (environment->parent != NULL)
    {
        return st_environment_set(
            environment->parent,
            name,
            value
        );
    }

    return st_environment_define(
        environment,
        name,
        value
    );
}

STValue* st_environment_get(
    STEnvironment* environment,
    const char* name
)
{
    int index;

    if (environment == NULL ||
        name == NULL)
    {
        return NULL;
    }

    index =
        st_environment_find_index(environment, name);

    if (index >= 0)
    {
        return &environment->entries[index].value;
    }

    if (environment->parent != NULL)
    {
        return st_environment_get(
            environment->parent,
            name
        );
    }

    return NULL;
}

int st_environment_has(
    STEnvironment* environment,
    const char* name
)
{
    return st_environment_get(
        environment,
        name
    ) != NULL;
}

int st_environment_delete(
    STEnvironment* environment,
    const char* name
)
{
    int index;
    int i;

    if (environment == NULL ||
        name == NULL)
    {
        return 0;
    }

    index =
        st_environment_find_index(environment, name);

    if (index < 0)
        return 0;

    free(environment->entries[index].name);
    environment->entries[index].name = NULL;

    st_value_free(
        &environment->entries[index].value
    );

    for (i = index; i < environment->count - 1; i++)
    {
        environment->entries[i] =
            environment->entries[i + 1];
    }

    memset(
        &environment->entries[environment->count - 1],
        0,
        sizeof(STEnvironmentEntry)
    );

    environment->count--;

    return 1;
}

void st_environment_clear(
    STEnvironment* environment
)
{
    STEnvironment* parent;

    if (environment == NULL)
        return;

    parent = environment->parent;

    st_environment_free(environment);

    environment->parent = parent;
}

STEnvironment* st_environment_parent(
    STEnvironment* environment
)
{
    if (environment == NULL)
        return NULL;

    return environment->parent;
}