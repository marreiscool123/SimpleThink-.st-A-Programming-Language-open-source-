#include "parser.h"

#include <stdio.h>
#include <string.h>

#include "operators.h"
#include "types.h"


/*
 * ---------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------
 */

static void st_parser_error(
    STParser* parser,
    const char* message
)
{
    if (parser == NULL)
        return;

    parser->error_count++;

    if (message == NULL)
        message = "parser error";

    snprintf(
        parser->last_error,
        ST_PARSER_MAX_ERROR,
        "%d:%d: %s",
        parser->current.line,
        parser->current.column,
        message
    );
}


static STOperatorType st_parser_get_operator(
    STTokenType type
)
{
    switch (type)
    {
        case ST_TOKEN_PLUS:
            return ST_OPERATOR_ADD;

        case ST_TOKEN_MINUS:
            return ST_OPERATOR_SUBTRACT;

        case ST_TOKEN_STAR:
            return ST_OPERATOR_MULTIPLY;

        case ST_TOKEN_SLASH:
            return ST_OPERATOR_DIVIDE;

        case ST_TOKEN_PERCENT:
            return ST_OPERATOR_MODULO;

        case ST_TOKEN_EQUAL:
            return ST_OPERATOR_EQUAL;

        case ST_TOKEN_NOT_EQUAL:
            return ST_OPERATOR_NOT_EQUAL;

        case ST_TOKEN_LESS:
            return ST_OPERATOR_LESS;

        case ST_TOKEN_LESS_EQUAL:
            return ST_OPERATOR_LESS_EQUAL;

        case ST_TOKEN_GREATER:
            return ST_OPERATOR_GREATER;

        case ST_TOKEN_GREATER_EQUAL:
            return ST_OPERATOR_GREATER_EQUAL;

        case ST_TOKEN_AND:
            return ST_OPERATOR_AND;

        case ST_TOKEN_OR:
            return ST_OPERATOR_OR;

        case ST_TOKEN_ASSIGN:
            return ST_OPERATOR_ASSIGN;

        case ST_TOKEN_ADD_ASSIGN:
            return ST_OPERATOR_ADD_ASSIGN;

        case ST_TOKEN_SUB_ASSIGN:
            return ST_OPERATOR_SUB_ASSIGN;

        case ST_TOKEN_MUL_ASSIGN:
            return ST_OPERATOR_MUL_ASSIGN;

        case ST_TOKEN_DIV_ASSIGN:
            return ST_OPERATOR_DIV_ASSIGN;

        case ST_TOKEN_MOD_ASSIGN:
            return ST_OPERATOR_MOD_ASSIGN;

        case ST_TOKEN_BIT_AND:
            return ST_OPERATOR_BIT_AND;

        case ST_TOKEN_BIT_OR:
            return ST_OPERATOR_BIT_OR;

        case ST_TOKEN_BIT_XOR:
            return ST_OPERATOR_BIT_XOR;

        case ST_TOKEN_SHIFT_LEFT:
            return ST_OPERATOR_SHIFT_LEFT;

        case ST_TOKEN_SHIFT_RIGHT:
            return ST_OPERATOR_SHIFT_RIGHT;

        default:
            return ST_OPERATOR_ADD;
    }
}


static int st_parser_is_binary(
    STTokenType type
)
{
    switch (type)
    {
        case ST_TOKEN_PLUS:
        case ST_TOKEN_MINUS:
        case ST_TOKEN_STAR:
        case ST_TOKEN_SLASH:
        case ST_TOKEN_PERCENT:

        case ST_TOKEN_EQUAL:
        case ST_TOKEN_NOT_EQUAL:

        case ST_TOKEN_LESS:
        case ST_TOKEN_LESS_EQUAL:
        case ST_TOKEN_GREATER:
        case ST_TOKEN_GREATER_EQUAL:

        case ST_TOKEN_AND:
        case ST_TOKEN_OR:

        case ST_TOKEN_ASSIGN:
        case ST_TOKEN_ADD_ASSIGN:
        case ST_TOKEN_SUB_ASSIGN:
        case ST_TOKEN_MUL_ASSIGN:
        case ST_TOKEN_DIV_ASSIGN:
        case ST_TOKEN_MOD_ASSIGN:

        case ST_TOKEN_BIT_AND:
        case ST_TOKEN_BIT_OR:
        case ST_TOKEN_BIT_XOR:
        case ST_TOKEN_SHIFT_LEFT:
        case ST_TOKEN_SHIFT_RIGHT:
            return 1;

        default:
            return 0;
    }
}


