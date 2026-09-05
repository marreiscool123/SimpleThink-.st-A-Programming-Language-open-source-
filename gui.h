#ifndef GUI_H
#define GUI_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif


#define GUI_MAX_WIDGETS 256
#define GUI_MAX_TEXT 512


typedef struct GUIColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} GUIColor;


typedef enum GUIWidgetType
{
    GUI_WIDGET_NONE = 0,
    GUI_WIDGET_PANEL,
    GUI_WIDGET_LABEL,
    GUI_WIDGET_BUTTON
} GUIWidgetType;


typedef struct GUIWidget
{
    int id;

    GUIWidgetType type;

    int x;
    int y;
    int width;
    int height;

    char text[GUI_MAX_TEXT];

    GUIColor background;
    GUIColor foreground;
    GUIColor border;

    int visible;
    int enabled;

    int hovered;
    int pressed;

    void (*on_click)(struct GUIWidget* widget);

    void* user_data;
} GUIWidget;


typedef struct GUITheme
{
    GUIColor background;
    GUIColor panel;
    GUIColor button;
    GUIColor button_hover;
    GUIColor button_pressed;
    GUIColor text;
    GUIColor border;
} GUITheme;


typedef struct GUI
{
    HINSTANCE instance;
    HWND window;

    char title[GUI_MAX_TEXT];

    int width;
    int height;

    int running;
    int initialized;

    int mouse_x;
    int mouse_y;

    int mouse_left;
    int mouse_right;

    int mouse_left_pressed;
    int mouse_left_released;

    int key_down[256];

    GUITheme theme;

    GUIWidget widgets[GUI_MAX_WIDGETS];
    int widget_count;

    int next_widget_id;

    int redraw;

    void* user_data;
} GUI;


/*
 * Initialize the Simple Think GUI system.
 */
int gui_init(
    GUI* gui,
    HINSTANCE instance,
    const char* title,
    int width,
    int height
);


/*
 * Run the GUI message loop.
 */
int gui_run(
    GUI* gui
);


/*
 * Process pending window/input events.
 */
int gui_update(
    GUI* gui
);


/*
 * Shut down the GUI.
 */
void gui_shutdown(
    GUI* gui
);


/*
 * Request a redraw.
 */
void gui_redraw(
    GUI* gui
);


/*
 * Set the window title.
 */
void gui_set_title(
    GUI* gui,
    const char* title
);


/*
 * Get the window title.
 */
const char* gui_get_title(
    GUI* gui
);


/*
 * Get the window handle.
 */
HWND gui_get_window(
    GUI* gui
);


/*
 * Get the GUI width.
 */
int gui_get_width(
    GUI* gui
);


/*
 * Get the GUI height.
 */
int gui_get_height(
    GUI* gui
);


/*
 * Set the GUI theme.
 */
void gui_set_theme(
    GUI* gui,
    const GUITheme* theme
);


/*
 * Get the current GUI theme.
 */
GUITheme* gui_get_theme(
    GUI* gui
);


/*
 * Add a generic widget.
 */
GUIWidget* gui_create_widget(
    GUI* gui,
    GUIWidgetType type,
    int x,
    int y,
    int width,
    int height,
    const char* text
);


/*
 * Add a panel.
 */
GUIWidget* gui_panel(
    GUI* gui,
    int x,
    int y,
    int width,
    int height
);


/*
 * Add a label.
 */
GUIWidget* gui_label(
    GUI* gui,
    const char* text,
    int x,
    int y
);


/*
 * Add a button.
 */
GUIWidget* gui_button(
    GUI* gui,
    const char* text,
    int x,
    int y,
    int width,
    int height
);


/*
 * Find a widget by ID.
 */
GUIWidget* gui_get_widget(
    GUI* gui,
    int id
);


/*
 * Remove all widgets.
 */
void gui_clear_widgets(
    GUI* gui
);


/*
 * Set widget text.
 */
void gui_widget_set_text(
    GUIWidget* widget,
    const char* text
);


/*
 * Set widget visibility.
 */
void gui_widget_set_visible(
    GUIWidget* widget,
    int visible
);


/*
 * Set widget enabled state.
 */
void gui_widget_set_enabled(
    GUIWidget* widget,
    int enabled
);


/*
 * Set widget click callback.
 */
void gui_widget_set_click_callback(
    GUIWidget* widget,
    void (*callback)(GUIWidget* widget)
);


/*
 * Get mouse X position.
 */
int gui_mouse_x(
    GUI* gui
);


/*
 * Get mouse Y position.
 */
int gui_mouse_y(
    GUI* gui
);


/*
 * Check whether the left mouse button is held.
 */
int gui_mouse_left(
    GUI* gui
);


/*
 * Check whether the left mouse button was pressed.
 */
int gui_mouse_left_pressed(
    GUI* gui
);


/*
 * Check whether the left mouse button was released.
 */
int gui_mouse_left_released(
    GUI* gui
);


/*
 * Check whether a keyboard key is held.
 */
int gui_key_down(
    GUI* gui,
    int key
);


/*
 * Draw text directly through the GUI system.
 *
 * This remains for compatibility with the existing
 * Simple Think GUI API.
 */
void gui_draw_text(
    GUI* gui,
    const char* text,
    int x,
    int y
);


/*
 * Check whether the GUI is still running.
 */
int gui_is_running(
    GUI* gui
);


#ifdef __cplusplus
}
#endif

#endif