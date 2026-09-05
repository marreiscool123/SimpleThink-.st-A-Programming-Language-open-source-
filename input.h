#ifndef ST_INPUT_H
#define ST_INPUT_H

#include <windows.h>

/*
    Simple Think Input Library
*/

int st_input_key_down(int key);
int st_input_key_pressed(int key);
int st_input_key_released(int key);

int st_input_mouse_x(HWND window);
int st_input_mouse_y(HWND window);

int st_input_mouse_button_down(int button);
int st_input_mouse_button_pressed(int button);
int st_input_mouse_button_released(int button);

void st_input_update(void);

#endif