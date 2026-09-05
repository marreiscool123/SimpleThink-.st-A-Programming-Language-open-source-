#include "window.h"

#include <windows.h>
#include <string.h>

static const char* ST_WINDOW_CLASS_NAME = "SimpleThinkWindow";

static LRESULT CALLBACK st_window_proc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (message)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcA(
        hwnd,
        message,
        wParam,
        lParam
    );
}

static int st_window_register_class(HINSTANCE instance)
{
    WNDCLASSA window_class;

    memset(&window_class, 0, sizeof(WNDCLASSA));

    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = st_window_proc;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    window_class.lpszClassName = ST_WINDOW_CLASS_NAME;

    if (RegisterClassA(&window_class) == 0)
    {
        if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
            return 0;
    }

    return 1;
}

int st_window_init(
    STWindow* window,
    HINSTANCE instance,
    const char* title,
    int width,
    int height
)
{
    HWND handle;

    if (window == NULL)
        return 0;

    if (title == NULL)
        title = "Simple Think";

    if (width <= 0)
        width = 800;

    if (height <= 0)
        height = 600;

    if (!st_window_register_class(instance))
        return 0;

    handle = CreateWindowExA(
        0,
        ST_WINDOW_CLASS_NAME,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        instance,
        NULL
    );

    if (handle == NULL)
        return 0;

    window->handle = handle;
    window->instance = instance;
    window->width = width;
    window->height = height;
    window->open = 1;

    ShowWindow(handle, SW_SHOW);
    UpdateWindow(handle);

    return 1;
}

void st_window_close(STWindow* window)
{
    if (window == NULL)
        return;

    if (window->handle != NULL)
    {
        DestroyWindow(window->handle);
        window->handle = NULL;
    }

    window->open = 0;
}

int st_window_is_open(const STWindow* window)
{
    if (window == NULL)
        return 0;

    return window->open;
}

void st_window_show(STWindow* window)
{
    if (window == NULL || window->handle == NULL)
        return;

    ShowWindow(window->handle, SW_SHOW);
    UpdateWindow(window->handle);
    window->open = 1;
}

void st_window_hide(STWindow* window)
{
    if (window == NULL || window->handle == NULL)
        return;

    ShowWindow(window->handle, SW_HIDE);
}

void st_window_set_title(STWindow* window, const char* title)
{
    if (window == NULL || window->handle == NULL)
        return;

    if (title == NULL)
        title = "Simple Think";

    SetWindowTextA(window->handle, title);
}

void st_window_set_size(
    STWindow* window,
    int width,
    int height
)
{
    if (window == NULL || window->handle == NULL)
        return;

    if (width <= 0 || height <= 0)
        return;

    SetWindowPos(
        window->handle,
        NULL,
        0,
        0,
        width,
        height,
        SWP_NOMOVE | SWP_NOZORDER
    );

    window->width = width;
    window->height = height;
}

HWND st_window_handle(STWindow* window)
{
    if (window == NULL)
        return NULL;

    return window->handle;
}

int st_window_process_events(void)
{
    MSG message;

    while (PeekMessageA(
        &message,
        NULL,
        0,
        0,
        PM_REMOVE
    ))
    {
        if (message.message == WM_QUIT)
            return 0;

        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return 1;
}