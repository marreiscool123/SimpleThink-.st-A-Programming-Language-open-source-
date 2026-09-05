#include "input.h"

static unsigned char st_previous_keys[256];
static unsigned char st_current_keys[256];

static int st_previous_mouse[3];
static int st_current_mouse[3];

static int st_mouse_x = 0;
static int st_mouse_y = 0;

static int st_input_initialized = 0;

static void st_input_initialize(void)
{
    int i;

    if (st_input_initialized)
        return;

    for (i = 0; i < 256; i++)
    {
        st_previous_keys[i] = 0;
        st_current_keys[i] = 0;
    }

    for (i = 0; i < 3; i++)
    {
        st_previous_mouse[i] = 0;
        st_current_mouse[i] = 0;
    }

    st_input_initialized = 1;
}

int st_input_key_down(int key)
{
    st_input_initialize();

    if (key < 0 || key > 255)
        return 0;

    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

int st_input_key_pressed(int key)
{
    st_input_initialize();

    if (key < 0 || key > 255)
        return 0;

    return st_current_keys[key] && !st_previous_keys[key];
}

int st_input_key_released(int key)
{
    st_input_initialize();

    if (key < 0 || key > 255)
        return 0;

    return !st_current_keys[key] && st_previous_keys[key];
}

int st_input_mouse_x(HWND window)
{
    POINT point;

    if (window == NULL)
        return 0;

    if (!GetCursorPos(&point))
        return 0;

    if (!ScreenToClient(window, &point))
        return 0;

    st_mouse_x = point.x;
    st_mouse_y = point.y;

    return st_mouse_x;
}

int st_input_mouse_y(HWND window)
{
    POINT point;

    if (window == NULL)
        return 0;

    if (!GetCursorPos(&point))
        return 0;

    if (!ScreenToClient(window, &point))
        return 0;

    st_mouse_x = point.x;
    st_mouse_y = point.y;

    return st_mouse_y;
}

int st_input_mouse_button_down(int button)
{
    int key;

    st_input_initialize();

    if (button == 0)
        key = VK_LBUTTON;
    else if (button == 1)
        key = VK_RBUTTON;
    else if (button == 2)
        key = VK_MBUTTON;
    else
        return 0;

    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

int st_input_mouse_button_pressed(int button)
{
    st_input_initialize();

    if (button < 0 || button >= 3)
        return 0;

    return st_current_mouse[button] &&
           !st_previous_mouse[button];
}

int st_input_mouse_button_released(int button)
{
    st_input_initialize();

    if (button < 0 || button >= 3)
        return 0;

    return !st_current_mouse[button] &&
           st_previous_mouse[button];
}

void st_input_update(void)
{
    int i;

    st_input_initialize();

    for (i = 0; i < 256; i++)
    {
        st_previous_keys[i] = st_current_keys[i];

        st_current_keys[i] =
            (unsigned char)(
                (GetAsyncKeyState(i) & 0x8000) != 0
            );
    }

    st_previous_mouse[0] = st_current_mouse[0];
    st_previous_mouse[1] = st_current_mouse[1];
    st_previous_mouse[2] = st_current_mouse[2];

    st_current_mouse[0] =
        (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

    st_current_mouse[1] =
        (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;

    st_current_mouse[2] =
        (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
}