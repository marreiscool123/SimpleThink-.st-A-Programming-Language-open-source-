#ifndef ST_PARSER_H
#define ST_PARSER_H

#include "lexer.h"
#include "statements.h"

#define ST_PARSER_MAX_ERROR 512

typedef struct STParser
{
    STLexer lexer;

    STToken current;
    STToken previous;

    int error_count;
    char last_error[ST_PARSER_MAX_ERROR];

} STParser;


/*
 * Initialize parser.
 */
void st_parser_init(
    STParser* parser,
    const char* source
);


/*
 * Read the next token.
 */
void st_parser_advance(
    STParser* parser
);


/*
 * Check current token.
 */
int st_parser_check(
    const STParser* parser,
    STTokenType type
);


/*
 * Consume token if it matches.
 */
int st_parser_match(
    STParser* parser,
    STTokenType type
);


/*
 * Require a token.
 */
int st_parser_expect(
    STParser* parser,
    STTokenType type,
    const char* message
);


/*
 * Parse an expression.
 */
STExpression* st_parser_expression(
    STParser* parser
);


/*
 * Parse a primary expression.
 */
STExpression* st_parser_primary(
    STParser* parser
);


/*
 * Parse a unary expression.
 */
STExpression* st_parser_unary(
    STParser* parser
);


/*
 * Parse a binary expression.
 */
STExpression* st_parser_binary(
    STParser* parser,
    int minimum_precedence
);


/*
 * Parse one statement.
 */
STStatement* st_parser_statement(
    STParser* parser
);


/*
 * Parse the entire program.
 */
STStatement* st_parser_program(
    STParser* parser
);


/*
 * Check parser errors.
 */
int st_parser_has_errors(
    const STParser* parser
);


/*
 * Get last error.
 */
const char* st_parser_last_error(
    const STParser* parser
);


/*
 * Get number of errors.
 */
int st_parser_error_count(
    const STParser* parser
);

#endif