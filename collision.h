#ifndef ST_COLLISION_H
#define ST_COLLISION_H

/*
    Simple Think Collision Library
*/

typedef struct STPoint
{
    float x;
    float y;
} STPoint;

typedef struct STRect
{
    float x;
    float y;
    float width;
    float height;
} STRect;

typedef struct STCircle
{
    float x;
    float y;
    float radius;
} STCircle;

float st_collision_distance(
    float x1,
    float y1,
    float x2,
    float y2
);

int st_collision_point_in_rect(
    float x,
    float y,
    const STRect* rectangle
);

int st_collision_point_in_circle(
    float x,
    float y,
    const STCircle* circle
);

int st_collision_rect_rect(
    const STRect* a,
    const STRect* b
);

int st_collision_circle_circle(
    const STCircle* a,
    const STCircle* b
);

int st_collision_circle_rect(
    const STCircle* circle,
    const STRect* rectangle
);

int st_collision_point_line(
    float px,
    float py,
    float x1,
    float y1,
    float x2,
    float y2,
    float tolerance
);

#endif