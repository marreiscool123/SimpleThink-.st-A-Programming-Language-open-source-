#include "lexer.h"

#include <ctype.h>
#include <string.h>


static char st_lexer_peek(
    const STLexer* lexer
)
{
    if (
        lexer == NULL ||
        lexer->source == NULL
    )
    {
        return '\0';
    }

    if (
        lexer->position >= lexer->length
    )
    {
        return '\0';
    }

    return lexer->source[
        lexer->position
    ];
}


static char st_lexer_peek_next(
    const STLexer* lexer
)
{
    if (
        lexer == NULL ||
        lexer->source == NULL
    )
    {
        return '\0';
    }

    if (
        lexer->position + 1 >=
        lexer->length
    )
    {
        return '\0';
    }

    return lexer->source[
        lexer->position + 1
    ];
}


static char st_lexer_advance(
    STLexer* lexer
)
{
    char character;

    if (
        lexer == NULL ||
        lexer->source == NULL
    )
    {
        return '\0';
    }

    if (
        lexer->position >= lexer->length
    )
    {
        return '\0';
    }

    character =
        lexer->source[
            lexer->position
        ];

    lexer->position++;
    lexer->column++;

    return character;
}


static void st_lexer_new_line(
    STLexer* lexer
)
{
    lexer->line++;
    lexer->column = 1;
}


static void st_token_set_text(
    STToken* token,
    const char* source,
    long start,
    long end
)
{
    long length;

    if (
        token == NULL ||
        source == NULL
    )
    {
        return;
    }

    length = end - start;

    if (
        length >= ST_TOKEN_MAX_TEXT
    )
    {
        length =
            ST_TOKEN_MAX_TEXT - 1;
    }

    if (length < 0)
        length = 0;

    memcpy(
        token->text,
        source + start,
        (size_t)length
    );

    token->text[length] = '\0';
}


static STToken st_make_token(
    STTokenType type,
    int line,
    int column,
    const char* text
)
{
    STToken token;

    memset(
        &token,
        0,
        sizeof(STToken)
    );

    token.type = type;
    token.line = line;
    token.column = column;

    if (text != NULL)
    {
        strncpy(
            token.text,
            text,
            ST_TOKEN_MAX_TEXT - 1
        );

        token.text[
            ST_TOKEN_MAX_TEXT - 1
        ] = '\0';
    }

    return token;
}


void st_lexer_init(
    STLexer* lexer,
    const char* source
)
{
    if (lexer == NULL)
        return;

    memset(
        lexer,
        0,
        sizeof(STLexer)
    );

    lexer->source = source;

    if (source != NULL)
        lexer->length =
            (long)strlen(source);

    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;

    lexer->current =
        st_make_token(
            ST_TOKEN_EOF,
            1,
            1,
            NULL
        );

    lexer->previous =
        lexer->current;
}


static void st_lexer_skip_whitespace(
    STLexer* lexer
)
{
    char character;

    for (;;)
    {
        character =
            st_lexer_peek(lexer);

        if (
            character == ' ' ||
            character == '\t' ||
            character == '\r'
        )
        {
            st_lexer_advance(lexer);
            continue;
        }

        if (character == '\n')
        {
            st_lexer_advance(lexer);
            st_lexer_new_line(lexer);
            continue;
        }

        /*
         * Single-line comment.
         */
        if (
            character == '/' &&
            st_lexer_peek_next(lexer) == '/'
        )
        {
            while (
                st_lexer_peek(lexer) != '\0' &&
                st_lexer_peek(lexer) != '\n'
            )
            {
                st_lexer_advance(lexer);
            }

            continue;
        }

        /*
         * Multi-line comment.
         */
        if (
            character == '/' &&
            st_lexer_peek_next(lexer) == '*'
        )
        {
            st_lexer_advance(lexer);
            st_lexer_advance(lexer);

            while (
                st_lexer_peek(lexer) != '\0'
            )
            {
                if (
                    st_lexer_peek(lexer) == '*' &&
                    st_lexer_peek_next(lexer) == '/'
                )
                {
                    st_lexer_advance(lexer);
                    st_lexer_advance(lexer);
                    break;
                }

                if (
                    st_lexer_peek(lexer) == '\n'
                )
                {
                    st_lexer_advance(lexer);
                    st_lexer_new_line(lexer);
                }
                else
                {
                    st_lexer_advance(lexer);
                }
            }

            continue;
        }

        break;
    }
}


