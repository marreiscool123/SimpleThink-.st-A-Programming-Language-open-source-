#ifndef ST_MATH_H
#define ST_MATH_H

/*
    Simple Think Math Library
*/

int st_math_abs_int(int value);
float st_math_abs_float(float value);

int st_math_min_int(int a, int b);
int st_math_max_int(int a, int b);

float st_math_min_float(float a, float b);
float st_math_max_float(float a, float b);

int st_math_clamp_int(int value, int minimum, int maximum);
float st_math_clamp_float(float value, float minimum, float maximum);

int st_math_pow_int(int base, int exponent);
float st_math_pow_float(float base, int exponent);

int st_math_mod(int a, int b);

int st_math_gcd(int a, int b);
int st_math_lcm(int a, int b);

int st_math_factorial(int value);

int st_math_sign_int(int value);
int st_math_sign_float(float value);

int st_math_is_even(int value);
int st_math_is_odd(int value);

float st_math_lerp(float a, float b, float amount);

float st_math_degrees_to_radians(float degrees);
float st_math_radians_to_degrees(float radians);

#endif