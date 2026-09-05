#ifndef ST_CONSOLE_H
#define ST_CONSOLE_H

/*
    Simple Think Console Library
*/

void st_console_init(void);
void st_console_clear(void);

void st_console_print(const char* text);
void st_console_print_line(const char* text);

void st_console_print_int(int value);
void st_console_print_float(float value);
void st_console_print_bool(int value);
void st_console_print_char(char value);

int st_console_read_line(char* buffer, int buffer_size);
int st_console_read_int(int* value);
int st_console_read_float(float* value);

void st_console_set_title(const char* title);

#endif