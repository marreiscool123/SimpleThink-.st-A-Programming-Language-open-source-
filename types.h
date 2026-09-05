#ifndef ST_TYPES_H
#define ST_TYPES_H

typedef enum STType
{
    ST_TYPE_VOID = 0,

    ST_TYPE_INT,
    ST_TYPE_FLOAT,
    ST_TYPE_STRING,
    ST_TYPE_BOOL,
    ST_TYPE_CHAR,

    ST_TYPE_ARRAY,
    ST_TYPE_STRUCT

} STType;


/*
 * Get the name of a type.
 */
const char* st_type_name(
    STType type
);


/*
 * Get the size of a basic type in bytes.
 *
 * Returns 0 for types whose size is
 * determined dynamically.
 */
int st_type_size(
    STType type
);


/*
 * Check whether a type is numeric.
 */
int st_type_is_numeric(
    STType type
);


/*
 * Check whether a type is integral.
 */
int st_type_is_integer(
    STType type
);


/*
 * Check whether a type is a basic built-in type.
 */
int st_type_is_builtin(
    STType type
);


/*
 * Check whether two types are compatible.
 */
int st_type_compatible(
    STType left,
    STType right
);


/*
 * Check whether a value can be converted
 * from one type to another.
 */
int st_type_can_convert(
    STType from,
    STType to
);


/*
 * Convert a type name into an STType.
 *
 * Example:
 *
 * "int" -> ST_TYPE_INT
 */
int st_type_from_name(
    const char* name,
    STType* type
);

#endif /* ST_TYPES_H */