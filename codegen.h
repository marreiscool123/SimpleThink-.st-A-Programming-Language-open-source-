#ifndef ST_CODEGEN_H
#define ST_CODEGEN_H

#include "ast.h"

#define ST_CODEGEN_MAX_OUTPUT 65536

typedef struct STCodeGenerator
{
    STASTNode* root;

    char* output;
    int output_size;
    int output_capacity;

    int error_count;
    char last_error[512];

} STCodeGenerator;


/*
 * Initialize code generator.
 */
void st_codegen_init(
    STCodeGenerator* codegen
);


/*
 * Free code generator.
 */
void st_codegen_free(
    STCodeGenerator* codegen
);


/*
 * Set AST root.
 */
int st_codegen_set_root(
    STCodeGenerator* codegen,
    STASTNode* root
);


/*
 * Generate code from the AST.
 */
int st_codegen_generate(
    STCodeGenerator* codegen
);


/*
 * Get generated code.
 */
const char* st_codegen_output(
    const STCodeGenerator* codegen
);


/*
 * Get generated output size.
 */
int st_codegen_output_size(
    const STCodeGenerator* codegen
);


/*
 * Check for errors.
 */
int st_codegen_has_errors(
    const STCodeGenerator* codegen
);


/*
 * Get last error.
 */
const char* st_codegen_last_error(
    const STCodeGenerator* codegen
);

#endif