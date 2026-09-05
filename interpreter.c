#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void st_interpreter_error(
    STInterpreter* interpreter,
    const char* message
)
{
    if (interpreter == NULL)
        return;

    interpreter->error_count++;

    if (message == NULL)
        message = "Unknown interpreter error.";

    strncpy(
        interpreter->last_error,
        message,
        sizeof(interpreter->last_error) - 1
    );

    interpreter->last_error[
        sizeof(interpreter->last_error) - 1
    ] = '\0';
}

static int st_interpreter_evaluate(
    STInterpreter* interpreter,
    STExpression* expression,
    STValue* result
)
{
    STValue* variable;
    STValue left;
    STValue right;
    int left_int;
    int right_int;

    if (interpreter == NULL ||
        expression == NULL ||
        result == NULL)
    {
        return 0;
    }

    st_value_init(result);

    switch (expression->type)
    {
        case ST_EXPRESSION_INTEGER:
            *result = st_value_int(
                atoi(expression->value)
            );
            return 1;

        case ST_EXPRESSION_FLOAT:
            *result = st_value_float(
                (float)atof(expression->value)
            );
            return 1;

        case ST_EXPRESSION_STRING:
            *result = st_value_string(
                expression->value
            );
            return 1;

        case ST_EXPRESSION_CHARACTER:
            *result = st_value_char(
                expression->value != NULL
                    ? expression->value[0]
                    : '\0'
            );
            return 1;

        case ST_EXPRESSION_BOOLEAN:
            *result = st_value_bool(
                expression->value != NULL &&
                strcmp(expression->value, "true") == 0
            );
            return 1;

        case ST_EXPRESSION_IDENTIFIER:
            variable =
                st_environment_get(
                    interpreter->current,
                    expression->value
                );

            if (variable == NULL)
            {
                st_interpreter_error(
                    interpreter,
                    "Undefined variable."
                );

                return 0;
            }

            if (variable->type == ST_TYPE_STRING)
            {
                *result =
                    st_value_string(
                        variable->data.string_value
                    );
            }
            else
            {
                *result = *variable;
            }

            return 1;

        case ST_EXPRESSION_GROUP:
            return st_interpreter_evaluate(
                interpreter,
                expression->left,
                result
            );

        case ST_EXPRESSION_UNARY:
            if (!st_interpreter_evaluate(
                    interpreter,
                    expression->left,
                    &left))
            {
                return 0;
            }

            switch (expression->operator)
            {
                case ST_OPERATOR_NOT:
                    *result =
                        st_value_bool(
                            !st_value_is_truthy(&left)
                        );
                    break;

                case ST_OPERATOR_SUBTRACT:
                    if (left.type == ST_TYPE_FLOAT)
                    {
                        *result =
                            st_value_float(
                                -left.data.float_value
                            );
                    }
                    else
                    {
                        *result =
                            st_value_int(
                                -st_value_to_int(&left)
                            );
                    }
                    break;

                case ST_OPERATOR_ADD:
                    *result =
                        st_value_int(
                            st_value_to_int(&left)
                        );
                    break;

                default:
                    st_value_free(&left);

                    st_interpreter_error(
                        interpreter,
                        "Unsupported unary operator."
                    );

                    return 0;
            }

            st_value_free(&left);
            return 1;

        case ST_EXPRESSION_BINARY:
            if (!st_interpreter_evaluate(
                    interpreter,
                    expression->left,
                    &left))
            {
                return 0;
            }

            if (!st_interpreter_evaluate(
                    interpreter,
                    expression->right,
                    &right))
            {
                st_value_free(&left);
                return 0;
            }

            left_int = st_value_to_int(&left);
            right_int = st_value_to_int(&right);

            switch (expression->operator)
            {
                case ST_OPERATOR_ADD:
                    *result =
                        st_value_int(
                            left_int + right_int
                        );
                    break;

                case ST_OPERATOR_SUBTRACT:
                    *result =
                        st_value_int(
                            left_int - right_int
                        );
                    break;

                case ST_OPERATOR_MULTIPLY:
                    *result =
                        st_value_int(
                            left_int * right_int
                        );
                    break;

                case ST_OPERATOR_DIVIDE:
                    if (right_int == 0)
                    {
                        st_value_free(&left);
                        st_value_free(&right);

                        st_interpreter_error(
                            interpreter,
                            "Division by zero."
                        );

                        return 0;
                    }

                    *result =
                        st_value_int(
                            left_int / right_int
                        );
                    break;

                case ST_OPERATOR_MODULO:
                    if (right_int == 0)
                    {
                        st_value_free(&left);
                        st_value_free(&right);

                        st_interpreter_error(
                            interpreter,
                            "Modulo by zero."
                        );

                        return 0;
                    }

                    *result =
                        st_value_int(
                            left_int % right_int
                        );
                    break;

                case ST_OPERATOR_EQUAL:
                    *result =
                        st_value_bool(
                            left_int == right_int
                        );
                    break;

                case ST_OPERATOR_NOT_EQUAL:
                    *result =
                        st_value_bool(
                            left_int != right_int
                        );
                    break;

                case ST_OPERATOR_LESS:
                    *result =
                        st_value_bool(
                            left_int < right_int
                        );
                    break;

                case ST_OPERATOR_LESS_EQUAL:
                    *result =
                        st_value_bool(
                            left_int <= right_int
                        );
                    break;

                case ST_OPERATOR_GREATER:
                    *result =
                        st_value_bool(
                            left_int > right_int
                        );
                    break;

                case ST_OPERATOR_GREATER_EQUAL:
                    *result =
                        st_value_bool(
                            left_int >= right_int
                        );
                    break;

                case ST_OPERATOR_AND:
                    *result =
                        st_value_bool(
                            st_value_is_truthy(&left) &&
                            st_value_is_truthy(&right)
                        );
                    break;

                case ST_OPERATOR_OR:
                    *result =
                        st_value_bool(
                            st_value_is_truthy(&left) ||
                            st_value_is_truthy(&right)
                        );
                    break;

                default:
                    st_value_free(&left);
                    st_value_free(&right);

                    st_interpreter_error(
                        interpreter,
                        "Unsupported binary operator."
                    );

                    return 0;
            }

            st_value_free(&left);
            st_value_free(&right);

            return 1;

        default:
            st_interpreter_error(
                interpreter,
                "Unsupported expression."
            );

            return 0;
    }
}

