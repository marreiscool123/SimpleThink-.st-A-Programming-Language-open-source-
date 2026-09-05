#ifndef ST_LOOPS_H
#define ST_LOOPS_H

/*
 * Simple Think loop system.
 */

typedef enum STLoopType
{
    ST_LOOP_WHILE,
    ST_LOOP_FOR,
    ST_LOOP_DO_WHILE
} STLoopType;


typedef struct STLoop
{
    STLoopType type;

    int line;
    int column;

    char* condition;
    char* initialization;
    char* increment;

    char* body;

} STLoop;


typedef struct STLoopStack
{
    STLoop* loops;

    int count;
    int capacity;

} STLoopStack;


/*
 * Initialize the loop system.
 */
void st_loops_init(
    STLoopStack* stack
);


/*
 * Free the loop system.
 */
void st_loops_free(
    STLoopStack* stack
);


/*
 * Create a loop.
 */
STLoop* st_loop_create(
    STLoopStack* stack,
    STLoopType type,
    int line,
    int column
);


/*
 * Set the loop condition.
 */
int st_loop_set_condition(
    STLoop* loop,
    const char* condition
);


/*
 * Set the initialization expression.
 *
 * Used by for loops.
 */
int st_loop_set_initialization(
    STLoop* loop,
    const char* initialization
);


/*
 * Set the increment expression.
 *
 * Used by for loops.
 */
int st_loop_set_increment(
    STLoop* loop,
    const char* increment
);


/*
 * Set the loop body.
 */
int st_loop_set_body(
    STLoop* loop,
    const char* body
);


/*
 * Remove the most recent loop.
 */
int st_loop_pop(
    STLoopStack* stack
);


/*
 * Get the current loop.
 */
STLoop* st_loop_current(
    STLoopStack* stack
);


/*
 * Check whether we are currently inside
 * a loop.
 */
int st_loop_inside(
    const STLoopStack* stack
);


/*
 * Get the loop type name.
 */
const char* st_loop_type_name(
    STLoopType type
);


/*
 * Mark a break operation.
 *
 * Returns 1 if break is valid.
 */
int st_loop_break(
    STLoopStack* stack
);


/*
 * Mark a continue operation.
 *
 * Returns 1 if continue is valid.
 */
int st_loop_continue(
    STLoopStack* stack
);

#endif /* ST_LOOPS_H */