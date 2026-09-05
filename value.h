#ifndef ST_VALUE_H
#define ST_VALUE_H

#include "types.h"

typedef struct STValue
{
    STType type;

    union
    {
        int int_value;
        float float_value;
        char char_value;
        int bool_value;
        char* string_value;
        void* pointer_value;
    } data;

} STValue;

void st_value_init(STValue* value);

STValue st_value_int(int value);
STValue st_value_float(float value);
STValue st_value_char(char value);
STValue st_value_bool(int value);
STValue st_value_string(const char* value);
STValue st_value_pointer(void* value);

void st_value_free(STValue* value);

int st_value_is_truthy(const STValue* value);

int st_value_to_int(const STValue* value);
float st_value_to_float(const STValue* value);

const char* st_value_type_name(const STValue* value);

#endif