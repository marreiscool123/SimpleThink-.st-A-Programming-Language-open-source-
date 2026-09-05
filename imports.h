#ifndef ST_IMPORTS_H
#define ST_IMPORTS_H

#include "module.h"

#define ST_IMPORT_MAX 128

typedef struct STImport
{
    char module_name[ST_MODULE_MAX_NAME];
    char path[ST_MODULE_MAX_PATH];
    int resolved;
} STImport;

typedef struct STImportTable
{
    STImport imports[ST_IMPORT_MAX];
    int count;
} STImportTable;

void st_imports_init(STImportTable* table);
void st_imports_clear(STImportTable* table);

int st_import_add(
    STImportTable* table,
    const char* module_name,
    const char* path
);

STImport* st_import_find(
    STImportTable* table,
    const char* module_name
);

int st_import_resolve(
    STImportTable* table,
    STModuleTable* modules
);

int st_import_is_resolved(
    const STImport* import
);

#endif