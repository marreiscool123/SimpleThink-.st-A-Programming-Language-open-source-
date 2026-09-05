#ifndef ST_RUNTIME_H
#define ST_RUNTIME_H

#include "types.h"
#include "variables.h"
#include "functions.h"
#include "arrays.h"
#include "strings.h"
#include "structs.h"

typedef struct STRuntime
{
    STVariableTable variables;
    STFunctionTable functions;

    int initialized;
    int exit_code;

} STRuntime;


/*
 * Initialize the Simple Think runtime.
 */
int st_runtime_init(
    STRuntime* runtime
);


/*
 * Shut down the runtime.
 */
void st_runtime_shutdown(
    STRuntime* runtime
);


/*
 * Reset runtime state.
 */
void st_runtime_reset(
    STRuntime* runtime
);


/*
 * Set program exit code.
 */
void st_runtime_set_exit_code(
    STRuntime* runtime,
    int exit_code
);


/*
 * Get program exit code.
 */
int st_runtime_get_exit_code(
    const STRuntime* runtime
);


/*
 * Get runtime variable table.
 */
STVariableTable* st_runtime_variables(
    STRuntime* runtime
);


/*
 * Get runtime function table.
 */
STFunctionTable* st_runtime_functions(
    STRuntime* runtime
);


/*
 * Check whether runtime is initialized.
 */
int st_runtime_is_initialized(
    const STRuntime* runtime
);

#endif /* ST_RUNTIME_H */