int st_lexer_keyword(
    const char* text,
    STTokenType* type
)
{
    if (
        text == NULL ||
        type == NULL
    )
    {
        return 0;
    }


    if (strcmp(text, "true") == 0)
    {
        *type = ST_TOKEN_TRUE;
        return 1;
    }

    if (strcmp(text, "false") == 0)
    {
        *type = ST_TOKEN_FALSE;
        return 1;
    }


    if (strcmp(text, "int") == 0)
    {
        *type = ST_TOKEN_INT;
        return 1;
    }

    if (strcmp(text, "float") == 0)
    {
        *type = ST_TOKEN_FLOAT_TYPE;
        return 1;
    }

    if (strcmp(text, "string") == 0)
    {
        *type = ST_TOKEN_STRING_TYPE;
        return 1;
    }

    if (strcmp(text, "bool") == 0)
    {
        *type = ST_TOKEN_BOOL;
        return 1;
    }

    if (strcmp(text, "char") == 0)
    {
        *type = ST_TOKEN_CHAR;
        return 1;
    }


    if (strcmp(text, "if") == 0)
    {
        *type = ST_TOKEN_IF;
        return 1;
    }

    if (strcmp(text, "else") == 0)
    {
        *type = ST_TOKEN_ELSE;
        return 1;
    }

    if (strcmp(text, "switch") == 0)
    {
        *type = ST_TOKEN_SWITCH;
        return 1;
    }

    if (strcmp(text, "case") == 0)
    {
        *type = ST_TOKEN_CASE;
        return 1;
    }

    if (strcmp(text, "default") == 0)
    {
        *type = ST_TOKEN_DEFAULT;
        return 1;
    }


    if (strcmp(text, "while") == 0)
    {
        *type = ST_TOKEN_WHILE;
        return 1;
    }

    if (strcmp(text, "for") == 0)
    {
        *type = ST_TOKEN_FOR;
        return 1;
    }

    if (strcmp(text, "do") == 0)
    {
        *type = ST_TOKEN_DO;
        return 1;
    }

    if (strcmp(text, "break") == 0)
    {
        *type = ST_TOKEN_BREAK;
        return 1;
    }

    if (strcmp(text, "continue") == 0)
    {
        *type = ST_TOKEN_CONTINUE;
        return 1;
    }


    if (strcmp(text, "function") == 0)
    {
        *type = ST_TOKEN_FUNCTION;
        return 1;
    }

    if (strcmp(text, "return") == 0)
    {
        *type = ST_TOKEN_RETURN;
        return 1;
    }


    return 0;
}


static STToken st_lexer_identifier(
    STLexer* lexer
)
{
    long start;
    STTokenType keyword_type;
    STToken token;

    start = lexer->position;

    while (
        isalnum(
            (unsigned char)
            st_lexer_peek(lexer)
        ) ||
        st_lexer_peek(lexer) == '_'
    )
    {
        st_lexer_advance(lexer);
    }

    token = st_make_token(
        ST_TOKEN_IDENTIFIER,
        lexer->line,
        lexer->column,
        NULL
    );

    st_token_set_text(
        &token,
        lexer->source,
        start,
        lexer->position
    );

    if (
        st_lexer_keyword(
            token.text,
            &keyword_type
        )
    )
    {
        token.type = keyword_type;
    }

    return token;
}


