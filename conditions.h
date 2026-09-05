#ifndef ST_CONDITIONS_H
#define ST_CONDITIONS_H

#define ST_CONDITION_MAX_CASES 64

typedef enum STConditionType
{
    ST_CONDITION_IF,
    ST_CONDITION_ELSE_IF,
    ST_CONDITION_ELSE,
    ST_CONDITION_SWITCH
} STConditionType;


typedef struct STConditionCase
{
    char* value;
    char* body;

    int line;
    int column;

    int is_default;

} STConditionCase;


typedef struct STCondition
{
    STConditionType type;

    int line;
    int column;

    char* condition;
    char* body;

    STConditionCase cases[ST_CONDITION_MAX_CASES];

    int case_count;

} STCondition;


/*
 * Create an if/else-if/else condition.
 */
STCondition* st_condition_create(
    STConditionType type,
    int line,
    int column
);


/*
 * Create a switch condition.
 */
STCondition* st_condition_create_switch(
    int line,
    int column
);


/*
 * Set the condition expression.
 */
int st_condition_set_expression(
    STCondition* condition,
    const char* expression
);


/*
 * Set the body of an if/else condition.
 */
int st_condition_set_body(
    STCondition* condition,
    const char* body
);


/*
 * Add a case to a switch.
 */
int st_condition_add_case(
    STCondition* condition,
    const char* value,
    const char* body,
    int line,
    int column
);


/*
 * Add a default case to a switch.
 */
int st_condition_add_default(
    STCondition* condition,
    const char* body,
    int line,
    int column
);


/*
 * Free a condition.
 */
void st_condition_free(
    STCondition* condition
);


/*
 * Get the condition type name.
 */
const char* st_condition_type_name(
    STConditionType type
);


/*
 * Check whether this condition is a switch.
 */
int st_condition_is_switch(
    const STCondition* condition
);


/*
 * Check whether a switch already has a default.
 */
int st_condition_has_default(
    const STCondition* condition
);

#endif /* ST_CONDITIONS_H */