#include "ast.h"

#include <stdlib.h>
#include <string.h>


static char* st_ast_strdup(
    const char* text
)
{
    size_t length;
    char* result;

    if (text == NULL)
        return NULL;

    length = strlen(text);

    result = (char*)malloc(
        length + 1
    );

    if (result == NULL)
        return NULL;

    memcpy(
        result,
        text,
        length + 1
    );

    return result;
}


STASTNode* st_ast_create(
    STASTNodeType type,
    int line,
    int column
)
{
    STASTNode* node;

    node = (STASTNode*)calloc(
        1,
        sizeof(STASTNode)
    );

    if (node == NULL)
        return NULL;

    node->type = type;
    node->line = line;
    node->column = column;
    node->data_type = ST_TYPE_VOID;

    return node;
}


int st_ast_set_name(
    STASTNode* node,
    const char* name
)
{
    if (node == NULL)
        return 0;

    if (name == NULL)
    {
        node->name[0] = '\0';
        return 1;
    }

    strncpy(
        node->name,
        name,
        ST_AST_MAX_NAME - 1
    );

    node->name[
        ST_AST_MAX_NAME - 1
    ] = '\0';

    return 1;
}


int st_ast_set_type(
    STASTNode* node,
    STType type
)
{
    if (node == NULL)
        return 0;

    node->data_type = type;

    return 1;
}


int st_ast_set_expression(
    STASTNode* node,
    STExpression* expression
)
{
    if (node == NULL)
        return 0;

    node->expression = expression;

    return 1;
}


int st_ast_set_statement(
    STASTNode* node,
    STStatement* statement
)
{
    if (node == NULL)
        return 0;

    node->statement = statement;

    return 1;
}


int st_ast_add_child(
    STASTNode* parent,
    STASTNode* child
)
{
    if (parent == NULL ||
        child == NULL)
    {
        return 0;
    }

    if (parent->child_count >=
        ST_AST_MAX_CHILDREN)
    {
        return 0;
    }

    parent->children[
        parent->child_count
    ] = child;

    parent->child_count++;

    child->parent = parent;

    return 1;
}


STASTNode* st_ast_get_child(
    STASTNode* node,
    int index
)
{
    if (node == NULL)
        return NULL;

    if (index < 0 ||
        index >= node->child_count)
    {
        return NULL;
    }

    return node->children[index];
}


int st_ast_child_count(
    const STASTNode* node
)
{
    if (node == NULL)
        return 0;

    return node->child_count;
}


const char* st_ast_node_type_name(
    STASTNodeType type
)
{
    switch (type)
    {
        case ST_AST_NONE:
            return "none";

        case ST_AST_PROGRAM:
            return "program";

        case ST_AST_BLOCK:
            return "block";

        case ST_AST_EXPRESSION:
            return "expression";

        case ST_AST_VARIABLE:
            return "variable";

        case ST_AST_ASSIGNMENT:
            return "assignment";

        case ST_AST_FUNCTION:
            return "function";

        case ST_AST_FUNCTION_CALL:
            return "function_call";

        case ST_AST_RETURN:
            return "return";

        case ST_AST_BREAK:
            return "break";

        case ST_AST_CONTINUE:
            return "continue";

        case ST_AST_IF:
            return "if";

        case ST_AST_ELSE_IF:
            return "else_if";

        case ST_AST_ELSE:
            return "else";

        case ST_AST_SWITCH:
            return "switch";

        case ST_AST_CASE:
            return "case";

        case ST_AST_DEFAULT:
            return "default";

        case ST_AST_WHILE:
            return "while";

        case ST_AST_FOR:
            return "for";

        case ST_AST_DO_WHILE:
            return "do_while";

        default:
            return "unknown";
    }
}


void st_ast_free(
    STASTNode* node
)
{
    int i;

    if (node == NULL)
        return;


    /*
     * Free children first.
     */

    for (i = 0;
         i < node->child_count;
         i++)
    {
        if (node->children[i] != NULL)
        {
            node->children[i]->parent = NULL;

            st_ast_free(
                node->children[i]
            );

            node->children[i] = NULL;
        }
    }


    /*
     * The AST owns its expression.
     */

    if (node->expression != NULL)
    {
        st_expression_free(
            node->expression
        );

        node->expression = NULL;
    }


    /*
     * The AST owns its statement.
     */

    if (node->statement != NULL)
    {
        st_statement_free(
            node->statement
        );

        node->statement = NULL;
    }


    free(node);
}


static STASTNode* st_ast_statement_node(
    STStatement* statement
)
{
    STASTNode* node;

    if (statement == NULL)
        return NULL;


    switch (statement->type)
    {
        case ST_STATEMENT_EXPRESSION:
            node =
                st_ast_create(
                    ST_AST_EXPRESSION,
                    statement->line,
                    statement->column
                );
            break;


        case ST_STATEMENT_VARIABLE_DECLARATION:
            node =
                st_ast_create(
                    ST_AST_VARIABLE,
                    statement->line,
                    statement->column
                );
            break;


        case ST_STATEMENT_ASSIGNMENT:
            node =
                st_ast_create(
                    ST_AST_ASSIGNMENT,
                    statement->line,
                    statement->column
                );
            break;


        case ST_STATEMENT_RETURN:
            node =
                st_ast_create(
                    ST_AST_RETURN,
                    statement->line,
                    statement->column
                );
            break;


        case ST_STATEMENT_BREAK:
            node =
                st_ast_create(
                    ST_AST_BREAK,
                    statement->line,
                    statement->column
                );
            break;


        case ST_STATEMENT_CONTINUE:
            node =
                st_ast_create(
                    ST_AST_CONTINUE,
                    statement->line,
                    statement->column
                );
            break;


        case ST_STATEMENT_BLOCK:
            node =
                st_ast_create(
                    ST_AST_BLOCK,
                    statement->line,
                    statement->column
                );
            break;


        case ST_STATEMENT_FUNCTION_CALL:
            node =
                st_ast_create(
                    ST_AST_FUNCTION_CALL,
                    statement->line,
                    statement->column
                );
            break;


        case ST_STATEMENT_IF:
            node =
                st_ast_create(
                    ST_AST_IF,
                    statement->line,
                    statement->column
                );
            break;


        case ST_STATEMENT_LOOP:
            node =
                st_ast_create(
                    ST_AST_WHILE,
                    statement->line,
                    statement->column
                );
            break;


        default:
            node =
                st_ast_create(
                    ST_AST_NONE,
                    statement->line,
                    statement->column
                );
            break;
    }


    if (node == NULL)
        return NULL;


    if (statement->name != NULL)
    {
        st_ast_set_name(
            node,
            statement->name
        );
    }


    if (statement->expression != NULL)
    {
        node->expression =
            statement->expression;

        statement->expression = NULL;
    }


    return node;
}


STASTNode* st_ast_from_statements(
    STStatement* statements
)
{
    STASTNode* program;
    STStatement* current;

    program =
        st_ast_create(
            ST_AST_PROGRAM,
            1,
            1
        );

    if (program == NULL)
        return NULL;


    current = statements;

    while (current != NULL)
    {
        STStatement* next;
        STASTNode* node;

        next = current->next;

        current->next = NULL;

        node =
            st_ast_statement_node(
                current
            );

        /*
         * The AST node takes ownership
         * of the expression, but not the
         * original statement object.
         */

        st_statement_free(
            current
        );

        if (node == NULL)
        {
            st_ast_free(program);
            return NULL;
        }


        if (!st_ast_add_child(
                program,
                node))
        {
            st_ast_free(node);
            st_ast_free(program);
            return NULL;
        }


        current = next;
    }


    return program;
}