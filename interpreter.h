#ifndef ST_INTERPRETER_H
#define ST_INTERPRETER_H

#include "ast.h"
#include "environment.h"

typedef struct STInterpreter
{
    STEnvironment global;
    STEnvironment* current;

    STValue result;

    int initialized;
    int error_count;

    char last_error[512];

} STInterpreter;

int st_interpreter_init(STInterpreter* interpreter);

void st_interpreter_shutdown(
    STInterpreter* interpreter
);

int st_interpreter_execute(
    STInterpreter* interpreter,
    STASTNode* root
);

int st_interpreter_evaluate_expression(
    STInterpreter* interpreter,
    STExpression* expression,
    STValue* result
);

int st_interpreter_has_errors(
    const STInterpreter* interpreter
);

const char* st_interpreter_last_error(
    const STInterpreter* interpreter
);

int st_interpreter_error_count(
    const STInterpreter* interpreter
);

STEnvironment* st_interpreter_environment(
    STInterpreter* interpreter
);

STValue* st_interpreter_result(
    STInterpreter* interpreter
);

#endif