#include "console.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void st_console_init(void)
{
    HANDLE output;
    HANDLE input;

    output = GetStdHandle(STD_OUTPUT_HANDLE);
    input = GetStdHandle(STD_INPUT_HANDLE);

    if (output == INVALID_HANDLE_VALUE)
        return;

    if (input == INVALID_HANDLE_VALUE)
        return;
}

void st_console_clear(void)
{
    HANDLE console;
    CONSOLE_SCREEN_BUFFER_INFO info;
    DWORD cells;
    DWORD written;
    COORD position;

    console = GetStdHandle(STD_OUTPUT_HANDLE);

    if (console == INVALID_HANDLE_VALUE)
        return;

    if (!GetConsoleScreenBufferInfo(console, &info))
        return;

    cells = (DWORD)info.dwSize.X * (DWORD)info.dwSize.Y;
    position.X = 0;
    position.Y = 0;

    FillConsoleOutputCharacterA(
        console,
        ' ',
        cells,
        position,
        &written
    );

    FillConsoleOutputAttribute(
        console,
        info.wAttributes,
        cells,
        position,
        &written
    );

    SetConsoleCursorPosition(console, position);
}

void st_console_print(const char* text)
{
    if (text == NULL)
        return;

    printf("%s", text);
}

void st_console_print_line(const char* text)
{
    if (text == NULL)
    {
        printf("\n");
        return;
    }

    printf("%s\n", text);
}

void st_console_print_int(int value)
{
    printf("%d", value);
}

void st_console_print_float(float value)
{
    printf("%f", value);
}

void st_console_print_bool(int value)
{
    printf("%s", value ? "true" : "false");
}

void st_console_print_char(char value)
{
    printf("%c", value);
}

int st_console_read_line(char* buffer, int buffer_size)
{
    size_t length;

    if (buffer == NULL || buffer_size <= 0)
        return 0;

    if (fgets(buffer, buffer_size, stdin) == NULL)
        return 0;

    length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n')
        buffer[length - 1] = '\0';

    return 1;
}

int st_console_read_int(int* value)
{
    char buffer[128];
    char* end;
    long result;

    if (value == NULL)
        return 0;

    if (!st_console_read_line(buffer, sizeof(buffer)))
        return 0;

    result = strtol(buffer, &end, 10);

    if (end == buffer)
        return 0;

    *value = (int)result;

    return 1;
}

int st_console_read_float(float* value)
{
    char buffer[128];
    char* end;
    float result;

    if (value == NULL)
        return 0;

    if (!st_console_read_line(buffer, sizeof(buffer)))
        return 0;

    result = (float)strtod(buffer, &end);

    if (end == buffer)
        return 0;

    *value = result;

    return 1;
}

void st_console_set_title(const char* title)
{
    if (title == NULL)
        return;

    SetConsoleTitleA(title);
}