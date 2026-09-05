#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdlib.h>
#include <string.h>

#include "graphics.h"


/*
    Internal helpers.
*/

static int st_graphics_inside(
    STGraphics* graphics,
    int x,
    int y
)
{
    if (graphics == NULL)
        return 0;

    if (!graphics->initialized)
        return 0;

    if (x < graphics->clip_x)
        return 0;

    if (y < graphics->clip_y)
        return 0;

    if (x >= graphics->clip_x + graphics->clip_width)
        return 0;

    if (y >= graphics->clip_y + graphics->clip_height)
        return 0;

    if (x < 0 || y < 0)
        return 0;

    if (x >= graphics->width)
        return 0;

    if (y >= graphics->height)
        return 0;

    return 1;
}


static unsigned int st_graphics_pack_rgba(
    unsigned char r,
    unsigned char g,
    unsigned char b,
    unsigned char a
)
{
    return
        ((unsigned int)a << 24) |
        ((unsigned int)r << 16) |
        ((unsigned int)g << 8) |
        (unsigned int)b;
}


static STColor st_graphics_unpack_rgba(
    unsigned int value
)
{
    STColor color;

    color.a = (unsigned char)((value >> 24) & 0xFF);
    color.r = (unsigned char)((value >> 16) & 0xFF);
    color.g = (unsigned char)((value >> 8) & 0xFF);
    color.b = (unsigned char)(value & 0xFF);

    return color;
}


static unsigned int st_graphics_blend_pixel(
    unsigned int destination,
    STColor source
)
{
    STColor dst;
    STColor result;

    unsigned int alpha;
    unsigned int inverse_alpha;

    if (source.a == 255)
    {
        return st_graphics_pack_rgba(
            source.r,
            source.g,
            source.b,
            255
        );
    }

    if (source.a == 0)
        return destination;

    dst = st_graphics_unpack_rgba(destination);

    alpha = source.a;
    inverse_alpha = 255 - alpha;

    result.r = (unsigned char)
        (((unsigned int)source.r * alpha +
          (unsigned int)dst.r * inverse_alpha) / 255);

    result.g = (unsigned char)
        (((unsigned int)source.g * alpha +
          (unsigned int)dst.g * inverse_alpha) / 255);

    result.b = (unsigned char)
        (((unsigned int)source.b * alpha +
          (unsigned int)dst.b * inverse_alpha) / 255);

    result.a = 255;

    return st_graphics_pack_rgba(
        result.r,
        result.g,
        result.b,
        result.a
    );
}


static void st_graphics_draw_pixel_internal(
    STGraphics* graphics,
    int x,
    int y,
    STColor color
)
{
    unsigned int* pixel;

    if (!st_graphics_inside(graphics, x, y))
        return;

    pixel =
        &graphics->pixels[
            y * graphics->pitch + x
        ];

    *pixel = st_graphics_blend_pixel(
        *pixel,
        color
    );
}


/*
    Color helpers.
*/

STColor st_graphics_color(
    unsigned char r,
    unsigned char g,
    unsigned char b
)
{
    STColor color;

    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;

    return color;
}


STColor st_graphics_color_alpha(
    unsigned char r,
    unsigned char g,
    unsigned char b,
    unsigned char a
)
{
    STColor color;

    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    return color;
}


unsigned int st_graphics_color_pack(
    STColor color
)
{
    return st_graphics_pack_rgba(
        color.r,
        color.g,
        color.b,
        color.a
    );
}


STColor st_graphics_color_unpack(
    unsigned int color
)
{
    return st_graphics_unpack_rgba(color);
}


/*
    Graphics lifecycle.
*/

