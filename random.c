#include "random.h"
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

static int st_random_seeded = 0;

void st_random_seed(unsigned int seed)
{
    srand(seed);
    st_random_seeded = 1;
}

static void st_random_ensure_seeded(void)
{
    if (!st_random_seeded)
    {
        #ifdef _WIN32
srand((unsigned int)GetTickCount());
#else
srand((unsigned int)time(NULL));
#endif
        st_random_seeded = 1;
    }
}

int st_random_int(void)
{
    st_random_ensure_seeded();

    return rand();
}

int st_random_int_range(int minimum, int maximum)
{
    int range;

    st_random_ensure_seeded();

    if (minimum > maximum)
    {
        int temp = minimum;
        minimum = maximum;
        maximum = temp;
    }

    range = maximum - minimum + 1;

    if (range <= 0)
        return minimum;

    return minimum + (rand() % range);
}

float st_random_float(void)
{
    st_random_ensure_seeded();

    return (float)rand() / (float)RAND_MAX;
}

float st_random_float_range(float minimum, float maximum)
{
    float amount;

    st_random_ensure_seeded();

    if (minimum > maximum)
    {
        float temp = minimum;
        minimum = maximum;
        maximum = temp;
    }

    amount = (float)rand() / (float)RAND_MAX;

    return minimum + (maximum - minimum) * amount;
}

int st_random_bool(void)
{
    st_random_ensure_seeded();

    return rand() % 2;
}