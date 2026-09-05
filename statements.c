#include "statements.h"

#include <stdlib.h>
#include <string.h>


static char* st_statement_strdup(
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


STStatement* st_statement_create(
    STStatementType type,
    int line,
    int column
)
{
    STStatement* statement;

    statement = malloc(
        sizeof(STStatement)
    );

    if (statement == NULL)
        return NULL;

    memset(
        statement,
        0,
        sizeof(STStatement)
    );

    statement->type = type;
    statement->line = line;
    statement->column = column;

    return statement;
}


int st_statement_set_name(
    STStatement* statement,
    const char* name
)
{
    char* new_name;

    if (statement == NULL)
        return 0;

    new_name =
        st_statement_strdup(name);

    if (
        name != NULL &&
        new_name == NULL
    )
    {
        return 0;
    }

    free(statement->name);

    statement->name = new_name;

    return 1;
}


int st_statement_set_value(
    STStatement* statement,
    const char* value
)
{
    char* new_value;

    if (statement == NULL)
        return 0;

    new_value =
        st_statement_strdup(value);

    if (
        value != NULL &&
        new_value == NULL
    )
    {
        return 0;
    }

    free(statement->value);

    statement->value = new_value;

    return 1;
}


int st_statement_set_expression(
    STStatement* statement,
    STExpression* expression
)
{
    if (statement == NULL)
        return 0;

    statement->expression = expression;

    return 1;
}


int st_statement_add_argument(
    STStatement* statement,
    STExpression* argument
)
{
    if (statement == NULL)
        return 0;

    if (argument == NULL)
        return 0;

    if (
        statement->argument_count
        >= ST_STATEMENT_MAX_ARGUMENTS
    )
    {
        return 0;
    }

    statement->arguments[
        statement->argument_count
    ] = argument;

    statement->argument_count++;

    return 1;
}


int st_statement_set_next(
    STStatement* statement,
    STStatement* next
)
{
    if (statement == NULL)
        return 0;

    statement->next = next;

    return 1;
}


STStatement* st_statement_expression(
    STExpression* expression,
    int line,
    int column
)
{
    STStatement* statement;

    statement =
        st_statement_create(
            ST_STATEMENT_EXPRESSION,
            line,
            column
        );

    if (statement == NULL)
        return NULL;

    statement->expression = expression;

    return statement;
}


STStatement* st_statement_return(
    STExpression* expression,
    int line,
    int column
)
{
    STStatement* statement;

    statement =
        st_statement_create(
            ST_STATEMENT_RETURN,
            line,
            column
        );

    if (statement == NULL)
        return NULL;

    statement->expression = expression;

    return statement;
}


STStatement* st_statement_break(
    int line,
    int column
)
{
    return st_statement_create(
        ST_STATEMENT_BREAK,
        line,
        column
    );
}


STStatement* st_statement_continue(
    int line,
    int column
)
{
    return st_statement_create(
        ST_STATEMENT_CONTINUE,
        line,
        column
    );
}


int st_statement_is_control_flow(
    STStatementType type
)
{
    switch (type)
    {
        case ST_STATEMENT_RETURN:
        case ST_STATEMENT_BREAK:
        case ST_STATEMENT_CONTINUE:
        case ST_STATEMENT_IF:
        case ST_STATEMENT_LOOP:
            return 1;

        default:
            return 0;
    }
}


const char* st_statement_type_name(
    STStatementType type
)
{
    switch (type)
    {
        case ST_STATEMENT_EXPRESSION:
            return "expression";

        case ST_STATEMENT_VARIABLE_DECLARATION:
            return "variable declaration";

        case ST_STATEMENT_ASSIGNMENT:
            return "assignment";

        case ST_STATEMENT_FUNCTION_CALL:
            return "function call";

        case ST_STATEMENT_RETURN:
            return "return";

        case ST_STATEMENT_BREAK:
            return "break";

        case ST_STATEMENT_CONTINUE:
            return "continue";

        case ST_STATEMENT_IF:
            return "if";

        case ST_STATEMENT_LOOP:
            return "loop";

        case ST_STATEMENT_BLOCK:
            return "block";

        default:
            return "none";
    }
}


void st_statement_free(
    STStatement* statement
)
{
    int i;

    if (statement == NULL)
        return;

    /*
     * Free the main expression.
     */
    if (statement->expression != NULL)
    {
        st_expression_free(
            statement->expression
        );

        statement->expression = NULL;
    }

    /*
     * Free function arguments.
     */
    for (
        i = 0;
        i < statement->argument_count;
        i++
    )
    {
        if (statement->arguments[i] != NULL)
        {
            st_expression_free(
                statement->arguments[i]
            );

            statement->arguments[i] = NULL;
        }
    }

    /*
     * Free the next statement.
     */
    if (statement->next != NULL)
    {
        st_statement_free(
            statement->next
        );

        statement->next = NULL;
    }

    free(statement->name);
    free(statement->value);

    statement->name = NULL;
    statement->value = NULL;

    free(statement);
}