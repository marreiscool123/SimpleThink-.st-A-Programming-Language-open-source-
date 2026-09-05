#ifndef ST_COMPILER_H
#define ST_COMPILER_H

#include <windows.h>

#define ST_VERSION "0.1.0"
#define ST_MAX_PATH  MAX_PATH
#define ST_MAX_ERROR 512

typedef struct STCompiler {
    char input_file[ST_MAX_PATH];
    char output_file[ST_MAX_PATH];

    char* source;
    long source_size;

    int error_count;
    int warning_count;

    char last_error[ST_MAX_ERROR];

    int compiled;
} STCompiler;


/*
 * Initialize the Simple Think compiler.
 */
void st_compiler_init(STCompiler* compiler);


/*
 * Free compiler memory.
 */
void st_compiler_free(STCompiler* compiler);


/*
 * Load a .st source file.
 *
 * Returns:
 *   1 = success
 *   0 = failure
 */
int st_compiler_load(
    STCompiler* compiler,
    const char* filename
);


/*
 * Compile the loaded Simple Think source.
 *
 * Returns:
 *   1 = success
 *   0 = failure
 */
int st_compiler_compile(
    STCompiler* compiler
);


/*
 * Write the compiled program to an .exe file.
 *
 * Returns:
 *   1 = success
 *   0 = failure
 */
int st_compiler_write_exe(
    STCompiler* compiler,
    const char* filename
);


/*
 * Compile a .st file directly into an .exe.
 *
 * Example:
 *
 * st_compiler_build(
 *     &compiler,
 *     "main.st",
 *     "program.exe"
 * );
 */
int st_compiler_build(
    STCompiler* compiler,
    const char* input_file,
    const char* output_file
);


/*
 * Set a compiler error.
 */
void st_compiler_error(
    STCompiler* compiler,
    const char* message
);


/*
 * Print compiler information.
 */
void st_compiler_print_version(void);

#endif /* ST_COMPILER_H */