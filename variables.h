#ifndef ST_VERIBLES_H
#define ST_VERIBLES_H

/*
 * Simple Think variable system
 *
 * Note:
 * "veribles" is kept as the project filename/name
 * for now, even though the usual spelling is "variables".
 */

typedef enum STVariableType
{
    ST_VARIABLE_INT,
    ST_VARIABLE_FLOAT,
    ST_VARIABLE_STRING,
    ST_VARIABLE_BOOL,
    ST_VARIABLE_CHAR
} STVariableType;


typedef struct STVariable
{
    char* name;

    STVariableType type;

    union
    {
        int int_value;
        float float_value;
        char* string_value;
        int bool_value;
        char char_value;
    } value;

} STVariable;


typedef struct STVariableTable
{
    STVariable* variables;

    int count;
    int capacity;

} STVariableTable;


/*
 * Initialize the variable table.
 */
void st_variables_init(
    STVariableTable* table
);


/*
 * Free the variable table.
 */
void st_variables_free(
    STVariableTable* table
);


/*
 * Create an integer variable.
 */
int st_variable_create_int(
    STVariableTable* table,
    const char* name,
    int value
);


/*
 * Create a floating-point variable.
 */
int st_variable_create_float(
    STVariableTable* table,
    const char* name,
    float value
);


/*
 * Create a string variable.
 */
int st_variable_create_string(
    STVariableTable* table,
    const char* name,
    const char* value
);


/*
 * Create a boolean variable.
 */
int st_variable_create_bool(
    STVariableTable* table,
    const char* name,
    int value
);


/*
 * Create a character variable.
 */
int st_variable_create_char(
    STVariableTable* table,
    const char* name,
    char value
);


/*
 * Find a variable by name.
 */
STVariable* st_variable_find(
    STVariableTable* table,
    const char* name
);


/*
 * Delete a variable.
 */
int st_variable_delete(
    STVariableTable* table,
    const char* name
);


/*
 * Change an integer variable.
 */
int st_variable_set_int(
    STVariableTable* table,
    const char* name,
    int value
);


/*
 * Change a float variable.
 */
int st_variable_set_float(
    STVariableTable* table,
    const char* name,
    float value
);


/*
 * Change a string variable.
 */
int st_variable_set_string(
    STVariableTable* table,
    const char* name,
    const char* value
);


/*
 * Get the type of a variable.
 */
STVariableType st_variable_get_type(
    STVariableTable* table,
    const char* name
);


/*
 * Get the name of a variable type.
 */
const char* st_variable_type_name(
    STVariableType type
);

#endif /* ST_VERIBLES_H */