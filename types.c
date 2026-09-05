#include "types.h"

#include <string.h>


const char* st_type_name(
    STType type
)
{
    switch (type)
    {
        case ST_TYPE_VOID:
            return "void";

        case ST_TYPE_INT:
            return "int";

        case ST_TYPE_FLOAT:
            return "float";

        case ST_TYPE_STRING:
            return "string";

        case ST_TYPE_BOOL:
            return "bool";

        case ST_TYPE_CHAR:
            return "char";

        case ST_TYPE_ARRAY:
            return "array";

        case ST_TYPE_STRUCT:
            return "struct";

        default:
            return "unknown";
    }
}


int st_type_size(
    STType type
)
{
    switch (type)
    {
        case ST_TYPE_VOID:
            return 0;

        case ST_TYPE_INT:
            return sizeof(int);

        case ST_TYPE_FLOAT:
            return sizeof(float);

        case ST_TYPE_STRING:
            return sizeof(char*);

        case ST_TYPE_BOOL:
            return sizeof(int);

        case ST_TYPE_CHAR:
            return sizeof(char);

        /*
         * Arrays and structs can have
         * variable sizes.
         */
        case ST_TYPE_ARRAY:
        case ST_TYPE_STRUCT:
            return 0;

        default:
            return 0;
    }
}


int st_type_is_numeric(
    STType type
)
{
    switch (type)
    {
        case ST_TYPE_INT:
        case ST_TYPE_FLOAT:
        case ST_TYPE_CHAR:
            return 1;

        default:
            return 0;
    }
}


int st_type_is_integer(
    STType type
)
{
    switch (type)
    {
        case ST_TYPE_INT:
        case ST_TYPE_CHAR:
        case ST_TYPE_BOOL:
            return 1;

        default:
            return 0;
    }
}


int st_type_is_builtin(
    STType type
)
{
    switch (type)
    {
        case ST_TYPE_VOID:
        case ST_TYPE_INT:
        case ST_TYPE_FLOAT:
        case ST_TYPE_STRING:
        case ST_TYPE_BOOL:
        case ST_TYPE_CHAR:
            return 1;

        default:
            return 0;
    }
}


int st_type_compatible(
    STType left,
    STType right
)
{
    if (left == right)
        return 1;

    /*
     * Integers and floats can participate
     * in numeric expressions.
     */
    if (
        st_type_is_numeric(left) &&
        st_type_is_numeric(right)
    )
    {
        return 1;
    }

    return 0;
}


int st_type_can_convert(
    STType from,
    STType to
)
{
    if (from == to)
        return 1;

    /*
     * Numeric conversions.
     */
    if (
        st_type_is_numeric(from) &&
        st_type_is_numeric(to)
    )
    {
        return 1;
    }

    /*
     * Boolean can be represented by an
     * integer value.
     */
    if (
        from == ST_TYPE_BOOL &&
        to == ST_TYPE_INT
    )
    {
        return 1;
    }

    if (
        from == ST_TYPE_INT &&
        to == ST_TYPE_BOOL
    )
    {
        return 1;
    }

    return 0;
}


int st_type_from_name(
    const char* name,
    STType* type
)
{
    if (
        name == NULL ||
        type == NULL
    )
    {
        return 0;
    }


    if (strcmp(name, "void") == 0)
    {
        *type = ST_TYPE_VOID;
        return 1;
    }


    if (strcmp(name, "int") == 0)
    {
        *type = ST_TYPE_INT;
        return 1;
    }


    if (
        strcmp(name, "float") == 0 ||
        strcmp(name, "double") == 0
    )
    {
        *type = ST_TYPE_FLOAT;
        return 1;
    }


    if (
        strcmp(name, "string") == 0
    )
    {
        *type = ST_TYPE_STRING;
        return 1;
    }


    if (
        strcmp(name, "bool") == 0 ||
        strcmp(name, "boolean") == 0
    )
    {
        *type = ST_TYPE_BOOL;
        return 1;
    }


    if (
        strcmp(name, "char") == 0
    )
    {
        *type = ST_TYPE_CHAR;
        return 1;
    }


    if (
        strcmp(name, "array") == 0
    )
    {
        *type = ST_TYPE_ARRAY;
        return 1;
    }


    if (
        strcmp(name, "struct") == 0
    )
    {
        *type = ST_TYPE_STRUCT;
        return 1;
    }


    return 0;
}