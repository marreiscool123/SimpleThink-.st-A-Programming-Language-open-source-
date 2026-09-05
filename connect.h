#ifndef ST_CONNECT_H
#define ST_CONNECT_H

#include "compiler.h"
#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"

#include "variables.h"
#include "functions.h"
#include "loops.h"
#include "conditions.h"
#include "operators.h"
#include "expressions.h"
#include "statements.h"
#include "types.h"

#include "memory.h"
#include "strings.h"
#include "arrays.h"
#include "structs.h"

#include "files.h"
#include "console.h"

#include "gui.h"
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "window.h"

#include "math.h"
#include "random.h"
#include "time.h"

#include "game.h"
#include "collision.h"

#include "runtime.h"

#include "module.h"
#include "imports.h"
#include "project.h"

#include "network.h"
#include "server.h"
#include "client.h"
#include "packet.h"

#include "value.h"
#include "environment.h"
#include "interpreter.h"


typedef struct STContext
{
    /* Compiler pipeline */
    STCompiler compiler;
    STErrorList errors;

    STParser parser;
    STASTNode* ast;

    STCodeGenerator codegen;

    /* Runtime */
    STRuntime runtime;

    /* Interpreter */
    STInterpreter interpreter;

    /* Project system */
    STProject project;

    /* Module system */
    STModuleTable modules;
    STImportTable imports;

    /* Network system */
    STNetwork network;
    STServer server;
    STClient client;

    /* State */
    int initialized;
    int compiled;
    int running;

} STContext;


/* Context lifecycle */

int st_connect_init(
    STContext* context
);

void st_connect_shutdown(
    STContext* context
);


/* Compiler pipeline */

int st_connect_compile(
    STContext* context,
    const char* input_file,
    const char* output_file
);

int st_connect_run(
    STContext* context
);

int st_connect_build(
    STContext* context,
    const char* input_file,
    const char* output_file
);


/* Accessors */

STCompiler* st_connect_compiler(
    STContext* context
);

STErrorList* st_connect_errors(
    STContext* context
);

STParser* st_connect_parser(
    STContext* context
);

STASTNode* st_connect_ast(
    STContext* context
);

STCodeGenerator* st_connect_codegen(
    STContext* context
);

STRuntime* st_connect_runtime(
    STContext* context
);

STInterpreter* st_connect_interpreter(
    STContext* context
);

STProject* st_connect_project(
    STContext* context
);

STModuleTable* st_connect_modules(
    STContext* context
);

STImportTable* st_connect_imports(
    STContext* context
);

STNetwork* st_connect_network(
    STContext* context
);

STServer* st_connect_server(
    STContext* context
);

STClient* st_connect_client(
    STContext* context
);


/* State */

int st_connect_is_initialized(
    const STContext* context
);

int st_connect_is_compiled(
    const STContext* context
);

int st_connect_is_running(
    const STContext* context
);

#endif