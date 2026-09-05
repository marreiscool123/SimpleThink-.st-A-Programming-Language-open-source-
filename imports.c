#include "imports.h"

#include <string.h>

void st_imports_init(STImportTable* table)
{
    if (table == NULL)
        return;

    memset(table, 0, sizeof(STImportTable));
}

void st_imports_clear(STImportTable* table)
{
    if (table == NULL)
        return;

    memset(table, 0, sizeof(STImportTable));
}

int st_import_add(
    STImportTable* table,
    const char* module_name,
    const char* path
)
{
    STImport* import;

    if (table == NULL || module_name == NULL)
        return 0;

    if (table->count >= ST_IMPORT_MAX)
        return 0;

    import = &table->imports[table->count];

    memset(import, 0, sizeof(STImport));

    strncpy(
        import->module_name,
        module_name,
        ST_MODULE_MAX_NAME - 1
    );

    import->module_name[ST_MODULE_MAX_NAME - 1] = '\0';

    if (path != NULL)
    {
        strncpy(
            import->path,
            path,
            ST_MODULE_MAX_PATH - 1
        );

        import->path[ST_MODULE_MAX_PATH - 1] = '\0';
    }

    import->resolved = 0;

    table->count++;

    return 1;
}

STImport* st_import_find(
    STImportTable* table,
    const char* module_name
)
{
    int i;

    if (table == NULL || module_name == NULL)
        return NULL;

    for (i = 0; i < table->count; i++)
    {
        if (strcmp(
            table->imports[i].module_name,
            module_name
        ) == 0)
        {
            return &table->imports[i];
        }
    }

    return NULL;
}

int st_import_resolve(
    STImportTable* table,
    STModuleTable* modules
)
{
    int i;
    int resolved_count = 0;

    if (table == NULL || modules == NULL)
        return 0;

    for (i = 0; i < table->count; i++)
    {
        STModule* module;

        module = st_module_find(
            modules,
            table->imports[i].module_name
        );

        if (module != NULL)
        {
            table->imports[i].resolved = 1;

            if (table->imports[i].path[0] == '\0')
            {
                strncpy(
                    table->imports[i].path,
                    module->path,
                    ST_MODULE_MAX_PATH - 1
                );

                table->imports[i].path[
                    ST_MODULE_MAX_PATH - 1
                ] = '\0';
            }

            resolved_count++;
        }
        else
        {
            table->imports[i].resolved = 0;
        }
    }

    return resolved_count;
}

int st_import_is_resolved(
    const STImport* import
)
{
    if (import == NULL)
        return 0;

    return import->resolved;
}