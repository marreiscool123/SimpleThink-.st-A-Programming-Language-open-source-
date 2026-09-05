#include "codegen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operators.h"


static int st_codegen_reserve(
    STCodeGenerator* codegen,
    int additional
)
{
    char* new_output;
    int required;
    int new_capacity;

    if (codegen == NULL)
        return 0;

    if (additional < 0)
        return 0;

    required =
        codegen->output_size +
        additional +
        1;

    if (required <= codegen->output_capacity)
        return 1;

    new_capacity =
        codegen->output_capacity;

    if (new_capacity < 256)
        new_capacity = 256;

    while (new_capacity < required)
    {
        new_capacity *= 2;
    }

    new_output =
        (char*)realloc(
            codegen->output,
            (size_t)new_capacity
        );

    if (new_output == NULL)
        return 0;

    codegen->output = new_output;
    codegen->output_capacity = new_capacity;

    return 1;
}


static int st_codegen_append(
    STCodeGenerator* codegen,
    const char* text
)
{
    int length;

    if (codegen == NULL ||
        text == NULL)
    {
        return 0;
    }

    length = (int)strlen(text);

    if (!st_codegen_reserve(
            codegen,
            length))
    {
        return 0;
    }

    memcpy(
        codegen->output +
            codegen->output_size,
        text,
        (size_t)length
    );

    codegen->output_size += length;

    codegen->output[
        codegen->output_size
    ] = '\0';

    return 1;
}


static void st_codegen_error(
    STCodeGenerator* codegen,
    const char* message
)
{
    if (codegen == NULL)
        return;

    codegen->error_count++;

    if (message == NULL)
        message = "code generation error";

    snprintf(
        codegen->last_error,
        sizeof(codegen->last_error),
        "%s",
        message
    );
}


static int st_codegen_expression(
    STCodeGenerator* codegen,
    STExpression* expression
)
{
    const char* operator_text;

    if (expression == NULL)
        return 0;

    switch (expression->type)
    {
        case ST_EXPRESSION_INTEGER:
        case ST_EXPRESSION_FLOAT:
        case ST_EXPRESSION_STRING:
        case ST_EXPRESSION_BOOLEAN:
        case ST_EXPRESSION_CHARACTER:
        case ST_EXPRESSION_IDENTIFIER:
        {
            if (expression->value == NULL)
                return 1;

            return st_codegen_append(
                codegen,
                expression->value
            );
        }


        case ST_EXPRESSION_UNARY:
        {
            operator_text =
                st_operator_symbol(
                    expression->operator
                );

            if (operator_text == NULL)
                operator_text = "";

            if (!st_codegen_append(
                    codegen,
                    operator_text))
            {
                return 0;
            }

            return st_codegen_expression(
                codegen,
                expression->left
            );
        }


        case ST_EXPRESSION_BINARY:
        {
            if (!st_codegen_append(
                    codegen,
                    "("))
            {
                return 0;
            }

            if (!st_codegen_expression(
                    codegen,
                    expression->left))
            {
                return 0;
            }

            operator_text =
                st_operator_symbol(
                    expression->operator
                );

            if (operator_text == NULL)
                operator_text = "?";

            if (!st_codegen_append(
                    codegen,
                    " "))
            {
                return 0;
            }

            if (!st_codegen_append(
                    codegen,
                    operator_text))
            {
                return 0;
            }

            if (!st_codegen_append(
                    codegen,
                    " "))
            {
                return 0;
            }

            if (!st_codegen_expression(
                    codegen,
                    expression->right))
            {
                return 0;
            }

            return st_codegen_append(
                codegen,
                ")"
            );
        }


        case ST_EXPRESSION_GROUP:
        {
            if (!st_codegen_append(
                    codegen,
                    "("))
            {
                return 0;
            }

            if (expression->child_count > 0)
            {
                if (!st_codegen_expression(
                        codegen,
                        expression->children[0]))
                {
                    return 0;
                }
            }

            return st_codegen_append(
                codegen,
                ")"
            );
        }


        default:
            return 0;
    }
}


