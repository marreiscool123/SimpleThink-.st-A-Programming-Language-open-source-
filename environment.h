#ifndef ST_ENVIRONMENT_H
#define ST_ENVIRONMENT_H

#include "value.h"

#define ST_ENVIRONMENT_MAX_VALUES 256

typedef struct STEnvironmentEntry
{
    char* name;
    STValue value;
} STEnvironmentEntry;

typedef struct STEnvironment
{
    STEnvironmentEntry entries[ST_ENVIRONMENT_MAX_VALUES];
    int count;
    struct STEnvironment* parent;
} STEnvironment;

void st_environment_init(
    STEnvironment* environment,
    STEnvironment* parent
);

void st_environment_free(
    STEnvironment* environment
);

int st_environment_define(
    STEnvironment* environment,
    const char* name,
    STValue value
);

int st_environment_set(
    STEnvironment* environment,
    const char* name,
    STValue value
);

STValue* st_environment_get(
    STEnvironment* environment,
    const char* name
);

int st_environment_has(
    STEnvironment* environment,
    const char* name
);

int st_environment_delete(
    STEnvironment* environment,
    const char* name
);

void st_environment_clear(
    STEnvironment* environment
);

STEnvironment* st_environment_parent(
    STEnvironment* environment
);

#endif