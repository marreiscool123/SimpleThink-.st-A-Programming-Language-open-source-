#include "math.h"

#define ST_MATH_PI 3.14159265358979323846f

int st_math_abs_int(int value)
{
    return value < 0 ? -value : value;
}

float st_math_abs_float(float value)
{
    return value < 0.0f ? -value : value;
}

int st_math_min_int(int a, int b)
{
    return a < b ? a : b;
}

int st_math_max_int(int a, int b)
{
    return a > b ? a : b;
}

float st_math_min_float(float a, float b)
{
    return a < b ? a : b;
}

float st_math_max_float(float a, float b)
{
    return a > b ? a : b;
}

int st_math_clamp_int(int value, int minimum, int maximum)
{
    if (value < minimum)
        return minimum;

    if (value > maximum)
        return maximum;

    return value;
}

float st_math_clamp_float(float value, float minimum, float maximum)
{
    if (value < minimum)
        return minimum;

    if (value > maximum)
        return maximum;

    return value;
}

int st_math_pow_int(int base, int exponent)
{
    int result = 1;
    int i;

    if (exponent < 0)
        return 0;

    for (i = 0; i < exponent; i++)
        result *= base;

    return result;
}

float st_math_pow_float(float base, int exponent)
{
    float result = 1.0f;
    int i;
    int negative = 0;

    if (exponent < 0)
    {
        negative = 1;
        exponent = -exponent;
    }

    for (i = 0; i < exponent; i++)
        result *= base;

    if (negative)
    {
        if (result == 0.0f)
            return 0.0f;

        return 1.0f / result;
    }

    return result;
}

int st_math_mod(int a, int b)
{
    if (b == 0)
        return 0;

    return a % b;
}

int st_math_gcd(int a, int b)
{
    a = st_math_abs_int(a);
    b = st_math_abs_int(b);

    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

int st_math_lcm(int a, int b)
{
    int gcd;

    if (a == 0 || b == 0)
        return 0;

    gcd = st_math_gcd(a, b);

    return st_math_abs_int((a / gcd) * b);
}

int st_math_factorial(int value)
{
    int result = 1;
    int i;

    if (value < 0)
        return 0;

    for (i = 2; i <= value; i++)
        result *= i;

    return result;
}

int st_math_sign_int(int value)
{
    if (value < 0)
        return -1;

    if (value > 0)
        return 1;

    return 0;
}

int st_math_sign_float(float value)
{
    if (value < 0.0f)
        return -1;

    if (value > 0.0f)
        return 1;

    return 0;
}

int st_math_is_even(int value)
{
    return value % 2 == 0;
}

int st_math_is_odd(int value)
{
    return value % 2 != 0;
}

float st_math_lerp(float a, float b, float amount)
{
    return a + (b - a) * amount;
}

float st_math_degrees_to_radians(float degrees)
{
    return degrees * (ST_MATH_PI / 180.0f);
}

float st_math_radians_to_degrees(float radians)
{
    return radians * (180.0f / ST_MATH_PI);
}