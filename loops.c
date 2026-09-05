#include "loops.h"

#include <stdlib.h>
#include <string.h>


#define ST_LOOP_INITIAL_CAPACITY 16


static char* st_loop_strdup(
    const char* text
)
{
    char* result;
    size_t length;

    if (text == NULL)
        return NULL;

    length = strlen(text);

    result = malloc(length + 1);

    if (result == NULL)
        return NULL;

    memcpy(result, text, length + 1);

    return result;
}


static int st_loops_grow(
    STLoopStack* stack
)
{
    STLoop* new_loops;
    int new_capacity;

    if (stack == NULL)
        return 0;

    if (stack->capacity == 0)
        new_capacity = ST_LOOP_INITIAL_CAPACITY;
    else
        new_capacity = stack->capacity * 2;

    new_loops = realloc(
        stack->loops,
        sizeof(STLoop) * new_capacity
    );

    if (new_loops == NULL)
        return 0;

    stack->loops = new_loops;
    stack->capacity = new_capacity;

    return 1;
}


static void st_loop_free(
    STLoop* loop
)
{
    if (loop == NULL)
        return;

    free(loop->condition);
    free(loop->initialization);
    free(loop->increment);
    free(loop->body);

    loop->condition = NULL;
    loop->initialization = NULL;
    loop->increment = NULL;
    loop->body = NULL;
}


void st_loops_init(
    STLoopStack* stack
)
{
    if (stack == NULL)
        return;

    stack->loops = NULL;
    stack->count = 0;
    stack->capacity = 0;
}


void st_loops_free(
    STLoopStack* stack
)
{
    int i;

    if (stack == NULL)
        return;

    for (i = 0; i < stack->count; i++)
    {
        st_loop_free(
            &stack->loops[i]
        );
    }

    free(stack->loops);

    stack->loops = NULL;
    stack->count = 0;
    stack->capacity = 0;
}


STLoop* st_loop_create(
    STLoopStack* stack,
    STLoopType type,
    int line,
    int column
)
{
    STLoop* loop;

    if (stack == NULL)
        return NULL;

    if (stack->count >= stack->capacity)
    {
        if (!st_loops_grow(stack))
            return NULL;
    }

    loop = &stack->loops[stack->count];

    memset(
        loop,
        0,
        sizeof(STLoop)
    );

    loop->type = type;
    loop->line = line;
    loop->column = column;

    stack->count++;

    return loop;
}


int st_loop_set_condition(
    STLoop* loop,
    const char* condition
)
{
    char* new_condition;

    if (loop == NULL)
        return 0;

    new_condition = st_loop_strdup(condition);

    if (condition != NULL &&
        new_condition == NULL)
    {
        return 0;
    }

    free(loop->condition);

    loop->condition = new_condition;

    return 1;
}


int st_loop_set_initialization(
    STLoop* loop,
    const char* initialization
)
{
    char* new_initialization;

    if (loop == NULL)
        return 0;

    new_initialization =
        st_loop_strdup(initialization);

    if (initialization != NULL &&
        new_initialization == NULL)
    {
        return 0;
    }

    free(loop->initialization);

    loop->initialization =
        new_initialization;

    return 1;
}


int st_loop_set_increment(
    STLoop* loop,
    const char* increment
)
{
    char* new_increment;

    if (loop == NULL)
        return 0;

    new_increment =
        st_loop_strdup(increment);

    if (increment != NULL &&
        new_increment == NULL)
    {
        return 0;
    }

    free(loop->increment);

    loop->increment = new_increment;

    return 1;
}


int st_loop_set_body(
    STLoop* loop,
    const char* body
)
{
    char* new_body;

    if (loop == NULL)
        return 0;

    new_body = st_loop_strdup(body);

    if (body != NULL &&
        new_body == NULL)
    {
        return 0;
    }

    free(loop->body);

    loop->body = new_body;

    return 1;
}


int st_loop_pop(
    STLoopStack* stack
)
{
    if (stack == NULL)
        return 0;

    if (stack->count <= 0)
        return 0;

    st_loop_free(
        &stack->loops[stack->count - 1]
    );

    stack->count--;

    return 1;
}


STLoop* st_loop_current(
    STLoopStack* stack
)
{
    if (stack == NULL)
        return NULL;

    if (stack->count <= 0)
        return NULL;

    return &stack->loops[
        stack->count - 1
    ];
}


int st_loop_inside(
    const STLoopStack* stack
)
{
    if (stack == NULL)
        return 0;

    return stack->count > 0;
}


const char* st_loop_type_name(
    STLoopType type
)
{
    switch (type)
    {
        case ST_LOOP_WHILE:
            return "while";

        case ST_LOOP_FOR:
            return "for";

        case ST_LOOP_DO_WHILE:
            return "do while";

        default:
            return "unknown";
    }
}


int st_loop_break(
    STLoopStack* stack
)
{
    /*
     * A break statement is only valid
     * when inside a loop.
     */
    if (!st_loop_inside(stack))
        return 0;

    return 1;
}


int st_loop_continue(
    STLoopStack* stack
)
{
    /*
     * A continue statement is only valid
     * when inside a loop.
     */
    if (!st_loop_inside(stack))
        return 0;

    return 1;
}