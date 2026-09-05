#include "gui.h"
#include <windowsx.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static const char* GUI_CLASS_NAME =
    "SimpleThinkGUI";


static GUI* gui_active =
    NULL;


static GUIColor gui_color(
    unsigned char r,
    unsigned char g,
    unsigned char b,
    unsigned char a
)
{
    GUIColor color;

    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    return color;
}


static void gui_default_theme(
    GUITheme* theme
)
{
    if (theme == NULL)
        return;

    theme->background =
        gui_color(24, 24, 28, 255);

    theme->panel =
        gui_color(34, 34, 40, 255);

    theme->button =
        gui_color(55, 55, 65, 255);

    theme->button_hover =
        gui_color(70, 70, 85, 255);

    theme->button_pressed =
        gui_color(45, 45, 55, 255);

    theme->text =
        gui_color(235, 235, 240, 255);

    theme->border =
        gui_color(90, 90, 105, 255);
}


static int gui_point_inside(
    GUIWidget* widget,
    int x,
    int y
)
{
    if (widget == NULL)
        return 0;

    if (!widget->visible)
        return 0;

    if (x < widget->x)
        return 0;

    if (y < widget->y)
        return 0;

    if (x >= widget->x + widget->width)
        return 0;

    if (y >= widget->y + widget->height)
        return 0;

    return 1;
}


static void gui_update_widgets(
    GUI* gui
)
{
    int i;

    if (gui == NULL)
        return;

    for (i = 0; i < gui->widget_count; i++)
    {
        GUIWidget* widget =
            &gui->widgets[i];

        if (!widget->visible)
        {
            widget->hovered = 0;
            widget->pressed = 0;
            continue;
        }

        widget->hovered =
            gui_point_inside(
                widget,
                gui->mouse_x,
                gui->mouse_y
            );

        if (!widget->enabled)
        {
            widget->pressed = 0;
            continue;
        }

        if (widget->type == GUI_WIDGET_BUTTON)
        {
            if (widget->hovered &&
                gui->mouse_left)
            {
                widget->pressed = 1;
            }
            else
            {
                widget->pressed = 0;
            }

            if (widget->hovered &&
                gui->mouse_left_released)
            {
                if (widget->on_click != NULL)
                {
                    widget->on_click(widget);
                }
            }
        }
    }
}


static void gui_render(
    GUI* gui
)
{
    PAINTSTRUCT paint;
    HDC hdc;

    int i;

    if (gui == NULL ||
        gui->window == NULL)
        return;

    hdc = BeginPaint(
        gui->window,
        &paint
    );

    if (hdc == NULL)
        return;

    /*
     * Temporary renderer.
     *
     * The actual Simple Think graphics
     * renderer will replace this section.
     */

    {
        RECT area;

        HBRUSH background;

        area.left = 0;
        area.top = 0;
        area.right = gui->width;
        area.bottom = gui->height;

        background = CreateSolidBrush(
            RGB(
                gui->theme.background.r,
                gui->theme.background.g,
                gui->theme.background.b
            )
        );

        if (background != NULL)
        {
            FillRect(
                hdc,
                &area,
                background
            );

            DeleteObject(background);
        }
    }

    for (i = 0; i < gui->widget_count; i++)
    {
        GUIWidget* widget =
            &gui->widgets[i];

        RECT rectangle;

        HBRUSH brush;

        COLORREF background_color;

        if (!widget->visible)
            continue;

        if (widget->type == GUI_WIDGET_LABEL)
        {
            SetTextColor(
                hdc,
                RGB(
                    widget->foreground.r,
                    widget->foreground.g,
                    widget->foreground.b
                )
            );

            SetBkMode(
                hdc,
                TRANSPARENT
            );

            TextOutA(
                hdc,
                widget->x,
                widget->y,
                widget->text,
                (int)strlen(widget->text)
            );

            continue;
        }

        rectangle.left =
            widget->x;

        rectangle.top =
            widget->y;

        rectangle.right =
            widget->x + widget->width;

        rectangle.bottom =
            widget->y + widget->height;

        if (widget->type == GUI_WIDGET_BUTTON)
        {
            if (widget->pressed)
            {
                background_color =
                    RGB(
                        widget->background.r,
                        widget->background.g,
                        widget->background.b
                    );
            }
            else if (widget->hovered)
            {
                background_color =
                    RGB(
                        gui->theme.button_hover.r,
                        gui->theme.button_hover.g,
                        gui->theme.button_hover.b
                    );
            }
            else
            {
                background_color =
                    RGB(
                        gui->theme.button.r,
                        gui->theme.button.g,
                        gui->theme.button.b
                    );
            }
        }
        else
        {
            background_color =
                RGB(
                    widget->background.r,
                    widget->background.g,
                    widget->background.b
                );
        }

        brush = CreateSolidBrush(
            background_color
        );

        if (brush != NULL)
        {
            FillRect(
                hdc,
                &rectangle,
                brush
            );

            DeleteObject(brush);
        }

        SetTextColor(
            hdc,
            RGB(
                widget->foreground.r,
                widget->foreground.g,
                widget->foreground.b
            )
        );

        SetBkMode(
            hdc,
            TRANSPARENT
        );

        TextOutA(
            hdc,
            widget->x + 8,
            widget->y + 8,
            widget->text,
            (int)strlen(widget->text)
        );
    }

    EndPaint(
        gui->window,
        &paint
    );
}


