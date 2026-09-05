#ifndef ST_GRAPHICS_H
#define ST_GRAPHICS_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/*
    Simple Think Graphics Library

    Software renderer.

    Simple Think performs the actual drawing into its own
    framebuffer. Windows is only used to present that framebuffer
    to the application window.
*/

typedef struct STColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} STColor;

typedef struct STGraphics
{
    HWND window;

    HDC device_context;

    int width;
    int height;
    int pitch;

    unsigned int* pixels;

    int initialized;

    int clip_x;
    int clip_y;
    int clip_width;
    int clip_height;
} STGraphics;


/*
    Color helpers.
*/

STColor st_graphics_color(
    unsigned char r,
    unsigned char g,
    unsigned char b
);

STColor st_graphics_color_alpha(
    unsigned char r,
    unsigned char g,
    unsigned char b,
    unsigned char a
);

unsigned int st_graphics_color_pack(
    STColor color
);

STColor st_graphics_color_unpack(
    unsigned int color
);


/*
    Graphics lifecycle.
*/

int st_graphics_init(
    STGraphics* graphics,
    HWND window
);

void st_graphics_shutdown(
    STGraphics* graphics
);

int st_graphics_resize(
    STGraphics* graphics,
    int width,
    int height
);

void st_graphics_present(
    STGraphics* graphics
);


/*
    Frame control.
*/

void st_graphics_clear(
    STGraphics* graphics
);

void st_graphics_clear_color(
    STGraphics* graphics,
    STColor color
);


/*
    Clipping.
*/

void st_graphics_set_clip(
    STGraphics* graphics,
    int x,
    int y,
    int width,
    int height
);

void st_graphics_reset_clip(
    STGraphics* graphics
);


/*
    Pixel operations.
*/

void st_graphics_pixel(
    STGraphics* graphics,
    int x,
    int y,
    COLORREF color
);

void st_graphics_pixel_color(
    STGraphics* graphics,
    int x,
    int y,
    STColor color
);

STColor st_graphics_get_pixel(
    STGraphics* graphics,
    int x,
    int y
);


/*
    Lines.
*/

void st_graphics_line(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    COLORREF color
);

void st_graphics_line_color(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    STColor color
);


/*
    Rectangles.
*/

void st_graphics_rectangle(
    STGraphics* graphics,
    int x,
    int y,
    int width,
    int height,
    COLORREF color
);

void st_graphics_rectangle_color(
    STGraphics* graphics,
    int x,
    int y,
    int width,
    int height,
    STColor color
);

void st_graphics_fill_rectangle(
    STGraphics* graphics,
    int x,
    int y,
    int width,
    int height,
    COLORREF color
);

void st_graphics_fill_rectangle_color(
    STGraphics* graphics,
    int x,
    int y,
    int width,
    int height,
    STColor color
);


/*
    Circles.
*/

void st_graphics_circle(
    STGraphics* graphics,
    int x,
    int y,
    int radius,
    COLORREF color
);

void st_graphics_circle_color(
    STGraphics* graphics,
    int x,
    int y,
    int radius,
    STColor color
);

void st_graphics_fill_circle(
    STGraphics* graphics,
    int x,
    int y,
    int radius,
    COLORREF color
);

void st_graphics_fill_circle_color(
    STGraphics* graphics,
    int x,
    int y,
    int radius,
    STColor color
);

void st_graphics_text(
    STGraphics* graphics,
    int x,
    int y,
    const char* text,
    STColor color
);

void st_graphics_text_scale(
    STGraphics* graphics,
    int x,
    int y,
    const char* text,
    STColor color,
    int scale
);


/*
    Triangles.
*/

void st_graphics_triangle(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    int x3,
    int y3,
    COLORREF color
);

void st_graphics_triangle_color(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    int x3,
    int y3,
    STColor color
);

void st_graphics_fill_triangle(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    int x3,
    int y3,
    COLORREF color
);

void st_graphics_fill_triangle_color(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    int x3,
    int y3,
    STColor color
);


/*
    Information.
*/

int st_graphics_get_width(
    STGraphics* graphics
);

int st_graphics_get_height(
    STGraphics* graphics
);

unsigned int* st_graphics_get_buffer(
    STGraphics* graphics
);

#endif