int st_graphics_init(
    STGraphics* graphics,
    HWND window
)
{
    RECT rectangle;

    if (graphics == NULL)
        return 0;

    memset(
        graphics,
        0,
        sizeof(STGraphics)
    );

    if (window == NULL)
        return 0;

    graphics->window = window;

    graphics->device_context =
        GetDC(window);

    if (graphics->device_context == NULL)
    {
        graphics->window = NULL;
        return 0;
    }

    if (!GetClientRect(
            window,
            &rectangle))
    {
        ReleaseDC(
            window,
            graphics->device_context
        );

        graphics->device_context = NULL;
        graphics->window = NULL;

        return 0;
    }

    graphics->width =
        rectangle.right - rectangle.left;

    graphics->height =
        rectangle.bottom - rectangle.top;

    if (graphics->width < 1)
        graphics->width = 1;

    if (graphics->height < 1)
        graphics->height = 1;

    graphics->pitch =
        graphics->width;

    graphics->pixels =
        (unsigned int*)calloc(
            (size_t)graphics->width *
            (size_t)graphics->height,
            sizeof(unsigned int)
        );

    if (graphics->pixels == NULL)
    {
        ReleaseDC(
            window,
            graphics->device_context
        );

        graphics->device_context = NULL;
        graphics->window = NULL;
        graphics->width = 0;
        graphics->height = 0;
        graphics->pitch = 0;

        return 0;
    }

    graphics->clip_x = 0;
    graphics->clip_y = 0;
    graphics->clip_width =
        graphics->width;
    graphics->clip_height =
        graphics->height;

    graphics->initialized = 1;

    st_graphics_clear(
        graphics
    );

    return 1;
}


void st_graphics_shutdown(
    STGraphics* graphics
)
{
    if (graphics == NULL)
        return;

    if (graphics->pixels != NULL)
    {
        free(graphics->pixels);
        graphics->pixels = NULL;
    }

    if (graphics->device_context != NULL &&
        graphics->window != NULL)
    {
        ReleaseDC(
            graphics->window,
            graphics->device_context
        );
    }

    graphics->device_context = NULL;
    graphics->window = NULL;

    graphics->width = 0;
    graphics->height = 0;
    graphics->pitch = 0;

    graphics->clip_x = 0;
    graphics->clip_y = 0;
    graphics->clip_width = 0;
    graphics->clip_height = 0;

    graphics->initialized = 0;
}


int st_graphics_resize(
    STGraphics* graphics,
    int width,
    int height
)
{
    unsigned int* new_pixels;

    int old_width;
    int old_height;

    int copy_width;
    int copy_height;

    int y;

    if (graphics == NULL)
        return 0;

    if (!graphics->initialized)
        return 0;

    if (width < 1)
        width = 1;

    if (height < 1)
        height = 1;

    if (width == graphics->width &&
        height == graphics->height)
    {
        return 1;
    }

    new_pixels =
        (unsigned int*)calloc(
            (size_t)width *
            (size_t)height,
            sizeof(unsigned int)
        );

    if (new_pixels == NULL)
        return 0;

    old_width = graphics->width;
    old_height = graphics->height;

    copy_width = width < old_width
        ? width
        : old_width;

    copy_height = height < old_height
        ? height
        : old_height;

    for (y = 0; y < copy_height; y++)
    {
        memcpy(
            &new_pixels[
                y * width
            ],
            &graphics->pixels[
                y * graphics->pitch
            ],
            (size_t)copy_width *
            sizeof(unsigned int)
        );
    }

    free(graphics->pixels);

    graphics->pixels = new_pixels;

    graphics->width = width;
    graphics->height = height;
    graphics->pitch = width;

    st_graphics_reset_clip(
        graphics
    );

    return 1;
}


/*
    Present the software framebuffer.

    This is the only place where the renderer talks
    to the Windows drawing system.
*/