/*
 * ---------------------------------------------------------
 * Parser initialization
 * ---------------------------------------------------------
 */

void st_parser_init(
    STParser* parser,
    const char* source
)
{
    if (parser == NULL)
        return;

    memset(
        parser,
        0,
        sizeof(STParser)
    );

    st_lexer_init(
        &parser->lexer,
        source
    );

    parser->current =
        st_lexer_next(
            &parser->lexer
        );

    parser->previous =
        parser->current;
}


/*
 * ---------------------------------------------------------
 * Token functions
 * ---------------------------------------------------------
 */

void st_parser_advance(
    STParser* parser
)
{
    if (parser == NULL)
        return;

    parser->previous =
        parser->current;

    parser->current =
        st_lexer_next(
            &parser->lexer
        );
}


int st_parser_check(
    const STParser* parser,
    STTokenType type
)
{
    if (parser == NULL)
        return 0;

    return parser->current.type == type;
}


int st_parser_match(
    STParser* parser,
    STTokenType type
)
{
    if (parser == NULL)
        return 0;

    if (!st_parser_check(
            parser,
            type))
    {
        return 0;
    }

    st_parser_advance(parser);

    return 1;
}


int st_parser_expect(
    STParser* parser,
    STTokenType type,
    const char* message
)
{
    if (parser == NULL)
        return 0;

    if (st_parser_check(
            parser,
            type))
    {
        st_parser_advance(parser);
        return 1;
    }

    st_parser_error(
        parser,
        message
    );

    return 0;
}


/*
 * ---------------------------------------------------------
 * Primary expressions
 * ---------------------------------------------------------
 */

STExpression* st_parser_primary(
    STParser* parser
)
{
    STExpression* expression;

    if (parser == NULL)
        return NULL;


    /*
     * Integer
     */

    if (st_parser_check(
            parser,
            ST_TOKEN_INTEGER))
    {
        expression =
            st_expression_create(
                ST_EXPRESSION_INTEGER,
                parser->current.line,
                parser->current.column
            );

        if (expression == NULL)
            return NULL;

        st_expression_set_value(
            expression,
            parser->current.text
        );

        st_parser_advance(parser);

        return expression;
    }


    /*
     * Float
     */

    if (st_parser_check(
            parser,
            ST_TOKEN_FLOAT))
    {
        expression =
            st_expression_create(
                ST_EXPRESSION_FLOAT,
                parser->current.line,
                parser->current.column
            );

        if (expression == NULL)
            return NULL;

        st_expression_set_value(
            expression,
            parser->current.text
        );

        st_parser_advance(parser);

        return expression;
    }


    /*
     * String
     */

    if (st_parser_check(
            parser,
            ST_TOKEN_STRING))
    {
        expression =
            st_expression_create(
                ST_EXPRESSION_STRING,
                parser->current.line,
                parser->current.column
            );

        if (expression == NULL)
            return NULL;

        st_expression_set_value(
            expression,
            parser->current.text
        );

        st_parser_advance(parser);

        return expression;
    }


    /*
     * Character
     */

    if (st_parser_check(
            parser,
            ST_TOKEN_CHARACTER))
    {
        expression =
            st_expression_create(
                ST_EXPRESSION_CHARACTER,
                parser->current.line,
                parser->current.column
            );

        if (expression == NULL)
            return NULL;

        st_expression_set_value(
            expression,
            parser->current.text
        );

        st_parser_advance(parser);

        return expression;
    }


    /*
     * Boolean
     */

    if (st_parser_check(
            parser,
            ST_TOKEN_TRUE) ||
        st_parser_check(
            parser,
            ST_TOKEN_FALSE))
    {
        expression =
            st_expression_create(
                ST_EXPRESSION_BOOLEAN,
                parser->current.line,
                parser->current.column
            );

        if (expression == NULL)
            return NULL;

        st_expression_set_value(
            expression,
            parser->current.text
        );

        st_parser_advance(parser);

        return expression;
    }


    /*
     * Identifier
     */

    if (st_parser_check(
            parser,
            ST_TOKEN_IDENTIFIER))
    {
        expression =
            st_expression_create(
                ST_EXPRESSION_IDENTIFIER,
                parser->current.line,
                parser->current.column
            );

        if (expression == NULL)
            return NULL;

        st_expression_set_value(
            expression,
            parser->current.text
        );

        st_parser_advance(parser);

        return expression;
    }


    /*
     * Parenthesized expression
     */

    if (st_parser_match(
            parser,
            ST_TOKEN_LEFT_PAREN))
    {
        expression =
            st_parser_expression(
                parser
            );

        if (expression == NULL)
            return NULL;

        if (!st_parser_expect(
                parser,
                ST_TOKEN_RIGHT_PAREN,
                "expected ')'"
            ))
        {
            st_expression_free(
                expression
            );

            return NULL;
        }

        return expression;
    }


    st_parser_error(
        parser,
        "expected expression"
    );

    return NULL;
}


