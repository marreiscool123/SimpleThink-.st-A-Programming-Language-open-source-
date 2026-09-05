#include "connect.h"

#include <string.h>
#include <stdio.h>


static void st_connect_add_error(
    STContext* context,
    STErrorType type,
    const char* message
)
{
    if (context == NULL)
        return;

    st_error_add(
        &context->errors,
        type,
        0,
        0,
        message
    );
}


int st_connect_init(
    STContext* context
)
{
    if (context == NULL)
        return 0;

    memset(
        context,
        0,
        sizeof(STContext)
    );

    /* Compiler */
    st_compiler_init(
        &context->compiler
    );

    /* Errors */
    st_error_init(
        &context->errors
    );

    /* Code generator */
    st_codegen_init(
        &context->codegen
    );

    /* Runtime */
    if (!st_runtime_init(
            &context->runtime))
    {
        st_connect_add_error(
            context,
            ST_ERROR_INTERNAL,
            "Failed to initialize runtime."
        );

        return 0;
    }

    /* Interpreter */
    if (!st_interpreter_init(
            &context->interpreter))
    {
        st_connect_add_error(
            context,
            ST_ERROR_INTERNAL,
            "Failed to initialize interpreter."
        );

        st_runtime_shutdown(
            &context->runtime
        );

        return 0;
    }

    /* Modules */
    st_modules_init(
        &context->modules
    );

    /* Imports */
    st_imports_init(
        &context->imports
    );

    /* Project */
    memset(
        &context->project,
        0,
        sizeof(STProject)
    );

    /* Network */
    memset(
        &context->network,
        0,
        sizeof(STNetwork)
    );

    /* Server */
    memset(
        &context->server,
        0,
        sizeof(STServer)
    );

    /* Client */
    memset(
        &context->client,
        0,
        sizeof(STClient)
    );

    context->ast = NULL;

    context->initialized = 1;
    context->compiled = 0;
    context->running = 0;

    return 1;
}


void st_connect_shutdown(
    STContext* context
)
{
    if (context == NULL)
        return;

    context->running = 0;

    /* AST */
    if (context->ast != NULL)
    {
        st_ast_free(
            context->ast
        );

        context->ast = NULL;
    }

    /* Network */
    st_client_disconnect(
        &context->client
    );

    st_server_stop(
        &context->server
    );

    st_server_shutdown(
        &context->server
    );

    st_network_shutdown(
        &context->network
    );

    /* Project */
    if (context->project.initialized)
    {
        st_project_shutdown(
            &context->project
        );
    }

    /* Imports */
    st_imports_clear(
        &context->imports
    );

    /* Modules */
    st_modules_free(
        &context->modules
    );

    /* Interpreter */
    st_interpreter_shutdown(
        &context->interpreter
    );

    /* Runtime */
    st_runtime_shutdown(
        &context->runtime
    );

    /* Code generator */
    st_codegen_free(
        &context->codegen
    );

    /* Errors */
    st_error_free(
        &context->errors
    );

    /* Compiler */
    st_compiler_free(
        &context->compiler
    );

    context->initialized = 0;
    context->compiled = 0;
}


