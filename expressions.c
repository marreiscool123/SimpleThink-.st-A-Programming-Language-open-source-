#include "expressions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char* st_expression_strdup(
    const char* text
)
{
    char* result;
    size_t length;

    if (text == NULL)
        return NULL;

    length = strlen(text);

    result = malloc(length + 1);

    if (result == NULL)
        return NULL;

    memcpy(result, text, length + 1);

    return result;
}


STExpression* st_expression_create(
    STExpressionType type,
    int line,
    int column
)
{
    STExpression* expression;

    expression = malloc(
        sizeof(STExpression)
    );

    if (expression == NULL)
        return NULL;

    memset(
        expression,
        0,
        sizeof(STExpression)
    );

    expression->type = type;
    expression->line = line;
    expression->column = column;
    expression->operator = ST_OPERATOR_ADD;

    return expression;
}


int st_expression_set_value(
    STExpression* expression,
    const char* value
)
{
    char* new_value;

    if (expression == NULL)
        return 0;

    new_value =
        st_expression_strdup(value);

    if (
        value != NULL &&
        new_value == NULL
    )
    {
        return 0;
    }

    free(expression->value);

    expression->value = new_value;

    return 1;
}


int st_expression_set_operator(
    STExpression* expression,
    STOperatorType operator
)
{
    if (expression == NULL)
        return 0;

    expression->operator = operator;

    return 1;
}


int st_expression_set_binary(
    STExpression* expression,
    STOperatorType operator,
    STExpression* left,
    STExpression* right
)
{
    if (expression == NULL)
        return 0;

    expression->type =
        ST_EXPRESSION_BINARY;

    expression->operator = operator;
    expression->left = left;
    expression->right = right;

    return 1;
}


int st_expression_set_unary(
    STExpression* expression,
    STOperatorType operator,
    STExpression* child
)
{
    if (expression == NULL)
        return 0;

    expression->type =
        ST_EXPRESSION_UNARY;

    expression->operator = operator;
    expression->left = child;

    return 1;
}


int st_expression_add_child(
    STExpression* expression,
    STExpression* child
)
{
    if (expression == NULL)
        return 0;

    if (child == NULL)
        return 0;

    if (
        expression->child_count
        >= ST_EXPRESSION_MAX_CHILDREN
    )
    {
        return 0;
    }

    expression->children[
        expression->child_count
    ] = child;

    expression->child_count++;

    return 1;
}


STExpression* st_expression_integer(
    int value,
    int line,
    int column
)
{
    STExpression* expression;
    char buffer[64];

    expression =
        st_expression_create(
            ST_EXPRESSION_INTEGER,
            line,
            column
        );

    if (expression == NULL)
        return NULL;

    snprintf(
        buffer,
        sizeof(buffer),
        "%d",
        value
    );

    if (
        !st_expression_set_value(
            expression,
            buffer
        )
    )
    {
        st_expression_free(expression);
        return NULL;
    }

    return expression;
}


STExpression* st_expression_float(
    float value,
    int line,
    int column
)
{
    STExpression* expression;
    char buffer[64];

    expression =
        st_expression_create(
            ST_EXPRESSION_FLOAT,
            line,
            column
        );

    if (expression == NULL)
        return NULL;

    snprintf(
        buffer,
        sizeof(buffer),
        "%g",
        value
    );

    if (
        !st_expression_set_value(
            expression,
            buffer
        )
    )
    {
        st_expression_free(expression);
        return NULL;
    }

    return expression;
}


STExpression* st_expression_string(
    const char* value,
    int line,
    int column
)
{
    STExpression* expression;

    expression =
        st_expression_create(
            ST_EXPRESSION_STRING,
            line,
            column
        );

    if (expression == NULL)
        return NULL;

    if (
        !st_expression_set_value(
            expression,
            value
        )
    )
    {
        st_expression_free(expression);
        return NULL;
    }

    return expression;
}


STExpression* st_expression_identifier(
    const char* name,
    int line,
    int column
)
{
    STExpression* expression;

    expression =
        st_expression_create(
            ST_EXPRESSION_IDENTIFIER,
            line,
            column
        );

    if (expression == NULL)
        return NULL;

    if (
        !st_expression_set_value(
            expression,
            name
        )
    )
    {
        st_expression_free(expression);
        return NULL;
    }

    return expression;
}


const char* st_expression_type_name(
    STExpressionType type
)
{
    switch (type)
    {
        case ST_EXPRESSION_INTEGER:
            return "integer";

        case ST_EXPRESSION_FLOAT:
            return "float";

        case ST_EXPRESSION_STRING:
            return "string";

        case ST_EXPRESSION_BOOLEAN:
            return "boolean";

        case ST_EXPRESSION_CHARACTER:
            return "character";

        case ST_EXPRESSION_IDENTIFIER:
            return "identifier";

        case ST_EXPRESSION_BINARY:
            return "binary";

        case ST_EXPRESSION_UNARY:
            return "unary";

        case ST_EXPRESSION_GROUP:
            return "group";

        default:
            return "none";
    }
}


void st_expression_free(
    STExpression* expression
)
{
    int i;

    if (expression == NULL)
        return;

    /*
     * Binary and unary expressions
     * own their child expressions.
     */
    if (expression->left != NULL)
    {
        st_expression_free(
            expression->left
        );

        expression->left = NULL;
    }

    if (
        expression->right != NULL
    )
    {
        st_expression_free(
            expression->right
        );

        expression->right = NULL;
    }

    /*
     * Free additional children.
     */
    for (i = 0;
         i < expression->child_count;
         i++)
    {
        if (
            expression->children[i] != NULL
        )
        {
            /*
             * Avoid freeing a child twice
             * if it is also left/right.
             */
            if (
                expression->children[i]
                != expression->left &&
                expression->children[i]
                != expression->right
            )
            {
                st_expression_free(
                    expression->children[i]
                );
            }

            expression->children[i] = NULL;
        }
    }

    free(expression->value);
    expression->value = NULL;

    free(expression);
}