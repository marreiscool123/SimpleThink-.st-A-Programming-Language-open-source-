#ifndef ST_OPERATORS_H
#define ST_OPERATORS_H

typedef enum STOperatorType
{
    ST_OPERATOR_ADD,
    ST_OPERATOR_SUBTRACT,
    ST_OPERATOR_MULTIPLY,
    ST_OPERATOR_DIVIDE,
    ST_OPERATOR_MODULO,

    ST_OPERATOR_EQUAL,
    ST_OPERATOR_NOT_EQUAL,
    ST_OPERATOR_LESS,
    ST_OPERATOR_LESS_EQUAL,
    ST_OPERATOR_GREATER,
    ST_OPERATOR_GREATER_EQUAL,

    ST_OPERATOR_AND,
    ST_OPERATOR_OR,
    ST_OPERATOR_NOT,

    ST_OPERATOR_ASSIGN,
    ST_OPERATOR_ADD_ASSIGN,
    ST_OPERATOR_SUB_ASSIGN,
    ST_OPERATOR_MUL_ASSIGN,
    ST_OPERATOR_DIV_ASSIGN,
    ST_OPERATOR_MOD_ASSIGN,

    ST_OPERATOR_INCREMENT,
    ST_OPERATOR_DECREMENT,

    ST_OPERATOR_BIT_AND,
    ST_OPERATOR_BIT_OR,
    ST_OPERATOR_BIT_XOR,
    ST_OPERATOR_BIT_NOT,
    ST_OPERATOR_SHIFT_LEFT,
    ST_OPERATOR_SHIFT_RIGHT

} STOperatorType;


/*
 * Get the text representation of an operator.
 */
const char* st_operator_symbol(
    STOperatorType type
);


/*
 * Get the name of an operator.
 */
const char* st_operator_name(
    STOperatorType type
);


/*
 * Get operator precedence.
 * Higher number = evaluated first.
 */
int st_operator_precedence(
    STOperatorType type
);


/*
 * Check whether an operator is unary.
 */
int st_operator_is_unary(
    STOperatorType type
);


/*
 * Check whether an operator is binary.
 */
int st_operator_is_binary(
    STOperatorType type
);


/*
 * Check whether an operator is an assignment operator.
 */
int st_operator_is_assignment(
    STOperatorType type
);


/*
 * Convert a source symbol into an operator.
 *
 * Example:
 *
 * "+"  -> ST_OPERATOR_ADD
 * "==" -> ST_OPERATOR_EQUAL
 * "&&" -> ST_OPERATOR_AND
 */
int st_operator_from_symbol(
    const char* symbol,
    STOperatorType* type
);

#endif /* ST_OPERATORS_H */