static LRESULT CALLBACK gui_window_proc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    GUI* gui =
        gui_active;

    switch (message)
    {
        case WM_PAINT:
        {
            if (gui != NULL)
            {
                gui_render(gui);
                return 0;
            }

            break;
        }


        case WM_MOUSEMOVE:
        {
            if (gui != NULL)
            {
                gui->mouse_x =
                    GET_X_LPARAM(lParam);

                gui->mouse_y =
                    GET_Y_LPARAM(lParam);

                gui_update_widgets(gui);

                gui->redraw = 1;
            }

            break;
        }


        case WM_LBUTTONDOWN:
        {
            if (gui != NULL)
            {
                gui->mouse_left = 1;
                gui_update_widgets(gui);
                gui->redraw = 1;
            }

            break;
        }


        case WM_LBUTTONUP:
        {
            if (gui != NULL)
            {
                gui->mouse_left = 0;
                gui->mouse_left_released = 1;

                gui_update_widgets(gui);

                gui->redraw = 1;
            }

            break;
        }


        case WM_RBUTTONDOWN:
        {
            if (gui != NULL)
            {
                gui->mouse_right = 1;
                gui->redraw = 1;
            }

            break;
        }


        case WM_RBUTTONUP:
        {
            if (gui != NULL)
            {
                gui->mouse_right = 0;
                gui->redraw = 1;
            }

            break;
        }


        case WM_KEYDOWN:
        {
            if (gui != NULL &&
                wParam < 256)
            {
                gui->key_down[wParam] = 1;
            }

            break;
        }


        case WM_KEYUP:
        {
            if (gui != NULL &&
                wParam < 256)
            {
                gui->key_down[wParam] = 0;
            }

            break;
        }


        case WM_SIZE:
        {
            if (gui != NULL)
            {
                gui->width =
                    LOWORD(lParam);

                gui->height =
                    HIWORD(lParam);

                gui->redraw = 1;
            }

            break;
        }


        case WM_CLOSE:
        {
            if (gui != NULL)
                gui->running = 0;

            DestroyWindow(hwnd);

            return 0;
        }


        case WM_DESTROY:
        {
            if (gui != NULL)
                gui->running = 0;

            PostQuitMessage(0);

            return 0;
        }
    }

    return DefWindowProcA(
        hwnd,
        message,
        wParam,
        lParam
    );
}


int gui_init(
    GUI* gui,
    HINSTANCE instance,
    const char* title,
    int width,
    int height
)
{
    WNDCLASSA window_class;

    if (gui == NULL)
        return 0;

    memset(
        gui,
        0,
        sizeof(GUI)
    );

    gui->instance =
        instance;

    gui->width =
        width;

    gui->height =
        height;

    if (gui->width <= 0)
        gui->width = 800;

    if (gui->height <= 0)
        gui->height = 600;

    if (title == NULL)
    {
        title = "Simple Think";
    }

    strncpy(
        gui->title,
        title,
        GUI_MAX_TEXT - 1
    );

    gui->title[
        GUI_MAX_TEXT - 1
    ] = '\0';

    gui_default_theme(
        &gui->theme
    );

    gui->next_widget_id = 1;

    gui->redraw = 1;

    gui->running = 1;

    memset(
        &window_class,
        0,
        sizeof(window_class)
    );

    window_class.style =
        CS_HREDRAW |
        CS_VREDRAW;

    window_class.lpfnWndProc =
        gui_window_proc;

    window_class.hInstance =
        instance;

    window_class.hCursor =
        LoadCursorA(
            NULL,
            IDC_ARROW
        );

    window_class.hbrBackground =
        NULL;

    window_class.lpszClassName =
        GUI_CLASS_NAME;

    if (!RegisterClassA(
        &window_class
    ))
    {
        if (GetLastError() !=
            ERROR_CLASS_ALREADY_EXISTS)
        {
            return 0;
        }
    }

    gui->window =
        CreateWindowExA(
            0,
            GUI_CLASS_NAME,
            gui->title,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            gui->width,
            gui->height,
            NULL,
            NULL,
            instance,
            NULL
        );

    if (gui->window == NULL)
        return 0;

    gui_active =
        gui;

    gui->initialized =
        1;

    ShowWindow(
        gui->window,
        SW_SHOW
    );

    UpdateWindow(
        gui->window
    );

    return 1;
}