static int st_codegen_statement(
    STCodeGenerator* codegen,
    STStatement* statement
)
{
    if (statement == NULL)
        return 1;


    switch (statement->type)
    {
        case ST_STATEMENT_EXPRESSION:
        {
            if (!st_codegen_expression(
                    codegen,
                    statement->expression))
            {
                return 0;
            }

            return st_codegen_append(
                codegen,
                ";\n"
            );
        }


        case ST_STATEMENT_VARIABLE_DECLARATION:
        {
            if (statement->value == NULL ||
                statement->name == NULL)
            {
                st_codegen_error(
                    codegen,
                    "invalid variable declaration"
                );

                return 0;
            }

            if (!st_codegen_append(
                    codegen,
                    statement->value))
            {
                return 0;
            }

            if (!st_codegen_append(
                    codegen,
                    " "))
            {
                return 0;
            }

            if (!st_codegen_append(
                    codegen,
                    statement->name))
            {
                return 0;
            }

            if (statement->expression != NULL)
            {
                if (!st_codegen_append(
                        codegen,
                        " = "))
                {
                    return 0;
                }

                if (!st_codegen_expression(
                        codegen,
                        statement->expression))
                {
                    return 0;
                }
            }

            return st_codegen_append(
                codegen,
                ";\n"
            );
        }


        case ST_STATEMENT_ASSIGNMENT:
        {
            if (statement->name != NULL)
            {
                if (!st_codegen_append(
                        codegen,
                        statement->name))
                {
                    return 0;
                }
            }

            if (statement->expression != NULL)
            {
                if (!st_codegen_append(
                        codegen,
                        " = "))
                {
                    return 0;
                }

                if (!st_codegen_expression(
                        codegen,
                        statement->expression))
                {
                    return 0;
                }
            }

            return st_codegen_append(
                codegen,
                ";\n"
            );
        }


        case ST_STATEMENT_RETURN:
        {
            if (!st_codegen_append(
                    codegen,
                    "return"))
            {
                return 0;
            }

            if (statement->expression != NULL)
            {
                if (!st_codegen_append(
                        codegen,
                        " "))
                {
                    return 0;
                }

                if (!st_codegen_expression(
                        codegen,
                        statement->expression))
                {
                    return 0;
                }
            }

            return st_codegen_append(
                codegen,
                ";\n"
            );
        }


        case ST_STATEMENT_BREAK:
            return st_codegen_append(
                codegen,
                "break;\n"
            );


        case ST_STATEMENT_CONTINUE:
            return st_codegen_append(
                codegen,
                "continue;\n"
            );


        default:
            st_codegen_error(
                codegen,
                "unsupported statement"
            );

            return 0;
    }
}


static int st_codegen_node(
    STCodeGenerator* codegen,
    STASTNode* node
)
{
    int i;

    if (node == NULL)
        return 1;


    if (node->type == ST_AST_PROGRAM ||
        node->type == ST_AST_BLOCK)
    {
        for (i = 0;
             i < node->child_count;
             i++)
        {
            if (!st_codegen_node(
                    codegen,
                    node->children[i]))
            {
                return 0;
            }
        }

        return 1;
    }


    if (node->statement != NULL)
    {
        if (!st_codegen_statement(
                codegen,
                node->statement))
        {
            return 0;
        }
    }
    else if (node->expression != NULL)
    {
        if (!st_codegen_expression(
                codegen,
                node->expression))
        {
            return 0;
        }

        if (!st_codegen_append(
                codegen,
                ";\n"))
        {
            return 0;
        }
    }


    for (i = 0;
         i < node->child_count;
         i++)
    {
        if (!st_codegen_node(
                codegen,
                node->children[i]))
        {
            return 0;
        }
    }


    return 1;
}


void st_codegen_init(
    STCodeGenerator* codegen
)
{
    if (codegen == NULL)
        return;

    memset(
        codegen,
        0,
        sizeof(STCodeGenerator)
    );

    codegen->output_capacity = 256;

    codegen->output =
        (char*)malloc(
            (size_t)codegen->output_capacity
        );

    if (codegen->output != NULL)
        codegen->output[0] = '\0';
}


void st_codegen_free(
    STCodeGenerator* codegen
)
{
    if (codegen == NULL)
        return;

    free(
        codegen->output
    );

    codegen->output = NULL;
    codegen->output_size = 0;
    codegen->output_capacity = 0;
}


int st_codegen_set_root(
    STCodeGenerator* codegen,
    STASTNode* root
)
{
    if (codegen == NULL)
        return 0;

    codegen->root = root;

    return 1;
}


int st_codegen_generate(
    STCodeGenerator* codegen
)
{
    if (codegen == NULL)
        return 0;

    if (codegen->root == NULL)
    {
        st_codegen_error(
            codegen,
            "no AST root"
        );

        return 0;
    }

    codegen->output_size = 0;

    if (codegen->output != NULL)
        codegen->output[0] = '\0';

    codegen->error_count = 0;
    codegen->last_error[0] = '\0';


    /*
     * This stage currently generates
     * C-like intermediate source.
     *
     * A later backend will turn this
     * representation into a real Windows
     * executable.
     */

    if (!st_codegen_append(
            codegen,
            "/* Simple Think generated code */\n\n"))
    {
        st_codegen_error(
            codegen,
            "unable to create output"
        );

        return 0;
    }


    if (!st_codegen_node(
            codegen,
            codegen->root))
    {
        return 0;
    }


    return codegen->error_count == 0;
}


const char* st_codegen_output(
    const STCodeGenerator* codegen
)
{
    if (codegen == NULL)
        return NULL;

    return codegen->output;
}


int st_codegen_output_size(
    const STCodeGenerator* codegen
)
{
    if (codegen == NULL)
        return 0;

    return codegen->output_size;
}


int st_codegen_has_errors(
    const STCodeGenerator* codegen
)
{
    if (codegen == NULL)
        return 1;

    return codegen->error_count > 0;
}


const char* st_codegen_last_error(
    const STCodeGenerator* codegen
)
{
    if (codegen == NULL)
        return "invalid code generator";

    return codegen->last_error;
}