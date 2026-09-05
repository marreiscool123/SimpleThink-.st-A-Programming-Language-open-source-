#ifndef ST_LEXER_H
#define ST_LEXER_H

#define ST_TOKEN_MAX_TEXT 256


typedef enum STTokenType
{
    ST_TOKEN_EOF = 0,

    ST_TOKEN_IDENTIFIER,

    ST_TOKEN_INTEGER,
    ST_TOKEN_FLOAT,
    ST_TOKEN_STRING,
    ST_TOKEN_CHARACTER,

    ST_TOKEN_TRUE,
    ST_TOKEN_FALSE,

    ST_TOKEN_INT,
    ST_TOKEN_FLOAT_TYPE,
    ST_TOKEN_STRING_TYPE,
    ST_TOKEN_BOOL,
    ST_TOKEN_CHAR,

    ST_TOKEN_IF,
    ST_TOKEN_ELSE,
    ST_TOKEN_SWITCH,
    ST_TOKEN_CASE,
    ST_TOKEN_DEFAULT,

    ST_TOKEN_WHILE,
    ST_TOKEN_FOR,
    ST_TOKEN_DO,
    ST_TOKEN_BREAK,
    ST_TOKEN_CONTINUE,

    ST_TOKEN_FUNCTION,
    ST_TOKEN_RETURN,

    ST_TOKEN_PLUS,
    ST_TOKEN_MINUS,
    ST_TOKEN_STAR,
    ST_TOKEN_SLASH,
    ST_TOKEN_PERCENT,

    ST_TOKEN_EQUAL,
    ST_TOKEN_NOT_EQUAL,
    ST_TOKEN_LESS,
    ST_TOKEN_LESS_EQUAL,
    ST_TOKEN_GREATER,
    ST_TOKEN_GREATER_EQUAL,

    ST_TOKEN_AND,
    ST_TOKEN_OR,
    ST_TOKEN_NOT,

    ST_TOKEN_ASSIGN,
    ST_TOKEN_ADD_ASSIGN,
    ST_TOKEN_SUB_ASSIGN,
    ST_TOKEN_MUL_ASSIGN,
    ST_TOKEN_DIV_ASSIGN,
    ST_TOKEN_MOD_ASSIGN,

    ST_TOKEN_INCREMENT,
    ST_TOKEN_DECREMENT,

    ST_TOKEN_BIT_AND,
    ST_TOKEN_BIT_OR,
    ST_TOKEN_BIT_XOR,
    ST_TOKEN_BIT_NOT,
    ST_TOKEN_SHIFT_LEFT,
    ST_TOKEN_SHIFT_RIGHT,

    ST_TOKEN_LEFT_PAREN,
    ST_TOKEN_RIGHT_PAREN,

    ST_TOKEN_LEFT_BRACE,
    ST_TOKEN_RIGHT_BRACE,

    ST_TOKEN_LEFT_BRACKET,
    ST_TOKEN_RIGHT_BRACKET,

    ST_TOKEN_COMMA,
    ST_TOKEN_DOT,
    ST_TOKEN_COLON,
    ST_TOKEN_SEMICOLON

} STTokenType;


typedef struct STToken
{
    STTokenType type;

    char text[ST_TOKEN_MAX_TEXT];

    int line;
    int column;

} STToken;


typedef struct STLexer
{
    const char* source;

    long length;
    long position;

    int line;
    int column;

    STToken current;
    STToken previous;

    int error_count;

} STLexer;


/*
 * Initialize the lexer.
 */
void st_lexer_init(
    STLexer* lexer,
    const char* source
);


/*
 * Get the next token.
 */
STToken st_lexer_next(
    STLexer* lexer
);


/*
 * Get the current token.
 */
const STToken* st_lexer_current(
    const STLexer* lexer
);


/*
 * Check whether the lexer reached EOF.
 */
int st_lexer_at_end(
    const STLexer* lexer
);


/*
 * Get token type name.
 */
const char* st_token_type_name(
    STTokenType type
);


/*
 * Get token symbol.
 */
const char* st_token_symbol(
    STTokenType type
);


/*
 * Check whether text is a keyword.
 */
int st_lexer_keyword(
    const char* text,
    STTokenType* type
);

#endif /* ST_LEXER_H */