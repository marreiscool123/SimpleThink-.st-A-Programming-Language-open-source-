#ifndef ST_PROJECT_H
#define ST_PROJECT_H

#include "module.h"
#include "imports.h"

#define ST_PROJECT_MAX_FILES 256
#define ST_PROJECT_MAX_NAME 128
#define ST_PROJECT_MAX_PATH 512

typedef struct STProjectFile
{
    char name[ST_PROJECT_MAX_NAME];
    char path[ST_PROJECT_MAX_PATH];
    int loaded;
} STProjectFile;

typedef struct STProject
{
    char name[ST_PROJECT_MAX_NAME];
    char root_path[ST_PROJECT_MAX_PATH];

    STProjectFile files[ST_PROJECT_MAX_FILES];
    int file_count;

    STModuleTable modules;
    STImportTable imports;

    int initialized;
} STProject;

int st_project_init(
    STProject* project,
    const char* name,
    const char* root_path
);

void st_project_shutdown(STProject* project);

int st_project_add_file(
    STProject* project,
    const char* name,
    const char* path
);

STProjectFile* st_project_find_file(
    STProject* project,
    const char* name
);

int st_project_add_module(
    STProject* project,
    const char* name,
    const char* path
);

STModule* st_project_find_module(
    STProject* project,
    const char* name
);

int st_project_add_import(
    STProject* project,
    const char* module_name,
    const char* path
);

int st_project_resolve_imports(STProject* project);

int st_project_file_count(const STProject* project);
int st_project_module_count(const STProject* project);
int st_project_import_count(const STProject* project);

const char* st_project_name(const STProject* project);
const char* st_project_root(const STProject* project);

#endif