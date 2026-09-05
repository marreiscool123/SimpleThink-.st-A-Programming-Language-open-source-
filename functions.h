#ifndef ST_FUNCTIONS_H
#define ST_FUNCTIONS_H

#define ST_FUNCTION_MAX_NAME 128
#define ST_FUNCTION_MAX_PARAMS 32

typedef enum STFunctionReturnType
{
    ST_FUNCTION_VOID,
    ST_FUNCTION_INT,
    ST_FUNCTION_FLOAT,
    ST_FUNCTION_STRING,
    ST_FUNCTION_BOOL,
    ST_FUNCTION_CHAR
} STFunctionReturnType;


typedef struct STFunctionParameter
{
    char name[ST_FUNCTION_MAX_NAME];
    STFunctionReturnType type;
} STFunctionParameter;


typedef struct STFunction
{
    char name[ST_FUNCTION_MAX_NAME];

    STFunctionReturnType return_type;

    STFunctionParameter parameters[
        ST_FUNCTION_MAX_PARAMS
    ];

    int parameter_count;

    char* body;
    int body_size;

} STFunction;


typedef struct STFunctionTable
{
    STFunction* functions;

    int count;
    int capacity;

} STFunctionTable;


/*
 * Initialize the function table.
 */
void st_functions_init(
    STFunctionTable* table
);


/*
 * Free all functions.
 */
void st_functions_free(
    STFunctionTable* table
);


/*
 * Create a function.
 */
STFunction* st_function_create(
    STFunctionTable* table,
    const char* name,
    STFunctionReturnType return_type
);


/*
 * Find a function by name.
 */
STFunction* st_function_find(
    STFunctionTable* table,
    const char* name
);


/*
 * Add a parameter to a function.
 */
int st_function_add_parameter(
    STFunction* function,
    const char* name,
    STFunctionReturnType type
);


/*
 * Set the function body.
 */
int st_function_set_body(
    STFunction* function,
    const char* body
);


/*
 * Delete a function.
 */
int st_function_delete(
    STFunctionTable* table,
    const char* name
);


/*
 * Get the name of a return type.
 */
const char* st_function_type_name(
    STFunctionReturnType type
);

#endif /* ST_FUNCTIONS_H */