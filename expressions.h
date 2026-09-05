#ifndef ST_EXPRESSIONS_H
#define ST_EXPRESSIONS_H

#include "operators.h"

#define ST_EXPRESSION_MAX_CHILDREN 16


typedef enum STExpressionType
{
    ST_EXPRESSION_NONE = 0,

    ST_EXPRESSION_INTEGER,
    ST_EXPRESSION_FLOAT,
    ST_EXPRESSION_STRING,
    ST_EXPRESSION_BOOLEAN,
    ST_EXPRESSION_CHARACTER,

    ST_EXPRESSION_IDENTIFIER,

    ST_EXPRESSION_BINARY,
    ST_EXPRESSION_UNARY,

    ST_EXPRESSION_GROUP

} STExpressionType;


typedef struct STExpression
{
    STExpressionType type;

    int line;
    int column;

    char* value;

    STOperatorType operator;

    struct STExpression* left;
    struct STExpression* right;

    struct STExpression* children[
        ST_EXPRESSION_MAX_CHILDREN
    ];

    int child_count;

} STExpression;


/*
 * Create an expression.
 */
STExpression* st_expression_create(
    STExpressionType type,
    int line,
    int column
);


/*
 * Set the expression value.
 */
int st_expression_set_value(
    STExpression* expression,
    const char* value
);


/*
 * Set an operator.
 */
int st_expression_set_operator(
    STExpression* expression,
    STOperatorType operator
);


/*
 * Set the left and right expressions.
 */
int st_expression_set_binary(
    STExpression* expression,
    STOperatorType operator,
    STExpression* left,
    STExpression* right
);


/*
 * Set a unary expression.
 */
int st_expression_set_unary(
    STExpression* expression,
    STOperatorType operator,
    STExpression* child
);


/*
 * Add a child expression.
 */
int st_expression_add_child(
    STExpression* expression,
    STExpression* child
);


/*
 * Create an integer expression.
 */
STExpression* st_expression_integer(
    int value,
    int line,
    int column
);


/*
 * Create a float expression.
 */
STExpression* st_expression_float(
    float value,
    int line,
    int column
);


/*
 * Create a string expression.
 */
STExpression* st_expression_string(
    const char* value,
    int line,
    int column
);


/*
 * Create an identifier expression.
 */
STExpression* st_expression_identifier(
    const char* name,
    int line,
    int column
);


/*
 * Free an expression tree.
 */
void st_expression_free(
    STExpression* expression
);


/*
 * Get expression type name.
 */
const char* st_expression_type_name(
    STExpressionType type
);

#endif /* ST_EXPRESSIONS_H */