void st_graphics_present(
    STGraphics* graphics
)
{
    BITMAPINFO bitmap_info;

    if (graphics == NULL)
        return;

    if (!graphics->initialized)
        return;

    if (graphics->device_context == NULL)
        return;

    if (graphics->pixels == NULL)
        return;

    memset(
        &bitmap_info,
        0,
        sizeof(BITMAPINFO)
    );

    bitmap_info.bmiHeader.biSize =
        sizeof(BITMAPINFOHEADER);

    bitmap_info.bmiHeader.biWidth =
        graphics->width;

    bitmap_info.bmiHeader.biHeight =
        -graphics->height;

    bitmap_info.bmiHeader.biPlanes = 1;

    bitmap_info.bmiHeader.biBitCount = 32;

    bitmap_info.bmiHeader.biCompression =
        BI_RGB;

    StretchDIBits(
        graphics->device_context,
        0,
        0,
        graphics->width,
        graphics->height,
        0,
        0,
        graphics->width,
        graphics->height,
        graphics->pixels,
        &bitmap_info,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}


/*
    Frame control.
*/

void st_graphics_clear(
    STGraphics* graphics
)
{
    STColor color;

    color = st_graphics_color(
        0,
        0,
        0
    );

    st_graphics_clear_color(
        graphics,
        color
    );
}


void st_graphics_clear_color(
    STGraphics* graphics,
    STColor color
)
{
    unsigned int packed;
    size_t count;
    size_t index;

    if (graphics == NULL)
        return;

    if (!graphics->initialized)
        return;

    if (graphics->pixels == NULL)
        return;

    packed =
        st_graphics_color_pack(
            color
        );

    count =
        (size_t)graphics->width *
        (size_t)graphics->height;

    for (index = 0; index < count; index++)
    {
        graphics->pixels[index] = packed;
    }
}


/*
    Clipping.
*/

void st_graphics_set_clip(
    STGraphics* graphics,
    int x,
    int y,
    int width,
    int height
)
{
    if (graphics == NULL)
        return;

    if (!graphics->initialized)
        return;

    if (width < 0)
        width = 0;

    if (height < 0)
        height = 0;

    if (x < 0)
    {
        width += x;
        x = 0;
    }

    if (y < 0)
    {
        height += y;
        y = 0;
    }

    if (x >= graphics->width)
        width = 0;

    if (y >= graphics->height)
        height = 0;

    if (x + width > graphics->width)
        width = graphics->width - x;

    if (y + height > graphics->height)
        height = graphics->height - y;

    if (width < 0)
        width = 0;

    if (height < 0)
        height = 0;

    graphics->clip_x = x;
    graphics->clip_y = y;
    graphics->clip_width = width;
    graphics->clip_height = height;
}


void st_graphics_reset_clip(
    STGraphics* graphics
)
{
    if (graphics == NULL)
        return;

    if (!graphics->initialized)
        return;

    graphics->clip_x = 0;
    graphics->clip_y = 0;

    graphics->clip_width =
        graphics->width;

    graphics->clip_height =
        graphics->height;
}


/*
    Pixel operations.
*/

void st_graphics_pixel(
    STGraphics* graphics,
    int x,
    int y,
    COLORREF color
)
{
    STColor converted;

    converted =
        st_graphics_color(
            GetRValue(color),
            GetGValue(color),
            GetBValue(color)
        );

    st_graphics_pixel_color(
        graphics,
        x,
        y,
        converted
    );
}


void st_graphics_pixel_color(
    STGraphics* graphics,
    int x,
    int y,
    STColor color
)
{
    st_graphics_draw_pixel_internal(
        graphics,
        x,
        y,
        color
    );
}


STColor st_graphics_get_pixel(
    STGraphics* graphics,
    int x,
    int y
)
{
    STColor color;

    color = st_graphics_color(
        0,
        0,
        0
    );

    if (graphics == NULL)
        return color;

    if (!graphics->initialized)
        return color;

    if (graphics->pixels == NULL)
        return color;

    if (x < 0 || y < 0)
        return color;

    if (x >= graphics->width ||
        y >= graphics->height)
        return color;

    return st_graphics_unpack_rgba(
        graphics->pixels[
            y * graphics->pitch + x
        ]
    );
}


/*
    Lines.

    Integer Bresenham line algorithm.
*/

void st_graphics_line(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    COLORREF color
)
{
    STColor converted;

    converted =
        st_graphics_color(
            GetRValue(color),
            GetGValue(color),
            GetBValue(color)
        );

    st_graphics_line_color(
        graphics,
        x1,
        y1,
        x2,
        y2,
        converted
    );
}


void st_graphics_line_color(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    STColor color
)
{
    int dx;
    int dy;

    int sx;
    int sy;

    int error;
    int next_error;

    if (graphics == NULL)
        return;

    if (!graphics->initialized)
        return;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);

    sx = x1 < x2 ? 1 : -1;
    sy = y1 < y2 ? 1 : -1;

    error = dx - dy;

    while (1)
    {
        st_graphics_draw_pixel_internal(
            graphics,
            x1,
            y1,
            color
        );

        if (x1 == x2 &&
            y1 == y2)
        {
            break;
        }

        next_error = error * 2;

        if (next_error > -dy)
        {
            error -= dy;
            x1 += sx;
        }

        if (next_error < dx)
        {
            error += dx;
            y1 += sy;
        }
    }
}


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
)
{
    STColor converted;

    converted =
        st_graphics_color(
            GetRValue(color),
            GetGValue(color),
            GetBValue(color)
        );

    st_graphics_rectangle_color(
        graphics,
        x,
        y,
        width,
        height,
        converted
    );
}


