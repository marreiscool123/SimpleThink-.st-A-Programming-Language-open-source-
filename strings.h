#ifndef ST_STRINGS_H
#define ST_STRINGS_H

typedef struct STString
{
    char* data;
    int length;
    int capacity;

} STString;


/*
 * Create an empty string.
 */
STString* st_string_create(void);


/*
 * Create a string from text.
 */
STString* st_string_create_from(
    const char* text
);


/*
 * Set string contents.
 */
int st_string_set(
    STString* string,
    const char* text
);


/*
 * Append text.
 */
int st_string_append(
    STString* string,
    const char* text
);


/*
 * Append one character.
 */
int st_string_append_char(
    STString* string,
    char character
);


/*
 * Get string length.
 */
int st_string_length(
    const STString* string
);


/*
 * Get string data.
 */
const char* st_string_data(
    const STString* string
);


/*
 * Compare two strings.
 */
int st_string_equals(
    const STString* left,
    const STString* right
);


/*
 * Find text inside a string.
 *
 * Returns the index or -1.
 */
int st_string_find(
    const STString* string,
    const char* text
);


/*
 * Clear the string.
 */
void st_string_clear(
    STString* string
);


/*
 * Free the string.
 */
void st_string_free(
    STString* string
);

#endif /* ST_STRINGS_H */