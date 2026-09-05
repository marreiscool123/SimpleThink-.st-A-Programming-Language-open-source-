#include "operators.h"

#include <string.h>


const char* st_operator_symbol(
    STOperatorType type
)
{
    switch (type)
    {
        case ST_OPERATOR_ADD:
            return "+";

        case ST_OPERATOR_SUBTRACT:
            return "-";

        case ST_OPERATOR_MULTIPLY:
            return "*";

        case ST_OPERATOR_DIVIDE:
            return "/";

        case ST_OPERATOR_MODULO:
            return "%";

        case ST_OPERATOR_EQUAL:
            return "==";

        case ST_OPERATOR_NOT_EQUAL:
            return "!=";

        case ST_OPERATOR_LESS:
            return "<";

        case ST_OPERATOR_LESS_EQUAL:
            return "<=";

        case ST_OPERATOR_GREATER:
            return ">";

        case ST_OPERATOR_GREATER_EQUAL:
            return ">=";

        case ST_OPERATOR_AND:
            return "&&";

        case ST_OPERATOR_OR:
            return "||";

        case ST_OPERATOR_NOT:
            return "!";

        case ST_OPERATOR_ASSIGN:
            return "=";

        case ST_OPERATOR_ADD_ASSIGN:
            return "+=";

        case ST_OPERATOR_SUB_ASSIGN:
            return "-=";

        case ST_OPERATOR_MUL_ASSIGN:
            return "*=";

        case ST_OPERATOR_DIV_ASSIGN:
            return "/=";

        case ST_OPERATOR_MOD_ASSIGN:
            return "%=";

        case ST_OPERATOR_INCREMENT:
            return "++";

        case ST_OPERATOR_DECREMENT:
            return "--";

        case ST_OPERATOR_BIT_AND:
            return "&";

        case ST_OPERATOR_BIT_OR:
            return "|";

        case ST_OPERATOR_BIT_XOR:
            return "^";

        case ST_OPERATOR_BIT_NOT:
            return "~";

        case ST_OPERATOR_SHIFT_LEFT:
            return "<<";

        case ST_OPERATOR_SHIFT_RIGHT:
            return ">>";

        default:
            return "?";
    }
}


const char* st_operator_name(
    STOperatorType type
)
{
    switch (type)
    {
        case ST_OPERATOR_ADD:
            return "addition";

        case ST_OPERATOR_SUBTRACT:
            return "subtraction";

        case ST_OPERATOR_MULTIPLY:
            return "multiplication";

        case ST_OPERATOR_DIVIDE:
            return "division";

        case ST_OPERATOR_MODULO:
            return "modulo";

        case ST_OPERATOR_EQUAL:
            return "equal";

        case ST_OPERATOR_NOT_EQUAL:
            return "not equal";

        case ST_OPERATOR_LESS:
            return "less than";

        case ST_OPERATOR_LESS_EQUAL:
            return "less than or equal";

        case ST_OPERATOR_GREATER:
            return "greater than";

        case ST_OPERATOR_GREATER_EQUAL:
            return "greater than or equal";

        case ST_OPERATOR_AND:
            return "logical and";

        case ST_OPERATOR_OR:
            return "logical or";

        case ST_OPERATOR_NOT:
            return "logical not";

        case ST_OPERATOR_ASSIGN:
            return "assignment";

        case ST_OPERATOR_ADD_ASSIGN:
            return "addition assignment";

        case ST_OPERATOR_SUB_ASSIGN:
            return "subtraction assignment";

        case ST_OPERATOR_MUL_ASSIGN:
            return "multiplication assignment";

        case ST_OPERATOR_DIV_ASSIGN:
            return "division assignment";

        case ST_OPERATOR_MOD_ASSIGN:
            return "modulo assignment";

        case ST_OPERATOR_INCREMENT:
            return "increment";

        case ST_OPERATOR_DECREMENT:
            return "decrement";

        case ST_OPERATOR_BIT_AND:
            return "bitwise and";

        case ST_OPERATOR_BIT_OR:
            return "bitwise or";

        case ST_OPERATOR_BIT_XOR:
            return "bitwise xor";

        case ST_OPERATOR_BIT_NOT:
            return "bitwise not";

        case ST_OPERATOR_SHIFT_LEFT:
            return "shift left";

        case ST_OPERATOR_SHIFT_RIGHT:
            return "shift right";

        default:
            return "unknown";
    }
}