void st_graphics_rectangle_color(
    STGraphics* graphics,
    int x,
    int y,
    int width,
    int height,
    STColor color
)
{
    if (graphics == NULL)
        return;

    if (width <= 0 || height <= 0)
        return;

    st_graphics_line_color(
        graphics,
        x,
        y,
        x + width - 1,
        y,
        color
    );

    st_graphics_line_color(
        graphics,
        x,
        y,
        x,
        y + height - 1,
        color
    );

    st_graphics_line_color(
        graphics,
        x + width - 1,
        y,
        x + width - 1,
        y + height - 1,
        color
    );

    st_graphics_line_color(
        graphics,
        x,
        y + height - 1,
        x + width - 1,
        y + height - 1,
        color
    );
}


void st_graphics_fill_rectangle(
    STGraphics* graphics,
    int x,
    int y,
    int width,
    int height,
    COLORREF color
)
{
    STColor converted;

    converted =
        st_graphics_color(
            GetRValue(color),
            GetGValue(color),
            GetBValue(color)
        );

    st_graphics_fill_rectangle_color(
        graphics,
        x,
        y,
        width,
        height,
        converted
    );
}


void st_graphics_fill_rectangle_color(
    STGraphics* graphics,
    int x,
    int y,
    int width,
    int height,
    STColor color
)
{
    int start_x;
    int start_y;

    int end_x;
    int end_y;

    int current_x;
    int current_y;

    if (graphics == NULL)
        return;

    if (!graphics->initialized)
        return;

    if (width <= 0 || height <= 0)
        return;

    start_x = x;
    start_y = y;

    end_x = x + width;
    end_y = y + height;

    if (start_x < graphics->clip_x)
        start_x = graphics->clip_x;

    if (start_y < graphics->clip_y)
        start_y = graphics->clip_y;

    if (end_x >
        graphics->clip_x +
        graphics->clip_width)
    {
        end_x =
            graphics->clip_x +
            graphics->clip_width;
    }

    if (end_y >
        graphics->clip_y +
        graphics->clip_height)
    {
        end_y =
            graphics->clip_y +
            graphics->clip_height;
    }

    if (start_x < 0)
        start_x = 0;

    if (start_y < 0)
        start_y = 0;

    if (end_x > graphics->width)
        end_x = graphics->width;

    if (end_y > graphics->height)
        end_y = graphics->height;

    for (current_y = start_y;
         current_y < end_y;
         current_y++)
    {
        for (current_x = start_x;
             current_x < end_x;
             current_x++)
        {
            st_graphics_draw_pixel_internal(
                graphics,
                current_x,
                current_y,
                color
            );
        }
    }
}


