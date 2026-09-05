#include "compiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


static int st_write_gui_program(
    STCompiler* compiler,
    const char* filename
)
{
    FILE* output_file;

    char title[512];

    int width;
    int height;

    const char* gui_init;
    const char* gui_title;
    const char* gui_text;

    int text_x;
    int text_y;
    char text_value[512];

    char temp_c[MAX_PATH];
    char command[4096];
    char temp_dir[MAX_PATH];

    if (compiler == NULL || filename == NULL)
        return 0;

    title[0] = '\0';

    width = 800;
    height = 600;

    text_x = 20;
    text_y = 20;
    text_value[0] = '\0';


    /*
        Read gui_init().
    */

    gui_init = strstr(
        compiler->source,
        "gui_init("
    );

    if (gui_init != NULL)
    {
        if (sscanf(
            gui_init,
            "gui_init(\"%511[^\"]\", %d, %d);",
            title,
            &width,
            &height
        ) != 3)
        {
            title[0] = '\0';
        }
    }


    /*
        Read gui_set_title().
    */

    gui_title = strstr(
        compiler->source,
        "gui_set_title("
    );

    if (gui_title != NULL)
    {
        char new_title[512];

        new_title[0] = '\0';

        if (sscanf(
            gui_title,
            "gui_set_title(\"%511[^\"]\");",
            new_title
        ) == 1)
        {
            strncpy(
                title,
                new_title,
                sizeof(title) - 1
            );

            title[sizeof(title) - 1] = '\0';
        }
    }


    /*
        Read our own text function.
    */

    gui_text = strstr(
        compiler->source,
        "gui_draw_text("
    );

    if (gui_text != NULL)
    {
        if (sscanf(
            gui_text,
            "gui_draw_text(%d, %d, \"%511[^\"]\");",
            &text_x,
            &text_y,
            text_value
        ) != 3)
        {
            text_value[0] = '\0';
        }
    }


    /*
        Default title.
    */

    if (title[0] == '\0')
    {
        strncpy(
            title,
            "Simple Think",
            sizeof(title) - 1
        );

        title[sizeof(title) - 1] = '\0';
    }


    if (width <= 0)
        width = 800;

    if (height <= 0)
        height = 600;


    /*
        Temporary directory.
    */

    if (GetTempPathA(
        MAX_PATH,
        temp_dir
    ) == 0)
    {
        st_compiler_error(
            compiler,
            "Could not determine temporary directory."
        );

        return 0;
    }


    snprintf(
        temp_c,
        sizeof(temp_c),
        "%sSimpleThink_%lu.c",
        temp_dir,
        (unsigned long)GetCurrentProcessId()
    );


    output_file = fopen(
        temp_c,
        "wb"
    );

    if (output_file == NULL)
    {
        st_compiler_error(
            compiler,
            "Could not create temporary C source file."
        );

        return 0;
    }


    /*
        Generate temporary executable.

        Windows is only used for:
        - creating the window
        - presenting our framebuffer

        Text is drawn by our own 5x7 bitmap font.
    */

    fprintf(
        output_file,

        "#include <windows.h>\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "\n"

        "static const char* st_title = \"%s\";\n"
        "static int st_width = %d;\n"
        "static int st_height = %d;\n"
        "\n"

        "static unsigned int* st_pixels = NULL;\n"
        "\n"

        "static unsigned char st_font[128][7] =\n"
        "{\n"

        "    [' '] = {0,0,0,0,0,0,0},\n"

        "    ['A'] = {14,17,17,31,17,17,17},\n"
        "    ['B'] = {30,17,17,30,17,17,30},\n"
        "    ['C'] = {14,17,16,16,16,17,14},\n"
        "    ['D'] = {30,17,17,17,17,17,30},\n"
        "    ['E'] = {31,16,16,30,16,16,31},\n"
        "    ['F'] = {31,16,16,30,16,16,16},\n"
        "    ['G'] = {14,17,16,23,17,17,14},\n"
        "    ['H'] = {17,17,17,31,17,17,17},\n"
        "    ['I'] = {31,4,4,4,4,4,31},\n"
        "    ['J'] = {7,2,2,2,2,18,12},\n"
        "    ['K'] = {17,18,20,24,20,18,17},\n"
        "    ['L'] = {16,16,16,16,16,16,31},\n"
        "    ['M'] = {17,27,21,21,17,17,17},\n"
        "    ['N'] = {17,25,21,19,17,17,17},\n"
        "    ['O'] = {14,17,17,17,17,17,14},\n"
        "    ['P'] = {30,17,17,30,16,16,16},\n"
        "    ['Q'] = {14,17,17,17,21,18,13},\n"
        "    ['R'] = {30,17,17,30,20,18,17},\n"
        "    ['S'] = {15,16,16,14,1,1,30},\n"
        "    ['T'] = {31,4,4,4,4,4,4},\n"
        "    ['U'] = {17,17,17,17,17,17,14},\n"
        "    ['V'] = {17,17,17,17,17,10,4},\n"
        "    ['W'] = {17,17,17,21,21,21,10},\n"
        "    ['X'] = {17,17,10,4,10,17,17},\n"
        "    ['Y'] = {17,17,10,4,4,4,4},\n"
        "    ['Z'] = {31,1,2,4,8,16,31},\n"

        "    ['0'] = {14,17,19,21,25,17,14},\n"
        "    ['1'] = {4,12,4,4,4,4,14},\n"
        "    ['2'] = {14,17,1,2,4,8,31},\n"
        "    ['3'] = {30,1,1,14,1,1,30},\n"
        "    ['4'] = {2,6,10,18,31,2,2},\n"
        "    ['5'] = {31,16,16,30,1,1,30},\n"
        "    ['6'] = {14,16,16,30,17,17,14},\n"
        "    ['7'] = {31,1,2,4,8,8,8},\n"
        "    ['8'] = {14,17,17,14,17,17,14},\n"
        "    ['9'] = {14,17,17,15,1,1,14},\n"

        "    ['.'] = {0,0,0,0,0,0,4},\n"
        "    [','] = {0,0,0,0,0,4,8},\n"
        "    ['!'] = {4,4,4,4,4,0,4},\n"
        "    ['?'] = {14,17,1,2,4,0,4},\n"
        "    ['-'] = {0,0,0,31,0,0,0},\n"
        "    ['_'] = {0,0,0,0,0,0,31},\n"
        "    [':'] = {0,4,0,0,0,4,0},\n"
        "    ['('] = {2,4,8,8,8,4,2},\n"
        "    [')'] = {8,4,2,2,2,4,8},\n"
        "    ['/'] = {1,1,2,4,8,16,16},\n"
        "    ['+'] = {0,4,4,31,4,4,0}\n"

        "};\n"
        "\n"

        "static void st_pixel(\n"
        "    int x,\n"
        "    int y,\n"
        "    unsigned int color\n"
        ")\n"
        "{\n"
        "    if (st_pixels == NULL)\n"
        "        return;\n"
        "\n"
        "    if (x < 0 || y < 0)\n"
        "        return;\n"
        "\n"
        "    if (x >= st_width || y >= st_height)\n"
        "        return;\n"
        "\n"
        "    st_pixels[y * st_width + x] = color;\n"
        "}\n"
        "\n"

        "static void st_draw_char(\n"
        "    int x,\n"
        "    int y,\n"
        "    char character,\n"
        "    unsigned int color\n"
        ")\n"
        "{\n"
        "    int row;\n"
        "    int column;\n"
        "    unsigned char bits;\n"
        "\n"
        "    if ((unsigned char)character >= 128)\n"
        "        character = '?';\n"
        "\n"
        "    for (row = 0; row < 7; row++)\n"
        "    {\n"
        "        bits = st_font[(unsigned char)character][row];\n"
        "\n"
        "        for (column = 0; column < 5; column++)\n"
        "        {\n"
        "            if (bits & (1 << (4 - column)))\n"
        "            {\n"
        "                st_pixel(\n"
        "                    x + column,\n"
        "                    y + row,\n"
        "                    color\n"
        "                );\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}\n"
        "\n"

        "static void st_draw_text(\n"
        "    int x,\n"
        "    int y,\n"
        "    const char* text,\n"
        "    unsigned int color\n"
        ")\n"
        "{\n"
        "    int cursor_x;\n"
        "\n"
        "    if (text == NULL)\n"
        "        return;\n"
        "\n"
        "    cursor_x = x;\n"
        "\n"
        "    while (*text != '\\0')\n"
        "    {\n"
        "        char character = *text;\n"
        "\n"
        "        if (character >= 'a' && character <= 'z')\n"
        "            character = (char)(character - 'a' + 'A');\n"
        "\n"
        "        st_draw_char(\n"
        "            cursor_x,\n"
        "            y,\n"
        "            character,\n"
        "            color\n"
        "        );\n"
        "\n"
        "        cursor_x += 6;\n"
        "        text++;\n"
        "    }\n"
        "}\n"
        "\n"

        "static void st_present(HWND hwnd)\n"
        "{\n"
        "    PAINTSTRUCT ps;\n"
        "    HDC dc;\n"
        "    BITMAPINFO info;\n"
        "\n"
        "    dc = BeginPaint(hwnd, &ps);\n"
        "\n"
        "    ZeroMemory(&info, sizeof(info));\n"
        "\n"
        "    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);\n"
        "    info.bmiHeader.biWidth = st_width;\n"
        "    info.bmiHeader.biHeight = -st_height;\n"
        "    info.bmiHeader.biPlanes = 1;\n"
        "    info.bmiHeader.biBitCount = 32;\n"
        "    info.bmiHeader.biCompression = BI_RGB;\n"
        "\n"
        "    StretchDIBits(\n"
        "        dc,\n"
        "        0,\n"
        "        0,\n"
        "        st_width,\n"
        "        st_height,\n"
        "        0,\n"
        "        0,\n"
        "        st_width,\n"
        "        st_height,\n"
        "        st_pixels,\n"
        "        &info,\n"
        "        DIB_RGB_COLORS,\n"
        "        SRCCOPY\n"
        "    );\n"
        "\n"
        "    EndPaint(hwnd, &ps);\n"
        "}\n"
        "\n"

        "static LRESULT CALLBACK st_window_proc(\n"
        "    HWND hwnd,\n"
        "    UINT message,\n"
        "    WPARAM wParam,\n"
        "    LPARAM lParam\n"
        ")\n"
        "{\n"
        "    (void)wParam;\n"
        "    (void)lParam;\n"
        "\n"
        "    switch (message)\n"
        "    {\n"
        "        case WM_PAINT:\n"
        "            st_present(hwnd);\n"
        "            return 0;\n"
        "\n"
        "        case WM_ERASEBKGND:\n"
        "            return 1;\n"
        "\n"
        "        case WM_DESTROY:\n"
        "            PostQuitMessage(0);\n"
        "            return 0;\n"
        "    }\n"
        "\n"
        "    return DefWindowProcA(\n"
        "        hwnd,\n"
        "        message,\n"
        "        wParam,\n"
        "        lParam\n"
        "    );\n"
        "}\n"
        "\n"

        "int WINAPI WinMain(\n"
        "    HINSTANCE instance,\n"
        "    HINSTANCE previous,\n"
        "    LPSTR command_line,\n"
        "    int show_command\n"
        ")\n"
        "{\n"
        "    WNDCLASSA window_class;\n"
        "    HWND window;\n"
        "    MSG message;\n"
        "\n"
        "    (void)previous;\n"
        "    (void)command_line;\n"
        "\n"
        "    st_pixels = calloc(\n"
        "        (size_t)st_width * (size_t)st_height,\n"
        "        sizeof(unsigned int)\n"
        "    );\n"
        "\n"
        "    if (st_pixels == NULL)\n"
        "        return 1;\n"
        "\n"
        "    memset(\n"
        "        st_pixels,\n"
        "        0xFF,\n"
        "        (size_t)st_width * (size_t)st_height * sizeof(unsigned int)\n"
        "    );\n"
        "\n"
        "    ZeroMemory(\n"
        "        &window_class,\n"
        "        sizeof(window_class)\n"
        "    );\n"
        "\n"
        "    window_class.lpfnWndProc = st_window_proc;\n"
        "    window_class.hInstance = instance;\n"
        "    window_class.lpszClassName = \"SimpleThinkWindow\";\n"
        "    window_class.hCursor = LoadCursorA(NULL, IDC_ARROW);\n"
        "    window_class.hbrBackground = NULL;\n"
        "\n"
        "    if (!RegisterClassA(&window_class))\n"
        "    {\n"
        "        free(st_pixels);\n"
        "        return 1;\n"
        "    }\n"
        "\n"
        "    window = CreateWindowExA(\n"
        "        0,\n"
        "        \"SimpleThinkWindow\",\n"
        "        st_title,\n"
        "        WS_OVERLAPPEDWINDOW,\n"
        "        CW_USEDEFAULT,\n"
        "        CW_USEDEFAULT,\n"
        "        st_width,\n"
        "        st_height,\n"
        "        NULL,\n"
        "        NULL,\n"
        "        instance,\n"
        "        NULL\n"
        "    );\n"
        "\n"
        "    if (window == NULL)\n"
        "    {\n"
        "        free(st_pixels);\n"
        "        return 1;\n"
        "    }\n"
        "\n"

        "    st_draw_text(\n"
        "        %d,\n"
        "        %d,\n"
        "        \"%s\",\n"
        "        0xFF000000\n"
        "    );\n"
        "\n"

        "    ShowWindow(window, show_command);\n"
        "    UpdateWindow(window);\n"
        "\n"
        "    while (GetMessageA(&message, NULL, 0, 0) > 0)\n"
        "    {\n"
        "        TranslateMessage(&message);\n"
        "        DispatchMessageA(&message);\n"
        "    }\n"
        "\n"
        "    free(st_pixels);\n"
        "\n"
        "    return (int)message.wParam;\n"
        "}\n",

        title,
        width,
        height,
        text_x,
        text_y,
        text_value
    );

    fclose(output_file);


    /*
        Compile generated program.
    */

    snprintf(
        command,
        sizeof(command),
        "gcc -mwindows \"%s\" -o \"%s\"",
        temp_c,
        filename
    );

    if (system(command) != 0)
    {
        snprintf(
            command,
            sizeof(command),
            "clang -mwindows \"%s\" -o \"%s\"",
            temp_c,
            filename
        );

        if (system(command) != 0)
        {
            DeleteFileA(temp_c);

            st_compiler_error(
                compiler,
                "Could not generate Windows executable."
            );

            return 0;
        }
    }


    DeleteFileA(temp_c);

    return 1;
}