int st_operator_precedence(
    STOperatorType type
)
{
    switch (type)
    {
        case ST_OPERATOR_ASSIGN:
        case ST_OPERATOR_ADD_ASSIGN:
        case ST_OPERATOR_SUB_ASSIGN:
        case ST_OPERATOR_MUL_ASSIGN:
        case ST_OPERATOR_DIV_ASSIGN:
        case ST_OPERATOR_MOD_ASSIGN:
            return 1;

        case ST_OPERATOR_OR:
            return 2;

        case ST_OPERATOR_AND:
            return 3;

        case ST_OPERATOR_EQUAL:
        case ST_OPERATOR_NOT_EQUAL:
            return 4;

        case ST_OPERATOR_LESS:
        case ST_OPERATOR_LESS_EQUAL:
        case ST_OPERATOR_GREATER:
        case ST_OPERATOR_GREATER_EQUAL:
            return 5;

        case ST_OPERATOR_BIT_OR:
            return 6;

        case ST_OPERATOR_BIT_XOR:
            return 7;

        case ST_OPERATOR_BIT_AND:
            return 8;

        case ST_OPERATOR_SHIFT_LEFT:
        case ST_OPERATOR_SHIFT_RIGHT:
            return 9;

        case ST_OPERATOR_ADD:
        case ST_OPERATOR_SUBTRACT:
            return 10;

        case ST_OPERATOR_MULTIPLY:
        case ST_OPERATOR_DIVIDE:
        case ST_OPERATOR_MODULO:
            return 11;

        case ST_OPERATOR_NOT:
        case ST_OPERATOR_BIT_NOT:
        case ST_OPERATOR_INCREMENT:
        case ST_OPERATOR_DECREMENT:
            return 12;

        default:
            return 0;
    }
}


int st_operator_is_unary(
    STOperatorType type
)
{
    switch (type)
    {
        case ST_OPERATOR_NOT:
        case ST_OPERATOR_BIT_NOT:
        case ST_OPERATOR_INCREMENT:
        case ST_OPERATOR_DECREMENT:
            return 1;

        default:
            return 0;
    }
}


int st_operator_is_binary(
    STOperatorType type
)
{
    switch (type)
    {
        case ST_OPERATOR_ADD:
        case ST_OPERATOR_SUBTRACT:
        case ST_OPERATOR_MULTIPLY:
        case ST_OPERATOR_DIVIDE:
        case ST_OPERATOR_MODULO:

        case ST_OPERATOR_EQUAL:
        case ST_OPERATOR_NOT_EQUAL:
        case ST_OPERATOR_LESS:
        case ST_OPERATOR_LESS_EQUAL:
        case ST_OPERATOR_GREATER:
        case ST_OPERATOR_GREATER_EQUAL:

        case ST_OPERATOR_AND:
        case ST_OPERATOR_OR:

        case ST_OPERATOR_ASSIGN:
        case ST_OPERATOR_ADD_ASSIGN:
        case ST_OPERATOR_SUB_ASSIGN:
        case ST_OPERATOR_MUL_ASSIGN:
        case ST_OPERATOR_DIV_ASSIGN:
        case ST_OPERATOR_MOD_ASSIGN:

        case ST_OPERATOR_BIT_AND:
        case ST_OPERATOR_BIT_OR:
        case ST_OPERATOR_BIT_XOR:

        case ST_OPERATOR_SHIFT_LEFT:
        case ST_OPERATOR_SHIFT_RIGHT:
            return 1;

        default:
            return 0;
    }
}


int st_operator_is_assignment(
    STOperatorType type
)
{
    switch (type)
    {
        case ST_OPERATOR_ASSIGN:
        case ST_OPERATOR_ADD_ASSIGN:
        case ST_OPERATOR_SUB_ASSIGN:
        case ST_OPERATOR_MUL_ASSIGN:
        case ST_OPERATOR_DIV_ASSIGN:
        case ST_OPERATOR_MOD_ASSIGN:
            return 1;

        default:
            return 0;
    }
}