/*
    Circles.

    Midpoint circle algorithm.
*/

void st_graphics_circle(
    STGraphics* graphics,
    int x,
    int y,
    int radius,
    COLORREF color
)
{
    STColor converted;

    converted =
        st_graphics_color(
            GetRValue(color),
            GetGValue(color),
            GetBValue(color)
        );

    st_graphics_circle_color(
        graphics,
        x,
        y,
        radius,
        converted
    );
}


void st_graphics_circle_color(
    STGraphics* graphics,
    int x,
    int y,
    int radius,
    STColor color
)
{
    int px;
    int py;
    int decision;

    if (graphics == NULL)
        return;

    if (radius <= 0)
        return;

    px = radius;
    py = 0;

    decision = 1 - radius;

    while (px >= py)
    {
        st_graphics_draw_pixel_internal(
            graphics,
            x + px,
            y + py,
            color
        );

        st_graphics_draw_pixel_internal(
            graphics,
            x + py,
            y + px,
            color
        );

        st_graphics_draw_pixel_internal(
            graphics,
            x - py,
            y + px,
            color
        );

        st_graphics_draw_pixel_internal(
            graphics,
            x - px,
            y + py,
            color
        );

        st_graphics_draw_pixel_internal(
            graphics,
            x - px,
            y - py,
            color
        );

        st_graphics_draw_pixel_internal(
            graphics,
            x - py,
            y - px,
            color
        );

        st_graphics_draw_pixel_internal(
            graphics,
            x + py,
            y - px,
            color
        );

        st_graphics_draw_pixel_internal(
            graphics,
            x + px,
            y - py,
            color
        );

        py++;

        if (decision <= 0)
        {
            decision +=
                2 * py + 1;
        }
        else
        {
            px--;

            decision +=
                2 * (py - px) + 1;
        }
    }
}


void st_graphics_fill_circle(
    STGraphics* graphics,
    int x,
    int y,
    int radius,
    COLORREF color
)
{
    STColor converted;

    converted =
        st_graphics_color(
            GetRValue(color),
            GetGValue(color),
            GetBValue(color)
        );

    st_graphics_fill_circle_color(
        graphics,
        x,
        y,
        radius,
        converted
    );
}


void st_graphics_fill_circle_color(
    STGraphics* graphics,
    int x,
    int y,
    int radius,
    STColor color
)
{
    int current_y;
    int current_x;

    long long radius_squared;
    long long distance_squared;

    if (graphics == NULL)
        return;

    if (!graphics->initialized)
        return;

    if (radius <= 0)
        return;

    radius_squared =
        (long long)radius *
        (long long)radius;

    for (current_y = -radius;
         current_y <= radius;
         current_y++)
    {
        for (current_x = -radius;
             current_x <= radius;
             current_x++)
        {
            distance_squared =
                (long long)current_x *
                (long long)current_x +
                (long long)current_y *
                (long long)current_y;

            if (distance_squared <=
                radius_squared)
            {
                st_graphics_draw_pixel_internal(
                    graphics,
                    x + current_x,
                    y + current_y,
                    color
                );
            }
        }
    }
}


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
)
{
    STColor converted;

    converted =
        st_graphics_color(
            GetRValue(color),
            GetGValue(color),
            GetBValue(color)
        );

    st_graphics_triangle_color(
        graphics,
        x1,
        y1,
        x2,
        y2,
        x3,
        y3,
        converted
    );
}