int gui_update(
    GUI* gui
)
{
    MSG message;

    if (gui == NULL ||
        !gui->initialized)
        return 0;

    gui->mouse_left_pressed = 0;
    gui->mouse_left_released = 0;

    while (PeekMessageA(
        &message,
        NULL,
        0,
        0,
        PM_REMOVE
    ))
    {
        if (message.message ==
            WM_QUIT)
        {
            gui->running = 0;
            break;
        }

        TranslateMessage(
            &message
        );

        DispatchMessageA(
            &message
        );
    }

    gui_update_widgets(
        gui
    );

    if (gui->redraw)
    {
        InvalidateRect(
            gui->window,
            NULL,
            FALSE
        );

        gui->redraw = 0;
    }

    return gui->running;
}


int gui_run(
    GUI* gui
)
{
    if (gui == NULL ||
        !gui->initialized)
        return 0;

    gui->running = 1;

    while (gui->running)
    {
        if (!gui_update(gui))
            break;

        Sleep(1);
    }

    return 1;
}


void gui_shutdown(
    GUI* gui
)
{
    if (gui == NULL)
        return;

    gui->running = 0;

    if (gui->window != NULL)
    {
        DestroyWindow(
            gui->window
        );

        gui->window = NULL;
    }

    if (gui_active == gui)
        gui_active = NULL;

    gui->initialized = 0;

    gui->widget_count = 0;
}


void gui_redraw(
    GUI* gui
)
{
    if (gui == NULL)
        return;

    gui->redraw = 1;

    if (gui->window != NULL)
    {
        InvalidateRect(
            gui->window,
            NULL,
            FALSE
        );
    }
}


void gui_set_title(
    GUI* gui,
    const char* title
)
{
    if (gui == NULL ||
        title == NULL)
        return;

    strncpy(
        gui->title,
        title,
        GUI_MAX_TEXT - 1
    );

    gui->title[
        GUI_MAX_TEXT - 1
    ] = '\0';

    if (gui->window != NULL)
    {
        SetWindowTextA(
            gui->window,
            gui->title
        );
    }

    gui_redraw(gui);
}


const char* gui_get_title(
    GUI* gui
)
{
    if (gui == NULL)
        return NULL;

    return gui->title;
}


HWND gui_get_window(
    GUI* gui
)
{
    if (gui == NULL)
        return NULL;

    return gui->window;
}


int gui_get_width(
    GUI* gui
)
{
    if (gui == NULL)
        return 0;

    return gui->width;
}


int gui_get_height(
    GUI* gui
)
{
    if (gui == NULL)
        return 0;

    return gui->height;
}


void gui_set_theme(
    GUI* gui,
    const GUITheme* theme
)
{
    if (gui == NULL ||
        theme == NULL)
        return;

    gui->theme =
        *theme;

    gui_redraw(gui);
}


GUITheme* gui_get_theme(
    GUI* gui
)
{
    if (gui == NULL)
        return NULL;

    return &gui->theme;
}


GUIWidget* gui_create_widget(
    GUI* gui,
    GUIWidgetType type,
    int x,
    int y,
    int width,
    int height,
    const char* text
)
{
    GUIWidget* widget;

    if (gui == NULL)
        return NULL;

    if (gui->widget_count >=
        GUI_MAX_WIDGETS)
        return NULL;

    widget =
        &gui->widgets[
            gui->widget_count
        ];

    memset(
        widget,
        0,
        sizeof(GUIWidget)
    );

    widget->id =
        gui->next_widget_id++;

    widget->type =
        type;

    widget->x =
        x;

    widget->y =
        y;

    widget->width =
        width;

    widget->height =
        height;

    widget->visible =
        1;

    widget->enabled =
        1;

    widget->background =
        gui->theme.panel;

    widget->foreground =
        gui->theme.text;

    widget->border =
        gui->theme.border;

    if (text != NULL)
    {
        strncpy(
            widget->text,
            text,
            GUI_MAX_TEXT - 1
        );

        widget->text[
            GUI_MAX_TEXT - 1
        ] = '\0';
    }

    gui->widget_count++;

    gui_redraw(gui);

    return widget;
}


