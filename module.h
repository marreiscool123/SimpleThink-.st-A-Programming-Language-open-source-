#ifndef ST_MODULE_H
#define ST_MODULE_H

#define ST_MODULE_MAX_NAME 128
#define ST_MODULE_MAX_PATH 512

typedef struct STModule
{
    char name[ST_MODULE_MAX_NAME];
    char path[ST_MODULE_MAX_PATH];
    int loaded;
} STModule;

typedef struct STModuleTable
{
    STModule* modules;
    int count;
    int capacity;
} STModuleTable;

void st_modules_init(STModuleTable* table);
void st_modules_free(STModuleTable* table);

STModule* st_module_create(
    STModuleTable* table,
    const char* name,
    const char* path
);

STModule* st_module_find(
    STModuleTable* table,
    const char* name
);

int st_module_load(STModule* module);
int st_module_unload(STModule* module);

int st_module_is_loaded(const STModule* module);

const char* st_module_name(const STModule* module);
const char* st_module_path(const STModule* module);

#endif