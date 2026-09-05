#ifndef ST_RANDOM_H
#define ST_RANDOM_H

/*
    Simple Think Random Library
*/

void st_random_seed(unsigned int seed);

int st_random_int(void);
int st_random_int_range(int minimum, int maximum);

float st_random_float(void);
float st_random_float_range(float minimum, float maximum);

int st_random_bool(void);

#endif