int st_operator_from_symbol(
    const char* symbol,
    STOperatorType* type
)
{
    if (
        symbol == NULL ||
        type == NULL
    )
    {
        return 0;
    }


    if (strcmp(symbol, "+") == 0)
    {
        *type = ST_OPERATOR_ADD;
        return 1;
    }

    if (strcmp(symbol, "-") == 0)
    {
        *type = ST_OPERATOR_SUBTRACT;
        return 1;
    }

    if (strcmp(symbol, "*") == 0)
    {
        *type = ST_OPERATOR_MULTIPLY;
        return 1;
    }

    if (strcmp(symbol, "/") == 0)
    {
        *type = ST_OPERATOR_DIVIDE;
        return 1;
    }

    if (strcmp(symbol, "%") == 0)
    {
        *type = ST_OPERATOR_MODULO;
        return 1;
    }


    if (strcmp(symbol, "==") == 0)
    {
        *type = ST_OPERATOR_EQUAL;
        return 1;
    }

    if (strcmp(symbol, "!=") == 0)
    {
        *type = ST_OPERATOR_NOT_EQUAL;
        return 1;
    }

    if (strcmp(symbol, "<") == 0)
    {
        *type = ST_OPERATOR_LESS;
        return 1;
    }

    if (strcmp(symbol, "<=") == 0)
    {
        *type = ST_OPERATOR_LESS_EQUAL;
        return 1;
    }

    if (strcmp(symbol, ">") == 0)
    {
        *type = ST_OPERATOR_GREATER;
        return 1;
    }

    if (strcmp(symbol, ">=") == 0)
    {
        *type = ST_OPERATOR_GREATER_EQUAL;
        return 1;
    }


    if (strcmp(symbol, "&&") == 0)
    {
        *type = ST_OPERATOR_AND;
        return 1;
    }

    if (strcmp(symbol, "||") == 0)
    {
        *type = ST_OPERATOR_OR;
        return 1;
    }

    if (strcmp(symbol, "!") == 0)
    {
        *type = ST_OPERATOR_NOT;
        return 1;
    }


    if (strcmp(symbol, "=") == 0)
    {
        *type = ST_OPERATOR_ASSIGN;
        return 1;
    }

    if (strcmp(symbol, "+=") == 0)
    {
        *type = ST_OPERATOR_ADD_ASSIGN;
        return 1;
    }

    if (strcmp(symbol, "-=") == 0)
    {
        *type = ST_OPERATOR_SUB_ASSIGN;
        return 1;
    }

    if (strcmp(symbol, "*=") == 0)
    {
        *type = ST_OPERATOR_MUL_ASSIGN;
        return 1;
    }

    if (strcmp(symbol, "/=") == 0)
    {
        *type = ST_OPERATOR_DIV_ASSIGN;
        return 1;
    }

    if (strcmp(symbol, "%=") == 0)
    {
        *type = ST_OPERATOR_MOD_ASSIGN;
        return 1;
    }


    if (strcmp(symbol, "++") == 0)
    {
        *type = ST_OPERATOR_INCREMENT;
        return 1;
    }

    if (strcmp(symbol, "--") == 0)
    {
        *type = ST_OPERATOR_DECREMENT;
        return 1;
    }


    if (strcmp(symbol, "&") == 0)
    {
        *type = ST_OPERATOR_BIT_AND;
        return 1;
    }

    if (strcmp(symbol, "|") == 0)
    {
        *type = ST_OPERATOR_BIT_OR;
        return 1;
    }

    if (strcmp(symbol, "^") == 0)
    {
        *type = ST_OPERATOR_BIT_XOR;
        return 1;
    }

    if (strcmp(symbol, "~") == 0)
    {
        *type = ST_OPERATOR_BIT_NOT;
        return 1;
    }

    if (strcmp(symbol, "<<") == 0)
    {
        *type = ST_OPERATOR_SHIFT_LEFT;
        return 1;
    }

    if (strcmp(symbol, ">>") == 0)
    {
        *type = ST_OPERATOR_SHIFT_RIGHT;
        return 1;
    }


    return 0;
}