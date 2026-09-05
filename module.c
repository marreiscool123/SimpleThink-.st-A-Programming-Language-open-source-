#include "module.h"

#include <stdlib.h>
#include <string.h>

void st_modules_init(STModuleTable* table)
{
    if (table == NULL)
        return;

    table->modules = NULL;
    table->count = 0;
    table->capacity = 0;
}

void st_modules_free(STModuleTable* table)
{
    if (table == NULL)
        return;

    free(table->modules);

    table->modules = NULL;
    table->count = 0;
    table->capacity = 0;
}

STModule* st_module_create(
    STModuleTable* table,
    const char* name,
    const char* path
)
{
    STModule* module;

    if (table == NULL || name == NULL)
        return NULL;

    if (table->count >= table->capacity)
    {
        int new_capacity;
        STModule* new_modules;

        new_capacity = table->capacity == 0
            ? 16
            : table->capacity * 2;

        new_modules = (STModule*)realloc(
            table->modules,
            sizeof(STModule) * new_capacity
        );

        if (new_modules == NULL)
            return NULL;

        table->modules = new_modules;
        table->capacity = new_capacity;
    }

    module = &table->modules[table->count];

    memset(module, 0, sizeof(STModule));

    strncpy(
        module->name,
        name,
        ST_MODULE_MAX_NAME - 1
    );

    module->name[ST_MODULE_MAX_NAME - 1] = '\0';

    if (path != NULL)
    {
        strncpy(
            module->path,
            path,
            ST_MODULE_MAX_PATH - 1
        );

        module->path[ST_MODULE_MAX_PATH - 1] = '\0';
    }

    module->loaded = 0;

    table->count++;

    return module;
}

STModule* st_module_find(
    STModuleTable* table,
    const char* name
)
{
    int i;

    if (table == NULL || name == NULL)
        return NULL;

    for (i = 0; i < table->count; i++)
    {
        if (strcmp(table->modules[i].name, name) == 0)
            return &table->modules[i];
    }

    return NULL;
}

int st_module_load(STModule* module)
{
    if (module == NULL)
        return 0;

    module->loaded = 1;

    return 1;
}

int st_module_unload(STModule* module)
{
    if (module == NULL)
        return 0;

    module->loaded = 0;

    return 1;
}

int st_module_is_loaded(const STModule* module)
{
    if (module == NULL)
        return 0;

    return module->loaded;
}

const char* st_module_name(const STModule* module)
{
    if (module == NULL)
        return NULL;

    return module->name;
}

const char* st_module_path(const STModule* module)
{
    if (module == NULL)
        return NULL;

    return module->path;
}