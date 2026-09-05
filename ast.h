#ifndef ST_AST_H
#define ST_AST_H

#include "expressions.h"
#include "statements.h"
#include "types.h"

#define ST_AST_MAX_CHILDREN 64
#define ST_AST_MAX_NAME 128

typedef enum STASTNodeType
{
    ST_AST_NONE = 0,

    ST_AST_PROGRAM,
    ST_AST_BLOCK,

    ST_AST_EXPRESSION,
    ST_AST_VARIABLE,
    ST_AST_ASSIGNMENT,

    ST_AST_FUNCTION,
    ST_AST_FUNCTION_CALL,

    ST_AST_RETURN,
    ST_AST_BREAK,
    ST_AST_CONTINUE,

    ST_AST_IF,
    ST_AST_ELSE_IF,
    ST_AST_ELSE,
    ST_AST_SWITCH,
    ST_AST_CASE,
    ST_AST_DEFAULT,

    ST_AST_WHILE,
    ST_AST_FOR,
    ST_AST_DO_WHILE
} STASTNodeType;


typedef struct STASTNode
{
    STASTNodeType type;

    int line;
    int column;

    char name[ST_AST_MAX_NAME];

    STType data_type;

    STExpression* expression;

    STStatement* statement;

    struct STASTNode* parent;

    struct STASTNode* children[ST_AST_MAX_CHILDREN];

    int child_count;

} STASTNode;


/*
 * Create an AST node.
 */
STASTNode* st_ast_create(
    STASTNodeType type,
    int line,
    int column
);


/*
 * Set node name.
 */
int st_ast_set_name(
    STASTNode* node,
    const char* name
);


/*
 * Set node data type.
 */
int st_ast_set_type(
    STASTNode* node,
    STType type
);


/*
 * Set expression.
 */
int st_ast_set_expression(
    STASTNode* node,
    STExpression* expression
);


/*
 * Set statement.
 */
int st_ast_set_statement(
    STASTNode* node,
    STStatement* statement
);


/*
 * Add child node.
 */
int st_ast_add_child(
    STASTNode* parent,
    STASTNode* child
);


/*
 * Get child.
 */
STASTNode* st_ast_get_child(
    STASTNode* node,
    int index
);


/*
 * Get number of children.
 */
int st_ast_child_count(
    const STASTNode* node
);


/*
 * Get node type name.
 */
const char* st_ast_node_type_name(
    STASTNodeType type
);


/*
 * Free AST recursively.
 */
void st_ast_free(
    STASTNode* node
);


/*
 * Create a program node from statements.
 */
STASTNode* st_ast_from_statements(
    STStatement* statements
);

#endif