static STToken st_lexer_number(
    STLexer* lexer
)
{
    long start;
    int is_float;
    STToken token;

    start = lexer->position;
    is_float = 0;

    while (
        isdigit(
            (unsigned char)
            st_lexer_peek(lexer)
        )
    )
    {
        st_lexer_advance(lexer);
    }

    if (
        st_lexer_peek(lexer) == '.' &&
        isdigit(
            (unsigned char)
            st_lexer_peek_next(lexer)
        )
    )
    {
        is_float = 1;

        st_lexer_advance(lexer);

        while (
            isdigit(
                (unsigned char)
                st_lexer_peek(lexer)
            )
        )
        {
            st_lexer_advance(lexer);
        }
    }

    token = st_make_token(
        is_float
            ? ST_TOKEN_FLOAT
            : ST_TOKEN_INTEGER,
        lexer->line,
        lexer->column,
        NULL
    );

    st_token_set_text(
        &token,
        lexer->source,
        start,
        lexer->position
    );

    return token;
}


static STToken st_lexer_string(
    STLexer* lexer
)
{
    long start;
    STToken token;

    st_lexer_advance(lexer);

    start = lexer->position;

    while (
        st_lexer_peek(lexer) != '\0' &&
        st_lexer_peek(lexer) != '"'
    )
    {
        if (
            st_lexer_peek(lexer) == '\\' &&
            st_lexer_peek_next(lexer) != '\0'
        )
        {
            st_lexer_advance(lexer);
            st_lexer_advance(lexer);
            continue;
        }

        st_lexer_advance(lexer);
    }

    token = st_make_token(
        ST_TOKEN_STRING,
        lexer->line,
        lexer->column,
        NULL
    );

    st_token_set_text(
        &token,
        lexer->source,
        start,
        lexer->position
    );

    if (
        st_lexer_peek(lexer) == '"'
    )
    {
        st_lexer_advance(lexer);
    }
    else
    {
        lexer->error_count++;
    }

    return token;
}


static STToken st_lexer_character(
    STLexer* lexer
)
{
    long start;
    STToken token;

    st_lexer_advance(lexer);

    start = lexer->position;

    while (
        st_lexer_peek(lexer) != '\0' &&
        st_lexer_peek(lexer) != '\''
    )
    {
        if (
            st_lexer_peek(lexer) == '\\' &&
            st_lexer_peek_next(lexer) != '\0'
        )
        {
            st_lexer_advance(lexer);
            st_lexer_advance(lexer);
            continue;
        }

        st_lexer_advance(lexer);
    }

    token = st_make_token(
        ST_TOKEN_CHARACTER,
        lexer->line,
        lexer->column,
        NULL
    );

    st_token_set_text(
        &token,
        lexer->source,
        start,
        lexer->position
    );

    if (
        st_lexer_peek(lexer) == '\''
    )
    {
        st_lexer_advance(lexer);
    }
    else
    {
        lexer->error_count++;
    }

    return token;
}