void st_compiler_init(
    STCompiler* compiler
)
{
    if (compiler == NULL)
        return;

    memset(
        compiler,
        0,
        sizeof(STCompiler)
    );

    compiler->compiled = 0;
}


void st_compiler_free(
    STCompiler* compiler
)
{
    if (compiler == NULL)
        return;

    if (compiler->source != NULL)
    {
        free(compiler->source);
        compiler->source = NULL;
    }

    compiler->source_size = 0;
}


void st_compiler_error(
    STCompiler* compiler,
    const char* message
)
{
    if (compiler == NULL)
        return;

    compiler->error_count++;

    if (message == NULL)
    {
        compiler->last_error[0] = '\0';
        return;
    }

    strncpy(
        compiler->last_error,
        message,
        ST_MAX_ERROR - 1
    );

    compiler->last_error[ST_MAX_ERROR - 1] = '\0';

    fprintf(
        stderr,
        "Simple Think error: %s\n",
        compiler->last_error
    );
}


int st_compiler_load(
    STCompiler* compiler,
    const char* filename
)
{
    FILE* file;
    long size;

    if (compiler == NULL || filename == NULL)
        return 0;

    file = fopen(
        filename,
        "rb"
    );

    if (file == NULL)
    {
        st_compiler_error(
            compiler,
            "Could not open source file."
        );

        return 0;
    }

    fseek(
        file,
        0,
        SEEK_END
    );

    size = ftell(file);

    fseek(
        file,
        0,
        SEEK_SET
    );

    if (size < 0)
    {
        fclose(file);

        st_compiler_error(
            compiler,
            "Could not determine source file size."
        );

        return 0;
    }

    compiler->source = malloc(
        (size_t)size + 1
    );

    if (compiler->source == NULL)
    {
        fclose(file);

        st_compiler_error(
            compiler,
            "Not enough memory to load source file."
        );

        return 0;
    }

    if (fread(
        compiler->source,
        1,
        (size_t)size,
        file
    ) != (size_t)size)
    {
        fclose(file);

        free(compiler->source);

        compiler->source = NULL;

        st_compiler_error(
            compiler,
            "Could not read source file."
        );

        return 0;
    }

    compiler->source[size] = '\0';

    compiler->source_size = size;

    fclose(file);


    strncpy(
        compiler->input_file,
        filename,
        ST_MAX_PATH - 1
    );

    compiler->input_file[ST_MAX_PATH - 1] = '\0';

    return 1;
}


