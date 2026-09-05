#include <stdio.h>
#include <string.h>

#include "connect.h"

#define SIMPLE_THINK_NAME    "Simple Think"
#define SIMPLE_THINK_VERSION "1.0.0"

static void st_print_version(void)
{
    printf("%s %s\n", SIMPLE_THINK_NAME, SIMPLE_THINK_VERSION);
}

static void st_print_help(void)
{
    printf("Simple Think Compiler %s\n", SIMPLE_THINK_VERSION);
    printf("\n");
    printf("Usage:\n");
    printf("  stc [options] <source> [output]\n");
    printf("\n");
    printf("Options:\n");
    printf("  --version    Show compiler version\n");
    printf("  --help       Show this help message\n");
}

int main(int argc, char** argv)
{
    STContext context;
    int result;
    const char* input_file;
    const char* output_file;

    if (argc > 1)
    {
        if (strcmp(argv[1], "--version") == 0 ||
            strcmp(argv[1], "-v") == 0)
        {
            st_print_version();
            return 0;
        }

        if (strcmp(argv[1], "--help") == 0 ||
            strcmp(argv[1], "-h") == 0)
        {
            st_print_help();
            return 0;
        }
    }

    if (argc < 2)
    {
        fprintf(stderr, "Simple Think: no input specified.\n");
        fprintf(stderr, "Use \"stc --help\" for usage information.\n");
        return 1;
    }

    input_file = argv[1];

    if (argc >= 3)
        output_file = argv[2];
    else
        output_file = "output.exe";

    if (!st_connect_init(&context))
    {
        fprintf(stderr, "Simple Think: failed to initialize.\n");

        st_error_print_all(&context.errors);

        return 1;
    }

    result = st_connect_build(
        &context,
        input_file,
        output_file
    );

    if (!result)
    {
        fprintf(stderr, "Simple Think: compilation failed.\n");
        fprintf(stderr, "\n");

        st_error_print_all(&context.errors);

        st_connect_shutdown(&context);
        return 1;
    }

    printf("Simple Think: build successful.\n");
    printf("Output: %s\n", output_file);

    st_connect_shutdown(&context);

    return 0;
}