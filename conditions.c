#include "conditions.h"

#include <stdlib.h>
#include <string.h>


static char* st_condition_strdup(
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


static void st_condition_case_free(
    STConditionCase* condition_case
)
{
    if (condition_case == NULL)
        return;

    free(condition_case->value);
    free(condition_case->body);

    condition_case->value = NULL;
    condition_case->body = NULL;
}


STCondition* st_condition_create(
    STConditionType type,
    int line,
    int column
)
{
    STCondition* condition;

    condition = malloc(sizeof(STCondition));

    if (condition == NULL)
        return NULL;

    memset(
        condition,
        0,
        sizeof(STCondition)
    );

    condition->type = type;
    condition->line = line;
    condition->column = column;

    return condition;
}


STCondition* st_condition_create_switch(
    int line,
    int column
)
{
    return st_condition_create(
        ST_CONDITION_SWITCH,
        line,
        column
    );
}


int st_condition_set_expression(
    STCondition* condition,
    const char* expression
)
{
    char* new_expression;

    if (condition == NULL)
        return 0;

    new_expression =
        st_condition_strdup(expression);

    if (
        expression != NULL &&
        new_expression == NULL
    )
    {
        return 0;
    }

    free(condition->condition);

    condition->condition = new_expression;

    return 1;
}


int st_condition_set_body(
    STCondition* condition,
    const char* body
)
{
    char* new_body;

    if (condition == NULL)
        return 0;

    new_body =
        st_condition_strdup(body);

    if (
        body != NULL &&
        new_body == NULL
    )
    {
        return 0;
    }

    free(condition->body);

    condition->body = new_body;

    return 1;
}


int st_condition_add_case(
    STCondition* condition,
    const char* value,
    const char* body,
    int line,
    int column
)
{
    STConditionCase* condition_case;

    if (condition == NULL)
        return 0;

    if (
        condition->type != ST_CONDITION_SWITCH
    )
    {
        return 0;
    }

    if (
        condition->case_count
        >= ST_CONDITION_MAX_CASES
    )
    {
        return 0;
    }

    condition_case =
        &condition->cases[
            condition->case_count
        ];

    memset(
        condition_case,
        0,
        sizeof(STConditionCase)
    );

    condition_case->value =
        st_condition_strdup(value);

    if (
        value != NULL &&
        condition_case->value == NULL
    )
    {
        return 0;
    }

    condition_case->body =
        st_condition_strdup(body);

    if (
        body != NULL &&
        condition_case->body == NULL
    )
    {
        free(condition_case->value);
        condition_case->value = NULL;

        return 0;
    }

    condition_case->line = line;
    condition_case->column = column;
    condition_case->is_default = 0;

    condition->case_count++;

    return 1;
}


int st_condition_has_default(
    const STCondition* condition
)
{
    int i;

    if (condition == NULL)
        return 0;

    for (i = 0; i < condition->case_count; i++)
    {
        if (condition->cases[i].is_default)
            return 1;
    }

    return 0;
}


int st_condition_add_default(
    STCondition* condition,
    const char* body,
    int line,
    int column
)
{
    STConditionCase* condition_case;

    if (condition == NULL)
        return 0;

    if (
        condition->type != ST_CONDITION_SWITCH
    )
    {
        return 0;
    }

    if (st_condition_has_default(condition))
        return 0;

    if (
        condition->case_count
        >= ST_CONDITION_MAX_CASES
    )
    {
        return 0;
    }

    condition_case =
        &condition->cases[
            condition->case_count
        ];

    memset(
        condition_case,
        0,
        sizeof(STConditionCase)
    );

    condition_case->body =
        st_condition_strdup(body);

    if (
        body != NULL &&
        condition_case->body == NULL
    )
    {
        return 0;
    }

    condition_case->line = line;
    condition_case->column = column;
    condition_case->is_default = 1;

    condition->case_count++;

    return 1;
}


int st_condition_is_switch(
    const STCondition* condition
)
{
    if (condition == NULL)
        return 0;

    return condition->type == ST_CONDITION_SWITCH;
}


const char* st_condition_type_name(
    STConditionType type
)
{
    switch (type)
    {
        case ST_CONDITION_IF:
            return "if";

        case ST_CONDITION_ELSE_IF:
            return "else if";

        case ST_CONDITION_ELSE:
            return "else";

        case ST_CONDITION_SWITCH:
            return "switch";

        default:
            return "unknown";
    }
}


void st_condition_free(
    STCondition* condition
)
{
    int i;

    if (condition == NULL)
        return;

    free(condition->condition);
    free(condition->body);

    condition->condition = NULL;
    condition->body = NULL;

    for (i = 0; i < condition->case_count; i++)
    {
        st_condition_case_free(
            &condition->cases[i]
        );
    }

    free(condition);
}