/*
 * ---------------------------------------------------------
 * Unary expressions
 * ---------------------------------------------------------
 */

STExpression* st_parser_unary(
    STParser* parser
)
{
    STOperatorType operator;
    STExpression* child;
    STExpression* expression;

    int line;
    int column;

    if (parser == NULL)
        return NULL;


    if (st_parser_check(
            parser,
            ST_TOKEN_NOT) ||
        st_parser_check(
            parser,
            ST_TOKEN_BIT_NOT) ||
        st_parser_check(
            parser,
            ST_TOKEN_INCREMENT) ||
        st_parser_check(
            parser,
            ST_TOKEN_DECREMENT) ||
        st_parser_check(
            parser,
            ST_TOKEN_MINUS) ||
        st_parser_check(
            parser,
            ST_TOKEN_PLUS))
    {
        operator =
            st_parser_get_operator(
                parser->current.type
            );

        line = parser->current.line;
        column = parser->current.column;

        st_parser_advance(parser);

        child =
            st_parser_unary(
                parser
            );

        if (child == NULL)
            return NULL;

        expression =
            st_expression_create(
                ST_EXPRESSION_UNARY,
                line,
                column
            );

        if (expression == NULL)
        {
            st_expression_free(child);
            return NULL;
        }

        if (!st_expression_set_unary(
                expression,
                operator,
                child
            ))
        {
            st_expression_free(child);
            st_expression_free(expression);

            return NULL;
        }

        return expression;
    }


    return st_parser_primary(
        parser
    );
}


/*
 * ---------------------------------------------------------
 * Binary expressions
 * ---------------------------------------------------------
 */

STExpression* st_parser_binary(
    STParser* parser,
    int minimum_precedence
)
{
    STExpression* left;

    if (parser == NULL)
        return NULL;

    left =
        st_parser_unary(
            parser
        );

    if (left == NULL)
        return NULL;


    while (st_parser_is_binary(
        parser->current.type))
    {
        STOperatorType operator;
        STExpression* right;
        STExpression* result;

        int precedence;


        operator =
            st_parser_get_operator(
                parser->current.type
            );

        precedence =
            st_operator_precedence(
                operator
            );


        if (precedence < minimum_precedence)
            break;


        st_parser_advance(
            parser
        );


        right =
            st_parser_binary(
                parser,
                precedence + 1
            );

        if (right == NULL)
        {
            st_expression_free(left);
            return NULL;
        }


        result =
            st_expression_create(
                ST_EXPRESSION_BINARY,
                left->line,
                left->column
            );

        if (result == NULL)
        {
            st_expression_free(left);
            st_expression_free(right);

            return NULL;
        }


        if (!st_expression_set_binary(
                result,
                operator,
                left,
                right
            ))
        {
            st_expression_free(left);
            st_expression_free(right);
            st_expression_free(result);

            return NULL;
        }


        left = result;
    }


    return left;
}


/*
 * ---------------------------------------------------------
 * Expression
 * ---------------------------------------------------------
 */

STExpression* st_parser_expression(
    STParser* parser
)
{
    if (parser == NULL)
        return NULL;

    return st_parser_binary(
        parser,
        1
    );
}


/*
 * ---------------------------------------------------------
 * Variable declarations
 * ---------------------------------------------------------
 */

static int st_parser_is_type(
    STTokenType type
)
{
    switch (type)
    {
        case ST_TOKEN_INT:
        case ST_TOKEN_FLOAT_TYPE:
        case ST_TOKEN_STRING_TYPE:
        case ST_TOKEN_BOOL:
        case ST_TOKEN_CHAR:
            return 1;

        default:
            return 0;
    }
}


