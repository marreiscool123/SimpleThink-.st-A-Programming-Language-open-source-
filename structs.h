#ifndef ST_STRUCTS_H
#define ST_STRUCTS_H

#include "types.h"

#define ST_STRUCT_MAX_FIELDS 64
#define ST_STRUCT_MAX_NAME 128


typedef struct STStructField
{
    char name[ST_STRUCT_MAX_NAME];

    STType type;

    int offset;
    int size;

} STStructField;


typedef struct STStruct
{
    char name[ST_STRUCT_MAX_NAME];

    STStructField fields[
        ST_STRUCT_MAX_FIELDS
    ];

    int field_count;
    int size;

} STStruct;


/*
 * Create a struct definition.
 */
STStruct* st_struct_create(
    const char* name
);


/*
 * Add a field.
 */
int st_struct_add_field(
    STStruct* structure,
    const char* name,
    STType type
);


/*
 * Find a field by name.
 */
STStructField* st_struct_find_field(
    STStruct* structure,
    const char* name
);


/*
 * Get the size of a struct.
 */
int st_struct_size(
    const STStruct* structure
);


/*
 * Get the number of fields.
 */
int st_struct_field_count(
    const STStruct* structure
);


/*
 * Check whether a field exists.
 */
int st_struct_has_field(
    const STStruct* structure,
    const char* name
);


/*
 * Free a struct definition.
 */
void st_struct_free(
    STStruct* structure
);

#endif /* ST_STRUCTS_H */