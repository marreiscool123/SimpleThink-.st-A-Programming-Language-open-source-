#ifndef ST_STATEMENTS_H
#define ST_STATEMENTS_H

#include "expressions.h"

#define ST_STATEMENT_MAX_ARGUMENTS 32


typedef enum STStatementType
{
    ST_STATEMENT_NONE = 0,

    ST_STATEMENT_EXPRESSION,
    ST_STATEMENT_VARIABLE_DECLARATION,
    ST_STATEMENT_ASSIGNMENT,

    ST_STATEMENT_FUNCTION_CALL,

    ST_STATEMENT_RETURN,
    ST_STATEMENT_BREAK,
    ST_STATEMENT_CONTINUE,

    ST_STATEMENT_IF,
    ST_STATEMENT_LOOP,
    ST_STATEMENT_BLOCK

} STStatementType;


typedef struct STStatement
{
    STStatementType type;

    int line;
    int column;

    char* name;
    char* value;

    STExpression* expression;

    STExpression* arguments[
        ST_STATEMENT_MAX_ARGUMENTS
    ];

    int argument_count;

    struct STStatement* next;

} STStatement;


/*
 * Create a statement.
 */
STStatement* st_statement_create(
    STStatementType type,
    int line,
    int column
);


/*
 * Set the statement name.
 */
int st_statement_set_name(
    STStatement* statement,
    const char* name
);


/*
 * Set the statement value.
 */
int st_statement_set_value(
    STStatement* statement,
    const char* value
);


/*
 * Set the statement expression.
 */
int st_statement_set_expression(
    STStatement* statement,
    STExpression* expression
);


/*
 * Add a function argument.
 */
int st_statement_add_argument(
    STStatement* statement,
    STExpression* argument
);


/*
 * Connect this statement to the next statement.
 */
int st_statement_set_next(
    STStatement* statement,
    STStatement* next
);


/*
 * Create an expression statement.
 */
STStatement* st_statement_expression(
    STExpression* expression,
    int line,
    int column
);


/*
 * Create a return statement.
 */
STStatement* st_statement_return(
    STExpression* expression,
    int line,
    int column
);


/*
 * Create a break statement.
 */
STStatement* st_statement_break(
    int line,
    int column
);


/*
 * Create a continue statement.
 */
STStatement* st_statement_continue(
    int line,
    int column
);


/*
 * Check whether a statement is a control-flow statement.
 */
int st_statement_is_control_flow(
    STStatementType type
);


/*
 * Get statement type name.
 */
const char* st_statement_type_name(
    STStatementType type
);


/*
 * Free a statement and its children.
 */
void st_statement_free(
    STStatement* statement
);

#endif /* ST_STATEMENTS_H */