static STStatement* st_parser_variable(
    STParser* parser
)
{
    STStatement* statement;

    STType type;

    char type_text[64];
    char name_text[ST_TOKEN_MAX_TEXT];

    int line;
    int column;


    if (parser == NULL)
        return NULL;


    if (!st_parser_is_type(
            parser->current.type))
    {
        return NULL;
    }


    line = parser->current.line;
    column = parser->current.column;


    if (!st_type_from_name(
            parser->current.text,
            &type))
    {
        st_parser_error(
            parser,
            "invalid variable type"
        );

        return NULL;
    }


    snprintf(
        type_text,
        sizeof(type_text),
        "%s",
        st_type_name(type)
    );


    st_parser_advance(parser);


    if (!st_parser_check(
            parser,
            ST_TOKEN_IDENTIFIER))
    {
        st_parser_error(
            parser,
            "expected variable name"
        );

        return NULL;
    }


    snprintf(
        name_text,
        sizeof(name_text),
        "%s",
        parser->current.text
    );


    st_parser_advance(parser);


    statement =
        st_statement_create(
            ST_STATEMENT_VARIABLE_DECLARATION,
            line,
            column
        );

    if (statement == NULL)
        return NULL;


    if (!st_statement_set_name(
            statement,
            name_text))
    {
        st_statement_free(statement);
        return NULL;
    }


    if (!st_statement_set_value(
            statement,
            type_text))
    {
        st_statement_free(statement);
        return NULL;
    }


    /*
     * Optional initializer.
     */

    if (st_parser_match(
            parser,
            ST_TOKEN_ASSIGN))
    {
        STExpression* expression;

        expression =
            st_parser_expression(
                parser
            );

        if (expression == NULL)
        {
            st_statement_free(statement);
            return NULL;
        }


        if (!st_statement_set_expression(
                statement,
                expression))
        {
            st_expression_free(expression);
            st_statement_free(statement);

            return NULL;
        }
    }


    if (!st_parser_expect(
            parser,
            ST_TOKEN_SEMICOLON,
            "expected ';' after variable declaration"
        ))
    {
        st_statement_free(statement);
        return NULL;
    }


    return statement;
}


/*
 * ---------------------------------------------------------
 * Function calls
 * ---------------------------------------------------------
 */

static STStatement* st_parser_function_call(
    STParser* parser
)
{
    STStatement* statement;

    char name[ST_TOKEN_MAX_TEXT];

    int line;
    int column;


    if (parser == NULL)
        return NULL;


    if (!st_parser_check(
            parser,
            ST_TOKEN_IDENTIFIER))
    {
        return NULL;
    }


    line = parser->current.line;
    column = parser->current.column;


    snprintf(
        name,
        sizeof(name),
        "%s",
        parser->current.text
    );


    st_parser_advance(parser);


    if (!st_parser_expect(
            parser,
            ST_TOKEN_LEFT_PAREN,
            "expected '(' after function name"
        ))
    {
        return NULL;
    }


    statement =
        st_statement_create(
            ST_STATEMENT_FUNCTION_CALL,
            line,
            column
        );


    if (statement == NULL)
        return NULL;


    if (!st_statement_set_name(
            statement,
            name))
    {
        st_statement_free(statement);
        return NULL;
    }


    /*
     * Function arguments.
     */

    if (!st_parser_check(
            parser,
            ST_TOKEN_RIGHT_PAREN))
    {
        while (1)
        {
            STExpression* argument;


            argument =
                st_parser_expression(
                    parser
                );


            if (argument == NULL)
            {
                st_statement_free(statement);
                return NULL;
            }


            if (!st_statement_add_argument(
                    statement,
                    argument))
            {
                st_expression_free(argument);
                st_statement_free(statement);
                return NULL;
            }


            if (!st_parser_match(
                    parser,
                    ST_TOKEN_COMMA))
            {
                break;
            }
        }
    }


    if (!st_parser_expect(
            parser,
            ST_TOKEN_RIGHT_PAREN,
            "expected ')' after function arguments"
        ))
    {
        st_statement_free(statement);
        return NULL;
    }


    if (!st_parser_expect(
            parser,
            ST_TOKEN_SEMICOLON,
            "expected ';' after function call"
        ))
    {
        st_statement_free(statement);
        return NULL;
    }


    return statement;
}