int st_interpreter_init(
    STInterpreter* interpreter
)
{
    if (interpreter == NULL)
        return 0;

    memset(
        interpreter,
        0,
        sizeof(STInterpreter)
    );

    st_environment_init(
        &interpreter->global,
        NULL
    );

    interpreter->current =
        &interpreter->global;

    st_value_init(
        &interpreter->result
    );

    interpreter->initialized = 1;

    return 1;
}

void st_interpreter_shutdown(
    STInterpreter* interpreter
)
{
    if (interpreter == NULL)
        return;

    st_environment_free(
        &interpreter->global
    );

    st_value_free(
        &interpreter->result
    );

    interpreter->current = NULL;
    interpreter->initialized = 0;
}

int st_interpreter_evaluate_expression(
    STInterpreter* interpreter,
    STExpression* expression,
    STValue* result
)
{
    if (interpreter == NULL ||
        !interpreter->initialized)
    {
        return 0;
    }

    return st_interpreter_evaluate(
        interpreter,
        expression,
        result
    );
}

int st_interpreter_execute(
    STInterpreter* interpreter,
    STASTNode* root
)
{
    STASTNode* node;
    STValue value;

    if (interpreter == NULL ||
        root == NULL ||
        !interpreter->initialized)
    {
        return 0;
    }

    interpreter->error_count = 0;
    interpreter->last_error[0] = '\0';

    node = root;

    while (node != NULL)
    {
        if (node->expression != NULL)
        {
            if (!st_interpreter_evaluate(
                    interpreter,
                    node->expression,
                    &value))
            {
                return 0;
            }

            st_value_free(
                &interpreter->result
            );

            interpreter->result = value;
        }

        node = node->children[0];
    }

    return interpreter->error_count == 0;
}

int st_interpreter_has_errors(
    const STInterpreter* interpreter
)
{
    if (interpreter == NULL)
        return 1;

    return interpreter->error_count > 0;
}

const char* st_interpreter_last_error(
    const STInterpreter* interpreter
)
{
    if (interpreter == NULL)
        return "Invalid interpreter.";

    return interpreter->last_error;
}

int st_interpreter_error_count(
    const STInterpreter* interpreter
)
{
    if (interpreter == NULL)
        return 1;

    return interpreter->error_count;
}

STEnvironment* st_interpreter_environment(
    STInterpreter* interpreter
)
{
    if (interpreter == NULL)
        return NULL;

    return interpreter->current;
}

STValue* st_interpreter_result(
    STInterpreter* interpreter
)
{
    if (interpreter == NULL)
        return NULL;

    return &interpreter->result;
}