STToken st_lexer_next(
    STLexer* lexer
)
{
    char character;
    STToken token;

    if (lexer == NULL)
    {
        return st_make_token(
            ST_TOKEN_EOF,
            0,
            0,
            NULL
        );
    }

    st_lexer_skip_whitespace(lexer);

    lexer->previous =
        lexer->current;

    character =
        st_lexer_peek(lexer);

    if (character == '\0')
    {
        token = st_make_token(
            ST_TOKEN_EOF,
            lexer->line,
            lexer->column,
            NULL
        );

        lexer->current = token;

        return token;
    }


    if (
        isalpha(
            (unsigned char)character
        ) ||
        character == '_'
    )
    {
        token =
            st_lexer_identifier(lexer);

        lexer->current = token;

        return token;
    }


    if (
        isdigit(
            (unsigned char)character
        )
    )
    {
        token =
            st_lexer_number(lexer);

        lexer->current = token;

        return token;
    }


    if (character == '"')
    {
        token =
            st_lexer_string(lexer);

        lexer->current = token;

        return token;
    }


    if (character == '\'')
    {
        token =
            st_lexer_character(lexer);

        lexer->current = token;

        return token;
    }


    /*
     * Two-character operators.
     */

    if (
        character == '=' &&
        st_lexer_peek_next(lexer) == '='
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_EQUAL,
            lexer->line,
            lexer->column - 2,
            "=="
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '!' &&
        st_lexer_peek_next(lexer) == '='
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_NOT_EQUAL,
            lexer->line,
            lexer->column - 2,
            "!="
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '<' &&
        st_lexer_peek_next(lexer) == '='
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_LESS_EQUAL,
            lexer->line,
            lexer->column - 2,
            "<="
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '>' &&
        st_lexer_peek_next(lexer) == '='
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_GREATER_EQUAL,
            lexer->line,
            lexer->column - 2,
            ">="
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '&' &&
        st_lexer_peek_next(lexer) == '&'
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_AND,
            lexer->line,
            lexer->column - 2,
            "&&"
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '|' &&
        st_lexer_peek_next(lexer) == '|'
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_OR,
            lexer->line,
            lexer->column - 2,
            "||"
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '+' &&
        st_lexer_peek_next(lexer) == '+'
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_INCREMENT,
            lexer->line,
            lexer->column - 2,
            "++"
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '-' &&
        st_lexer_peek_next(lexer) == '-'
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_DECREMENT,
            lexer->line,
            lexer->column - 2,
            "--"
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '+' &&
        st_lexer_peek_next(lexer) == '='
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_ADD_ASSIGN,
            lexer->line,
            lexer->column - 2,
            "+="
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '-' &&
        st_lexer_peek_next(lexer) == '='
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_SUB_ASSIGN,
            lexer->line,
            lexer->column - 2,
            "-="
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '*' &&
        st_lexer_peek_next(lexer) == '='
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_MUL_ASSIGN,
            lexer->line,
            lexer->column - 2,
            "*="
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '/' &&
        st_lexer_peek_next(lexer) == '='
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_DIV_ASSIGN,
            lexer->line,
            lexer->column - 2,
            "/="
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '%' &&
        st_lexer_peek_next(lexer) == '='
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_MOD_ASSIGN,
            lexer->line,
            lexer->column - 2,
            "%="
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '<' &&
        st_lexer_peek_next(lexer) == '<'
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_SHIFT_LEFT,
            lexer->line,
            lexer->column - 2,
            "<<"
        );

        lexer->current = token;
        return token;
    }


    if (
        character == '>' &&
        st_lexer_peek_next(lexer) == '>'
    )
    {
        st_lexer_advance(lexer);
        st_lexer_advance(lexer);

        token = st_make_token(
            ST_TOKEN_SHIFT_RIGHT,
            lexer->line,
            lexer->column - 2,
            ">>"
        );

        lexer->current = token;
        return token;
    }


    /*
     * Single-character tokens.
     */

    st_lexer_advance(lexer);

    switch (character)
    {
        case '+':
            token = st_make_token(
                ST_TOKEN_PLUS,
                lexer->line,
                lexer->column - 1,
                "+"
            );
            break;

        case '-':
            token = st_make_token(
                ST_TOKEN_MINUS,
                lexer->line,
                lexer->column - 1,
                "-"
            );
            break;

        case '*':
            token = st_make_token(
                ST_TOKEN_STAR,
                lexer->line,
                lexer->column - 1,
                "*"
            );
            break;

        case '/':
            token = st_make_token(
                ST_TOKEN_SLASH,
                lexer->line,
                lexer->column - 1,
                "/"
            );
            break;

        case '%':
            token = st_make_token(
                ST_TOKEN_PERCENT,
                lexer->line,
                lexer->column - 1,
                "%"
            );
            break;

        case '<':
            token = st_make_token(
                ST_TOKEN_LESS,
                lexer->line,
                lexer->column - 1,
                "<"
            );
            break;

        case '>':
            token = st_make_token(
                ST_TOKEN_GREATER,
                lexer->line,
                lexer->column - 1,
                ">"
            );
            break;

        case '!':
            token = st_make_token(
                ST_TOKEN_NOT,
                lexer->line,
                lexer->column - 1,
                "!"
            );
            break;

        case '=':
            token = st_make_token(
                ST_TOKEN_ASSIGN,
                lexer->line,
                lexer->column - 1,
                "="
            );
            break;

        case '&':
            token = st_make_token(
                ST_TOKEN_BIT_AND,
                lexer->line,
                lexer->column - 1,
                "&"
            );
            break;

        case '|':
            token = st_make_token(
                ST_TOKEN_BIT_OR,
                lexer->line,
                lexer->column - 1,
                "|"
            );
            break;

        case '^':
            token = st_make_token(
                ST_TOKEN_BIT_XOR,
                lexer->line,
                lexer->column - 1,
                "^"
            );
            break;

        case '~':
            token = st_make_token(
                ST_TOKEN_BIT_NOT,
                lexer->line,
                lexer->column - 1,
                "~"
            );
            break;

        case '(':
            token = st_make_token(
                ST_TOKEN_LEFT_PAREN,
                lexer->line,
                lexer->column - 1,
                "("
            );
            break;

        case ')':
            token = st_make_token(
                ST_TOKEN_RIGHT_PAREN,
                lexer->line,
                lexer->column - 1,
                ")"
            );
            break;

        case '{':
            token = st_make_token(
                ST_TOKEN_LEFT_BRACE,
                lexer->line,
                lexer->column - 1,
                "{"
            );
            break;

        case '}':
            token = st_make_token(
                ST_TOKEN_RIGHT_BRACE,
                lexer->line,
                lexer->column - 1,
                "}"
            );
            break;

        case '[':
            token = st_make_token(
                ST_TOKEN_LEFT_BRACKET,
                lexer->line,
                lexer->column - 1,
                "["
            );
            break;

        case ']':
            token = st_make_token(
                ST_TOKEN_RIGHT_BRACKET,
                lexer->line,
                lexer->column - 1,
                "]"
            );
            break;

        case ',':
            token = st_make_token(
                ST_TOKEN_COMMA,
                lexer->line,
                lexer->column - 1,
                ","
            );
            break;

        case '.':
            token = st_make_token(
                ST_TOKEN_DOT,
                lexer->line,
                lexer->column - 1,
                "."
            );
            break;

        case ':':
            token = st_make_token(
                ST_TOKEN_COLON,
                lexer->line,
                lexer->column - 1,
                ":"
            );
            break;

        case ';':
            token = st_make_token(
                ST_TOKEN_SEMICOLON,
                lexer->line,
                lexer->column - 1,
                ";"
            );
            break;

        default:
            token = st_make_token(
                ST_TOKEN_EOF,
                lexer->line,
                lexer->column - 1,
                NULL
            );

            lexer->error_count++;
            break;
    }

    lexer->current = token;

    return token;
}


