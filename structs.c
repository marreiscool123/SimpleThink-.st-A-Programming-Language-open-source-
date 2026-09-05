#include "structs.h"

#include <stdlib.h>
#include <string.h>


STStruct* st_struct_create(
    const char* name
)
{
    STStruct* structure;

    if (name == NULL)
        return NULL;

    structure = malloc(
        sizeof(STStruct)
    );

    if (structure == NULL)
        return NULL;

    memset(
        structure,
        0,
        sizeof(STStruct)
    );

    strncpy(
        structure->name,
        name,
        ST_STRUCT_MAX_NAME - 1
    );

    structure->name[
        ST_STRUCT_MAX_NAME - 1
    ] = '\0';

    return structure;
}


STStructField* st_struct_find_field(
    STStruct* structure,
    const char* name
)
{
    int i;

    if (
        structure == NULL ||
        name == NULL
    )
    {
        return NULL;
    }

    for (
        i = 0;
        i < structure->field_count;
        i++
    )
    {
        if (
            strcmp(
                structure->fields[i].name,
                name
            ) == 0
        )
        {
            return &structure->fields[i];
        }
    }

    return NULL;
}


int st_struct_has_field(
    const STStruct* structure,
    const char* name
)
{
    int i;

    if (
        structure == NULL ||
        name == NULL
    )
    {
        return 0;
    }

    for (
        i = 0;
        i < structure->field_count;
        i++
    )
    {
        if (
            strcmp(
                structure->fields[i].name,
                name
            ) == 0
        )
        {
            return 1;
        }
    }

    return 0;
}


int st_struct_add_field(
    STStruct* structure,
    const char* name,
    STType type
)
{
    STStructField* field;

    if (
        structure == NULL ||
        name == NULL
    )
    {
        return 0;
    }

    if (
        structure->field_count
        >= ST_STRUCT_MAX_FIELDS
    )
    {
        return 0;
    }

    if (
        st_struct_has_field(
            structure,
            name
        )
    )
    {
        return 0;
    }

    field =
        &structure->fields[
            structure->field_count
        ];

    memset(
        field,
        0,
        sizeof(STStructField)
    );

    strncpy(
        field->name,
        name,
        ST_STRUCT_MAX_NAME - 1
    );

    field->name[
        ST_STRUCT_MAX_NAME - 1
    ] = '\0';

    field->type = type;

    field->size =
        st_type_size(type);

    /*
     * Fields with dynamic sizes have
     * a size of zero here. They can be
     * handled specially by the runtime.
     */
    field->offset =
        structure->size;

    if (field->size > 0)
    {
        structure->size +=
            field->size;
    }

    structure->field_count++;

    return 1;
}


int st_struct_size(
    const STStruct* structure
)
{
    if (structure == NULL)
        return 0;

    return structure->size;
}


int st_struct_field_count(
    const STStruct* structure
)
{
    if (structure == NULL)
        return 0;

    return structure->field_count;
}


void st_struct_free(
    STStruct* structure
)
{
    if (structure == NULL)
        return;

    free(structure);
}