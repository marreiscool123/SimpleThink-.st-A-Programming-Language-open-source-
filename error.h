#ifndef ST_ERROR_H
#define ST_ERROR_H

#define ST_ERROR_MAX_MESSAGE 512

typedef enum STErrorType {
    ST_ERROR_NONE = 0,

    ST_ERROR_LEXICAL,
    ST_ERROR_SYNTAX,
    ST_ERROR_SEMANTIC,
    ST_ERROR_COMPILE,
    ST_ERROR_INTERNAL
} STErrorType;


typedef struct STError {
    STErrorType type;

    int line;
    int column;

    char message[ST_ERROR_MAX_MESSAGE];
} STError;


typedef struct STErrorList {
    STError* errors;

    int count;
    int capacity;
} STErrorList;


/*
 * Initialize the error system.
 */
void st_error_init(
    STErrorList* list
);


/*
 * Free the error system.
 */
void st_error_free(
    STErrorList* list
);


/*
 * Add an error.
 */
void st_error_add(
    STErrorList* list,
    STErrorType type,
    int line,
    int column,
    const char* message
);


/*
 * Print one error.
 */
void st_error_print(
    const STError* error
);


/*
 * Print every error.
 */
void st_error_print_all(
    const STErrorList* list
);


/*
 * Get the number of errors.
 */
int st_error_count(
    const STErrorList* list
);


/*
 * Check whether compilation has errors.
 */
int st_error_has_errors(
    const STErrorList* list
);


/*
 * Convert an error type to text.
 */
const char* st_error_type_name(
    STErrorType type
);


/*
 * Clear all errors while keeping allocated memory.
 */
void st_error_clear(
    STErrorList* list
);

#endif /* ST_ERROR_H */