void st_graphics_triangle_color(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    int x3,
    int y3,
    STColor color
)
{
    st_graphics_line_color(
        graphics,
        x1,
        y1,
        x2,
        y2,
        color
    );

    st_graphics_line_color(
        graphics,
        x2,
        y2,
        x3,
        y3,
        color
    );

    st_graphics_line_color(
        graphics,
        x3,
        y3,
        x1,
        y1,
        color
    );
}


void st_graphics_fill_triangle(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    int x3,
    int y3,
    COLORREF color
)
{
    STColor converted;

    converted =
        st_graphics_color(
            GetRValue(color),
            GetGValue(color),
            GetBValue(color)
        );

    st_graphics_fill_triangle_color(
        graphics,
        x1,
        y1,
        x2,
        y2,
        x3,
        y3,
        converted
    );
}


void st_graphics_fill_triangle_color(
    STGraphics* graphics,
    int x1,
    int y1,
    int x2,
    int y2,
    int x3,
    int y3,
    STColor color
)
{
    int min_x;
    int max_x;
    int min_y;
    int max_y;

    int x;
    int y;

    long long edge1;
    long long edge2;
    long long edge3;

    int area;

    if (graphics == NULL)
        return;

    if (!graphics->initialized)
        return;

    min_x = x1;

    if (x2 < min_x)
        min_x = x2;

    if (x3 < min_x)
        min_x = x3;

    max_x = x1;

    if (x2 > max_x)
        max_x = x2;

    if (x3 > max_x)
        max_x = x3;

    min_y = y1;

    if (y2 < min_y)
        min_y = y2;

    if (y3 < min_y)
        min_y = y3;

    max_y = y1;

    if (y2 > max_y)
        max_y = y2;

    if (y3 > max_y)
        max_y = y3;

    if (min_x < graphics->clip_x)
        min_x = graphics->clip_x;

    if (min_y < graphics->clip_y)
        min_y = graphics->clip_y;

    if (max_x >=
        graphics->clip_x +
        graphics->clip_width)
    {
        max_x =
            graphics->clip_x +
            graphics->clip_width - 1;
    }

    if (max_y >=
        graphics->clip_y +
        graphics->clip_height)
    {
        max_y =
            graphics->clip_y +
            graphics->clip_height - 1;
    }

    if (min_x < 0)
        min_x = 0;

    if (min_y < 0)
        min_y = 0;

    if (max_x >= graphics->width)
        max_x = graphics->width - 1;

    if (max_y >= graphics->height)
        max_y = graphics->height - 1;

    area =
        (x2 - x1) * (y3 - y1) -
        (y2 - y1) * (x3 - x1);

    if (area == 0)
        return;

    for (y = min_y; y <= max_y; y++)
    {
        for (x = min_x; x <= max_x; x++)
        {
            edge1 =
                (long long)(x2 - x1) *
                (long long)(y - y1) -
                (long long)(y2 - y1) *
                (long long)(x - x1);

            edge2 =
                (long long)(x3 - x2) *
                (long long)(y - y2) -
                (long long)(y3 - y2) *
                (long long)(x - x2);

            edge3 =
                (long long)(x1 - x3) *
                (long long)(y - y3) -
                (long long)(y1 - y3) *
                (long long)(x - x3);

            if ((edge1 >= 0 &&
                 edge2 >= 0 &&
                 edge3 >= 0) ||
                (edge1 <= 0 &&
                 edge2 <= 0 &&
                 edge3 <= 0))
            {
                st_graphics_draw_pixel_internal(
                    graphics,
                    x,
                    y,
                    color
                );
            }
        }
    }
}


/*
    Information.
*/

int st_graphics_get_width(
    STGraphics* graphics
)
{
    if (graphics == NULL)
        return 0;

    return graphics->width;
}


int st_graphics_get_height(
    STGraphics* graphics
)
{
    if (graphics == NULL)
        return 0;

    return graphics->height;
}


unsigned int* st_graphics_get_buffer(
    STGraphics* graphics
)
{
    if (graphics == NULL)
        return NULL;

    return graphics->pixels;
}