#include "collision.h"
#include <stddef.h>

static float st_collision_abs(float value)
{
    return value < 0.0f ? -value : value;
}

static float st_collision_sqrt(float value)
{
    float result;
    int i;

    if (value <= 0.0f)
        return 0.0f;

    result = value;

    for (i = 0; i < 12; i++)
    {
        result = 0.5f * (result + value / result);
    }

    return result;
}

float st_collision_distance(
    float x1,
    float y1,
    float x2,
    float y2
)
{
    float dx;
    float dy;

    dx = x2 - x1;
    dy = y2 - y1;

    return st_collision_sqrt(
        dx * dx + dy * dy
    );
}

int st_collision_point_in_rect(
    float x,
    float y,
    const STRect* rectangle
)
{
    if (rectangle == NULL)
        return 0;

    if (x < rectangle->x)
        return 0;

    if (x > rectangle->x + rectangle->width)
        return 0;

    if (y < rectangle->y)
        return 0;

    if (y > rectangle->y + rectangle->height)
        return 0;

    return 1;
}

int st_collision_point_in_circle(
    float x,
    float y,
    const STCircle* circle
)
{
    float dx;
    float dy;

    if (circle == NULL)
        return 0;

    dx = x - circle->x;
    dy = y - circle->y;

    return (dx * dx + dy * dy) <=
           (circle->radius * circle->radius);
}

int st_collision_rect_rect(
    const STRect* a,
    const STRect* b
)
{
    if (a == NULL || b == NULL)
        return 0;

    if (a->x + a->width < b->x)
        return 0;

    if (b->x + b->width < a->x)
        return 0;

    if (a->y + a->height < b->y)
        return 0;

    if (b->y + b->height < a->y)
        return 0;

    return 1;
}

int st_collision_circle_circle(
    const STCircle* a,
    const STCircle* b
)
{
    float dx;
    float dy;
    float radius;

    if (a == NULL || b == NULL)
        return 0;

    dx = b->x - a->x;
    dy = b->y - a->y;

    radius = a->radius + b->radius;

    return (dx * dx + dy * dy) <=
           (radius * radius);
}

int st_collision_circle_rect(
    const STCircle* circle,
    const STRect* rectangle
)
{
    float closest_x;
    float closest_y;
    float dx;
    float dy;

    if (circle == NULL || rectangle == NULL)
        return 0;

    closest_x = circle->x;

    if (closest_x < rectangle->x)
        closest_x = rectangle->x;

    if (closest_x > rectangle->x + rectangle->width)
        closest_x = rectangle->x + rectangle->width;

    closest_y = circle->y;

    if (closest_y < rectangle->y)
        closest_y = rectangle->y;

    if (closest_y > rectangle->y + rectangle->height)
        closest_y = rectangle->y + rectangle->height;

    dx = circle->x - closest_x;
    dy = circle->y - closest_y;

    return (dx * dx + dy * dy) <=
           (circle->radius * circle->radius);
}

int st_collision_point_line(
    float px,
    float py,
    float x1,
    float y1,
    float x2,
    float y2,
    float tolerance
)
{
    float dx;
    float dy;
    float length_squared;
    float t;
    float closest_x;
    float closest_y;
    float distance_x;
    float distance_y;

    dx = x2 - x1;
    dy = y2 - y1;

    length_squared = dx * dx + dy * dy;

    if (length_squared == 0.0f)
    {
        return st_collision_distance(
            px,
            py,
            x1,
            y1
        ) <= tolerance;
    }

    t = ((px - x1) * dx + (py - y1) * dy)
        / length_squared;

    if (t < 0.0f)
        t = 0.0f;

    if (t > 1.0f)
        t = 1.0f;

    closest_x = x1 + t * dx;
    closest_y = y1 + t * dy;

    distance_x = px - closest_x;
    distance_y = py - closest_y;

    return (distance_x * distance_x +
            distance_y * distance_y) <=
           (tolerance * tolerance);
}