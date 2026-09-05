#include "project.h"

#include <string.h>

int st_project_init(
    STProject* project,
    const char* name,
    const char* root_path
)
{
    if (project == NULL)
        return 0;

    memset(project, 0, sizeof(STProject));

    if (name != NULL)
    {
        strncpy(
            project->name,
            name,
            ST_PROJECT_MAX_NAME - 1
        );

        project->name[ST_PROJECT_MAX_NAME - 1] = '\0';
    }

    if (root_path != NULL)
    {
        strncpy(
            project->root_path,
            root_path,
            ST_PROJECT_MAX_PATH - 1
        );

        project->root_path[ST_PROJECT_MAX_PATH - 1] = '\0';
    }

    st_modules_init(&project->modules);
    st_imports_init(&project->imports);

    project->initialized = 1;

    return 1;
}

void st_project_shutdown(STProject* project)
{
    if (project == NULL)
        return;

    st_modules_free(&project->modules);
    st_imports_clear(&project->imports);

    project->file_count = 0;
    project->initialized = 0;
}

int st_project_add_file(
    STProject* project,
    const char* name,
    const char* path
)
{
    STProjectFile* file;

    if (project == NULL ||
        name == NULL ||
        !project->initialized)
    {
        return 0;
    }

    if (project->file_count >= ST_PROJECT_MAX_FILES)
        return 0;

    file = &project->files[project->file_count];

    memset(file, 0, sizeof(STProjectFile));

    strncpy(
        file->name,
        name,
        ST_PROJECT_MAX_NAME - 1
    );

    file->name[ST_PROJECT_MAX_NAME - 1] = '\0';

    if (path != NULL)
    {
        strncpy(
            file->path,
            path,
            ST_PROJECT_MAX_PATH - 1
        );

        file->path[ST_PROJECT_MAX_PATH - 1] = '\0';
    }

    file->loaded = 0;

    project->file_count++;

    return 1;
}

STProjectFile* st_project_find_file(
    STProject* project,
    const char* name
)
{
    int i;

    if (project == NULL || name == NULL)
        return NULL;

    for (i = 0; i < project->file_count; i++)
    {
        if (strcmp(project->files[i].name, name) == 0)
            return &project->files[i];
    }

    return NULL;
}

int st_project_add_module(
    STProject* project,
    const char* name,
    const char* path
)
{
    if (project == NULL || !project->initialized)
        return 0;

    return st_module_create(
        &project->modules,
        name,
        path
    ) != NULL;
}

STModule* st_project_find_module(
    STProject* project,
    const char* name
)
{
    if (project == NULL)
        return NULL;

    return st_module_find(
        &project->modules,
        name
    );
}

int st_project_add_import(
    STProject* project,
    const char* module_name,
    const char* path
)
{
    if (project == NULL || !project->initialized)
        return 0;

    return st_import_add(
        &project->imports,
        module_name,
        path
    );
}

int st_project_resolve_imports(STProject* project)
{
    if (project == NULL || !project->initialized)
        return 0;

    return st_import_resolve(
        &project->imports,
        &project->modules
    );
}

int st_project_file_count(const STProject* project)
{
    if (project == NULL)
        return 0;

    return project->file_count;
}

int st_project_module_count(const STProject* project)
{
    if (project == NULL)
        return 0;

    return project->modules.count;
}

int st_project_import_count(const STProject* project)
{
    if (project == NULL)
        return 0;

    return project->imports.count;
}

const char* st_project_name(const STProject* project)
{
    if (project == NULL)
        return NULL;

    return project->name;
}

const char* st_project_root(const STProject* project)
{
    if (project == NULL)
        return NULL;

    return project->root_path;
}