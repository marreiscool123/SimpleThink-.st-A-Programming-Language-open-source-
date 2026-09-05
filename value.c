#include "value.h"

#include <stdlib.h>
#include <string.h>

void st_value_init(STValue* value)
{
    if (value == NULL)
        return;

    memset(value, 0, sizeof(STValue));

    value->type = ST_TYPE_VOID;
}

STValue st_value_int(int value)
{
    STValue result;

    st_value_init(&result);

    result.type = ST_TYPE_INT;
    result.data.int_value = value;

    return result;
}

STValue st_value_float(float value)
{
    STValue result;

    st_value_init(&result);

    result.type = ST_TYPE_FLOAT;
    result.data.float_value = value;

    return result;
}

STValue st_value_char(char value)
{
    STValue result;

    st_value_init(&result);

    result.type = ST_TYPE_CHAR;
    result.data.char_value = value;

    return result;
}

STValue st_value_bool(int value)
{
    STValue result;

    st_value_init(&result);

    result.type = ST_TYPE_BOOL;
    result.data.bool_value = value ? 1 : 0;

    return result;
}

STValue st_value_string(const char* value)
{
    STValue result;

    st_value_init(&result);

    result.type = ST_TYPE_STRING;

    if (value != NULL)
    {
        size_t length = strlen(value);

        result.data.string_value =
            (char*)malloc(length + 1);

        if (result.data.string_value != NULL)
        {
            memcpy(
                result.data.string_value,
                value,
                length + 1
            );
        }
    }

    return result;
}

STValue st_value_pointer(void* value)
{
    STValue result;

    st_value_init(&result);

    result.type = ST_TYPE_STRUCT;
    result.data.pointer_value = value;

    return result;
}

void st_value_free(STValue* value)
{
    if (value == NULL)
        return;

    if (value->type == ST_TYPE_STRING)
    {
        free(value->data.string_value);
        value->data.string_value = NULL;
    }

    value->type = ST_TYPE_VOID;
}

int st_value_is_truthy(const STValue* value)
{
    if (value == NULL)
        return 0;

    switch (value->type)
    {
        case ST_TYPE_INT:
            return value->data.int_value != 0;

        case ST_TYPE_FLOAT:
            return value->data.float_value != 0.0f;

        case ST_TYPE_BOOL:
            return value->data.bool_value != 0;

        case ST_TYPE_CHAR:
            return value->data.char_value != '\0';

        case ST_TYPE_STRING:
            return value->data.string_value != NULL &&
                   value->data.string_value[0] != '\0';

        case ST_TYPE_STRUCT:
        case ST_TYPE_ARRAY:
            return value->data.pointer_value != NULL;

        default:
            return 0;
    }
}

int st_value_to_int(const STValue* value)
{
    if (value == NULL)
        return 0;

    switch (value->type)
    {
        case ST_TYPE_INT:
            return value->data.int_value;

        case ST_TYPE_FLOAT:
            return (int)value->data.float_value;

        case ST_TYPE_BOOL:
            return value->data.bool_value;

        case ST_TYPE_CHAR:
            return (int)value->data.char_value;

        default:
            return 0;
    }
}

float st_value_to_float(const STValue* value)
{
    if (value == NULL)
        return 0.0f;

    switch (value->type)
    {
        case ST_TYPE_INT:
            return (float)value->data.int_value;

        case ST_TYPE_FLOAT:
            return value->data.float_value;

        case ST_TYPE_BOOL:
            return (float)value->data.bool_value;

        case ST_TYPE_CHAR:
            return (float)value->data.char_value;

        default:
            return 0.0f;
    }
}

const char* st_value_type_name(const STValue* value)
{
    if (value == NULL)
        return "void";

    return st_type_name(value->type);
}