int st_compiler_compile(
    STCompiler* compiler
)
{
    if (compiler == NULL)
        return 0;

    if (compiler->source == NULL)
    {
        st_compiler_error(
            compiler,
            "No source file has been loaded."
        );

        return 0;
    }

    printf(
        "Compiling %s...\n",
        compiler->input_file
    );

    compiler->compiled = 1;

    printf(
        "Compilation successful.\n"
    );

    return 1;
}


int st_compiler_write_exe(
    STCompiler* compiler,
    const char* filename
)
{
    if (compiler == NULL || filename == NULL)
        return 0;

    if (!compiler->compiled)
    {
        st_compiler_error(
            compiler,
            "Program has not been compiled."
        );

        return 0;
    }

    if (!st_write_gui_program(
        compiler,
        filename
    ))
    {
        return 0;
    }

    strncpy(
        compiler->output_file,
        filename,
        ST_MAX_PATH - 1
    );

    compiler->output_file[ST_MAX_PATH - 1] = '\0';

    printf(
        "Output: %s\n",
        compiler->output_file
    );

    return 1;
}


int st_compiler_build(
    STCompiler* compiler,
    const char* input_file,
    const char* output_file
)
{
    if (compiler == NULL)
        return 0;

    if (!st_compiler_load(
        compiler,
        input_file
    ))
    {
        return 0;
    }

    if (!st_compiler_compile(
        compiler
    ))
    {
        return 0;
    }

    if (!st_compiler_write_exe(
        compiler,
        output_file
    ))
    {
        return 0;
    }

    return 1;
}


void st_compiler_print_version(void)
{
    printf(
        "Simple Think Compiler (stc) %s\n",
        ST_VERSION
    );
}