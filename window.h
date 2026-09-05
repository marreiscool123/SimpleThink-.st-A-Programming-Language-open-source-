#ifndef ST_WINDOW_H
#define ST_WINDOW_H

#include <windows.h>

/*
    Simple Think Window Library
*/

typedef struct STWindow
{
    HWND handle;
    HINSTANCE instance;
    int width;
    int height;
    int open;
} STWindow;

int st_window_init(
    STWindow* window,
    HINSTANCE instance,
    const char* title,
    int width,
    int height
);

void st_window_close(STWindow* window);

int st_window_is_open(const STWindow* window);

void st_window_show(STWindow* window);
void st_window_hide(STWindow* window);

void st_window_set_title(STWindow* window, const char* title);

void st_window_set_size(
    STWindow* window,
    int width,
    int height
);

HWND st_window_handle(STWindow* window);

int st_window_process_events(void);

#endif