GUIWidget* gui_panel(
    GUI* gui,
    int x,
    int y,
    int width,
    int height
)
{
    GUIWidget* widget;

    widget =
        gui_create_widget(
            gui,
            GUI_WIDGET_PANEL,
            x,
            y,
            width,
            height,
            ""
        );

    if (widget != NULL)
    {
        widget->background =
            gui->theme.panel;
    }

    return widget;
}


GUIWidget* gui_label(
    GUI* gui,
    const char* text,
    int x,
    int y
)
{
    return gui_create_widget(
        gui,
        GUI_WIDGET_LABEL,
        x,
        y,
        0,
        0,
        text
    );
}


GUIWidget* gui_button(
    GUI* gui,
    const char* text,
    int x,
    int y,
    int width,
    int height
)
{
    GUIWidget* widget;

    widget =
        gui_create_widget(
            gui,
            GUI_WIDGET_BUTTON,
            x,
            y,
            width,
            height,
            text
        );

    if (widget != NULL)
    {
        widget->background =
            gui->theme.button;
    }

    return widget;
}


GUIWidget* gui_get_widget(
    GUI* gui,
    int id
)
{
    int i;

    if (gui == NULL)
        return NULL;

    for (i = 0;
         i < gui->widget_count;
         i++)
    {
        if (gui->widgets[i].id == id)
            return &gui->widgets[i];
    }

    return NULL;
}


void gui_clear_widgets(
    GUI* gui
)
{
    if (gui == NULL)
        return;

    memset(
        gui->widgets,
        0,
        sizeof(gui->widgets)
    );

    gui->widget_count = 0;

    gui_redraw(gui);
}


void gui_widget_set_text(
    GUIWidget* widget,
    const char* text
)
{
    if (widget == NULL)
        return;

    if (text == NULL)
    {
        widget->text[0] = '\0';
        return;
    }

    strncpy(
        widget->text,
        text,
        GUI_MAX_TEXT - 1
    );

    widget->text[
        GUI_MAX_TEXT - 1
    ] = '\0';
}


void gui_widget_set_visible(
    GUIWidget* widget,
    int visible
)
{
    if (widget == NULL)
        return;

    widget->visible =
        visible ? 1 : 0;
}


void gui_widget_set_enabled(
    GUIWidget* widget,
    int enabled
)
{
    if (widget == NULL)
        return;

    widget->enabled =
        enabled ? 1 : 0;
}


void gui_widget_set_click_callback(
    GUIWidget* widget,
    void (*callback)(GUIWidget* widget)
)
{
    if (widget == NULL)
        return;

    widget->on_click =
        callback;
}


int gui_mouse_x(
    GUI* gui
)
{
    if (gui == NULL)
        return 0;

    return gui->mouse_x;
}


int gui_mouse_y(
    GUI* gui
)
{
    if (gui == NULL)
        return 0;

    return gui->mouse_y;
}


int gui_mouse_left(
    GUI* gui
)
{
    if (gui == NULL)
        return 0;

    return gui->mouse_left;
}


int gui_mouse_left_pressed(
    GUI* gui
)
{
    if (gui == NULL)
        return 0;

    return gui->mouse_left_pressed;
}


int gui_mouse_left_released(
    GUI* gui
)
{
    if (gui == NULL)
        return 0;

    return gui->mouse_left_released;
}


int gui_key_down(
    GUI* gui,
    int key
)
{
    if (gui == NULL)
        return 0;

    if (key < 0 || key >= 256)
        return 0;

    return gui->key_down[key];
}


void gui_draw_text(
    GUI* gui,
    const char* text,
    int x,
    int y
)
{
    GUIWidget* widget;

    if (gui == NULL ||
        text == NULL)
        return;

    widget =
        gui_create_widget(
            gui,
            GUI_WIDGET_LABEL,
            x,
            y,
            0,
            0,
            text
        );

    if (widget != NULL)
        gui_redraw(gui);
}


int gui_is_running(
    GUI* gui
)
{
    if (gui == NULL)
        return 0;

    return gui->running;
}