const STToken* st_lexer_current(
    const STLexer* lexer
)
{
    if (lexer == NULL)
        return NULL;

    return &lexer->current;
}


int st_lexer_at_end(
    const STLexer* lexer
)
{
    if (lexer == NULL)
        return 1;

    return lexer->position >= lexer->length;
}


const char* st_token_type_name(
    STTokenType type
)
{
    switch (type)
    {
        case ST_TOKEN_EOF:
            return "end of file";

        case ST_TOKEN_IDENTIFIER:
            return "identifier";

        case ST_TOKEN_INTEGER:
            return "integer";

        case ST_TOKEN_FLOAT:
            return "float";

        case ST_TOKEN_STRING:
            return "string";

        case ST_TOKEN_CHARACTER:
            return "character";

        case ST_TOKEN_TRUE:
            return "true";

        case ST_TOKEN_FALSE:
            return "false";

        case ST_TOKEN_INT:
            return "int";

        case ST_TOKEN_FLOAT_TYPE:
            return "float";

        case ST_TOKEN_STRING_TYPE:
            return "string";

        case ST_TOKEN_BOOL:
            return "bool";

        case ST_TOKEN_CHAR:
            return "char";

        case ST_TOKEN_IF:
            return "if";

        case ST_TOKEN_ELSE:
            return "else";

        case ST_TOKEN_SWITCH:
            return "switch";

        case ST_TOKEN_CASE:
            return "case";

        case ST_TOKEN_DEFAULT:
            return "default";

        case ST_TOKEN_WHILE:
            return "while";

        case ST_TOKEN_FOR:
            return "for";

        case ST_TOKEN_DO:
            return "do";

        case ST_TOKEN_BREAK:
            return "break";

        case ST_TOKEN_CONTINUE:
            return "continue";

        case ST_TOKEN_FUNCTION:
            return "function";

        case ST_TOKEN_RETURN:
            return "return";

        case ST_TOKEN_PLUS:
            return "+";

        case ST_TOKEN_MINUS:
            return "-";

        case ST_TOKEN_STAR:
            return "*";

        case ST_TOKEN_SLASH:
            return "/";

        case ST_TOKEN_PERCENT:
            return "%";

        case ST_TOKEN_EQUAL:
            return "==";

        case ST_TOKEN_NOT_EQUAL:
            return "!=";

        case ST_TOKEN_LESS:
            return "<";

        case ST_TOKEN_LESS_EQUAL:
            return "<=";

        case ST_TOKEN_GREATER:
            return ">";

        case ST_TOKEN_GREATER_EQUAL:
            return ">=";

        case ST_TOKEN_AND:
            return "&&";

        case ST_TOKEN_OR:
            return "||";

        case ST_TOKEN_NOT:
            return "!";

        case ST_TOKEN_ASSIGN:
            return "=";

        case ST_TOKEN_ADD_ASSIGN:
            return "+=";

        case ST_TOKEN_SUB_ASSIGN:
            return "-=";

        case ST_TOKEN_MUL_ASSIGN:
            return "*=";

        case ST_TOKEN_DIV_ASSIGN:
            return "/=";

        case ST_TOKEN_MOD_ASSIGN:
            return "%=";

        case ST_TOKEN_INCREMENT:
            return "++";

        case ST_TOKEN_DECREMENT:
            return "--";

        case ST_TOKEN_BIT_AND:
            return "&";

        case ST_TOKEN_BIT_OR:
            return "|";

        case ST_TOKEN_BIT_XOR:
            return "^";

        case ST_TOKEN_BIT_NOT:
            return "~";

        case ST_TOKEN_SHIFT_LEFT:
            return "<<";

        case ST_TOKEN_SHIFT_RIGHT:
            return ">>";

        case ST_TOKEN_LEFT_PAREN:
            return "(";

        case ST_TOKEN_RIGHT_PAREN:
            return ")";

        case ST_TOKEN_LEFT_BRACE:
            return "{";

        case ST_TOKEN_RIGHT_BRACE:
            return "}";

        case ST_TOKEN_LEFT_BRACKET:
            return "[";

        case ST_TOKEN_RIGHT_BRACKET:
            return "]";

        case ST_TOKEN_COMMA:
            return ",";

        case ST_TOKEN_DOT:
            return ".";

        case ST_TOKEN_COLON:
            return ":";

        case ST_TOKEN_SEMICOLON:
            return ";";

        default:
            return "unknown";
    }
}


const char* st_token_symbol(
    STTokenType type
)
{
    return st_token_type_name(type);
}