/*
 * ---------------------------------------------------------
 * Statements
 * ---------------------------------------------------------
 */

STStatement* st_parser_statement(
    STParser* parser
)
{
    STStatement* statement;
    STExpression* expression;

    int line;
    int column;


    if (parser == NULL)
        return NULL;


    /*
     * Empty statement.
     */

    if (st_parser_match(
            parser,
            ST_TOKEN_SEMICOLON))
    {
        return NULL;
    }


    /*
     * Variable declaration.
     */

    if (st_parser_is_type(
            parser->current.type))
    {
        return st_parser_variable(
            parser
        );
    }


    /*
     * return
     */

    if (st_parser_match(
            parser,
            ST_TOKEN_RETURN))
    {
        line = parser->previous.line;
        column = parser->previous.column;


        if (st_parser_match(
                parser,
                ST_TOKEN_SEMICOLON))
        {
            return st_statement_return(
                NULL,
                line,
                column
            );
        }


        expression =
            st_parser_expression(
                parser
            );

        if (expression == NULL)
            return NULL;


        if (!st_parser_expect(
                parser,
                ST_TOKEN_SEMICOLON,
                "expected ';' after return"
            ))
        {
            st_expression_free(expression);
            return NULL;
        }


        return st_statement_return(
            expression,
            line,
            column
        );
    }


    /*
     * break
     */

    if (st_parser_match(
            parser,
            ST_TOKEN_BREAK))
    {
        line = parser->previous.line;
        column = parser->previous.column;


        if (!st_parser_expect(
                parser,
                ST_TOKEN_SEMICOLON,
                "expected ';' after break"
            ))
        {
            return NULL;
        }


        return st_statement_break(
            line,
            column
        );
    }


    /*
     * continue
     */

    if (st_parser_match(
            parser,
            ST_TOKEN_CONTINUE))
    {
        line = parser->previous.line;
        column = parser->previous.column;


        if (!st_parser_expect(
                parser,
                ST_TOKEN_SEMICOLON,
                "expected ';' after continue"
            ))
        {
            return NULL;
        }


        return st_statement_continue(
            line,
            column
        );
    }


    /*
     * Function call.
     */

    if (st_parser_check(
            parser,
            ST_TOKEN_IDENTIFIER))
    {
        STParser lookahead = *parser;


        st_parser_advance(
            &lookahead
        );


        if (st_parser_check(
                &lookahead,
                ST_TOKEN_LEFT_PAREN))
        {
            return st_parser_function_call(
                parser
            );
        }
    }


    /*
     * Normal expression statement.
     */

    line = parser->current.line;
    column = parser->current.column;


    expression =
        st_parser_expression(
            parser
        );

    if (expression == NULL)
        return NULL;


    if (!st_parser_expect(
            parser,
            ST_TOKEN_SEMICOLON,
            "expected ';' after expression"
        ))
    {
        st_expression_free(expression);
        return NULL;
    }


    statement =
        st_statement_expression(
            expression,
            line,
            column
        );


    return statement;
}


/*
 * ---------------------------------------------------------
 * Program
 * ---------------------------------------------------------
 */

STStatement* st_parser_program(
    STParser* parser
)
{
    STStatement* first;
    STStatement* last;

    first = NULL;
    last = NULL;


    if (parser == NULL)
        return NULL;


    while (!st_parser_check(
        parser,
        ST_TOKEN_EOF))
    {
        STStatement* statement;


        statement =
            st_parser_statement(
                parser
            );


        if (statement == NULL)
        {
            /*
             * Stop on an actual parser error.
             */

            if (parser->error_count > 0)
                break;


            continue;
        }


        if (first == NULL)
        {
            first = statement;
            last = statement;
        }
        else
        {
            st_statement_set_next(
                last,
                statement
            );

            last = statement;
        }
    }


    return first;
}


/*
 * ---------------------------------------------------------
 * Errors
 * ---------------------------------------------------------
 */

int st_parser_has_errors(
    const STParser* parser
)
{
    if (parser == NULL)
        return 1;

    return parser->error_count > 0;
}


const char* st_parser_last_error(
    const STParser* parser
)
{
    if (parser == NULL)
        return "invalid parser";

    return parser->last_error;
}


int st_parser_error_count(
    const STParser* parser
)
{
    if (parser == NULL)
        return -1;

    return parser->error_count;
}