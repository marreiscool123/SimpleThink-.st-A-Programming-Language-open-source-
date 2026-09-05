#include "runtime.h"

#include <string.h>


int st_runtime_init(
    STRuntime* runtime
)
{
    if (runtime == NULL)
        return 0;


    memset(
        runtime,
        0,
        sizeof(STRuntime)
    );


    st_variables_init(
        &runtime->variables
    );

    st_functions_init(
        &runtime->functions
    );


    runtime->exit_code = 0;
    runtime->initialized = 1;


    return 1;
}


void st_runtime_shutdown(
    STRuntime* runtime
)
{
    if (runtime == NULL)
        return;


    st_variables_free(
        &runtime->variables
    );

    st_functions_free(
        &runtime->functions
    );


    runtime->initialized = 0;
    runtime->exit_code = 0;
}


void st_runtime_reset(
    STRuntime* runtime
)
{
    if (runtime == NULL)
        return;

    if (!runtime->initialized)
        return;


    st_variables_free(
        &runtime->variables
    );

    st_functions_free(
        &runtime->functions
    );


    st_variables_init(
        &runtime->variables
    );

    st_functions_init(
        &runtime->functions
    );


    runtime->exit_code = 0;
}


void st_runtime_set_exit_code(
    STRuntime* runtime,
    int exit_code
)
{
    if (runtime == NULL)
        return;

    runtime->exit_code = exit_code;
}


int st_runtime_get_exit_code(
    const STRuntime* runtime
)
{
    if (runtime == NULL)
        return -1;

    return runtime->exit_code;
}


STVariableTable* st_runtime_variables(
    STRuntime* runtime
)
{
    if (runtime == NULL)
        return NULL;

    return &runtime->variables;
}


STFunctionTable* st_runtime_functions(
    STRuntime* runtime
)
{
    if (runtime == NULL)
        return NULL;

    return &runtime->functions;
}


int st_runtime_is_initialized(
    const STRuntime* runtime
)
{
    if (runtime == NULL)
        return 0;

    return runtime->initialized;
}