int st_connect_compile(
    STContext* context,
    const char* input_file,
    const char* output_file
)
{
    STStatement* statements;

    if (context == NULL ||
        input_file == NULL ||
        output_file == NULL)
    {
        return 0;
    }

    if (!context->initialized)
        return 0;

    context->compiled = 0;
    context->running = 0;

    /* Clear previous errors */
    st_error_clear(
        &context->errors
    );

    /* Remove old AST */
    if (context->ast != NULL)
    {
        st_ast_free(
            context->ast
        );

        context->ast = NULL;
    }

    /* Load source */
    if (!st_compiler_load(
            &context->compiler,
            input_file))
    {
        st_connect_add_error(
            context,
            ST_ERROR_COMPILE,
            "Failed to load source file."
        );

        return 0;
    }

    /*
        Lexer + Parser
    */

    st_parser_init(
        &context->parser,
        context->compiler.source
    );

    statements =
        st_parser_program(
            &context->parser
        );

    if (st_parser_has_errors(
            &context->parser))
    {
        st_connect_add_error(
            context,
            ST_ERROR_SYNTAX,
            st_parser_last_error(
                &context->parser
            )
        );

        if (statements != NULL)
        {
            st_statement_free(
                statements
            );
        }

        return 0;
    }

    /*
        Statements -> AST
    */

    context->ast =
        st_ast_from_statements(
            statements
        );

    if (context->ast == NULL)
    {
        st_connect_add_error(
            context,
            ST_ERROR_INTERNAL,
            "Failed to create AST."
        );

        return 0;
    }

    /*
        AST -> Generated Code
    */

    if (!st_codegen_set_root(
            &context->codegen,
            context->ast))
    {
        st_connect_add_error(
            context,
            ST_ERROR_COMPILE,
            "Failed to set code generator root."
        );

        return 0;
    }

    if (!st_codegen_generate(
            &context->codegen))
    {
        st_connect_add_error(
            context,
            ST_ERROR_COMPILE,
            st_codegen_last_error(
                &context->codegen
            )
        );

        return 0;
    }

    context->compiled = 1;
    context->compiler.compiled = 1;

    /*
        The compiler currently
        produces intermediate output.
        The executable is written
        during st_connect_build().
    */

    (void)output_file;

    return 1;
}


int st_connect_run(
    STContext* context
)
{
    if (context == NULL)
        return 0;

    if (!context->initialized)
        return 0;

    if (!context->compiled)
        return 0;

    if (context->ast == NULL)
        return 0;

    /*
        Reset interpreter state
        before execution.
    */

    st_environment_clear(
        &context->interpreter.global
    );

    context->interpreter.current =
        &context->interpreter.global;

    context->running = 1;

    if (!st_interpreter_execute(
            &context->interpreter,
            context->ast))
    {
        st_connect_add_error(
            context,
            ST_ERROR_INTERNAL,
            st_interpreter_last_error(
                &context->interpreter
            )
        );

        context->running = 0;

        return 0;
    }

    context->running = 0;

    return 1;
}


int st_connect_build(
    STContext* context,
    const char* input_file,
    const char* output_file
)
{
    if (context == NULL ||
        input_file == NULL ||
        output_file == NULL)
    {
        return 0;
    }

    if (!st_connect_compile(
            context,
            input_file,
            output_file))
    {
        return 0;
    }

    if (!st_compiler_write_exe(
            &context->compiler,
            output_file))
    {
        st_connect_add_error(
            context,
            ST_ERROR_COMPILE,
            "Failed to write output executable."
        );

        return 0;
    }

    return 1;
}


/* Compiler */

STCompiler* st_connect_compiler(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->compiler;
}


/* Errors */

STErrorList* st_connect_errors(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->errors;
}


/* Parser */

STParser* st_connect_parser(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->parser;
}


/* AST */

STASTNode* st_connect_ast(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return context->ast;
}


/* Code generator */

STCodeGenerator* st_connect_codegen(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->codegen;
}


/* Runtime */

STRuntime* st_connect_runtime(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->runtime;
}


/* Interpreter */

STInterpreter* st_connect_interpreter(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->interpreter;
}


/* Project */

STProject* st_connect_project(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->project;
}


/* Modules */

STModuleTable* st_connect_modules(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->modules;
}


/* Imports */

STImportTable* st_connect_imports(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->imports;
}


/* Network */

STNetwork* st_connect_network(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->network;
}


/* Server */

STServer* st_connect_server(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->server;
}


/* Client */

STClient* st_connect_client(
    STContext* context
)
{
    if (context == NULL)
        return NULL;

    return &context->client;
}


/* State */

int st_connect_is_initialized(
    const STContext* context
)
{
    if (context == NULL)
        return 0;

    return context->initialized;
}


int st_connect_is_compiled(
    const STContext* context
)
{
    if (context == NULL)
        return 0;

    return context->compiled;
}


int st_connect_is_running(
    const STContext* context
)
{
    if (context == NULL)
        return 0;

    return context->running;
}