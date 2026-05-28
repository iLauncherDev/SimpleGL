#include <default.h>
#include <simplegl/compiler.h>

char *file_path = NULL;
bool is_fragment = false;
bool is_opengl_3 = false;

char *readFile(char *path)
{
    size_t bytes = 0;
    long position = 0;
    FILE *file;
    char *string = NULL;

    file = fopen(path, "rb");
    if (!file)
    {
        printf("file not found!\n");
        goto errorL;
    }

    fseek(file, 0, SEEK_END);

    position = ftell(file);

    fseek(file, 0, SEEK_SET);

    string = malloc(position + 1);
    if (!string)
    {
        printf("cannot allocate string\n");
        goto errorL;
    }

    string[position] = '\0';

    bytes = fread(string, 1, position, file);
    if (bytes != position)
    {
        printf("cannot read string\n");
        goto errorL;
    }

    printf("bytes: %llu, size: %ld\n", (long long)bytes, position);

    return string;

errorL:
    if (file)
        fclose(file);
    if (string)
        free(string);

    return NULL;
}

int processArgs(int argc, char **argv)
{
    int skips = 1;

    for (int i = 0; i < argc; i += skips)
    {
        int remainding_i = argc - i;
        skips = 1;

        char **argv_window = &argv[i];
        char *string = argv_window[0];

        if (!simplegl_strcasecmp(string, "-file"))
        {
            skips = 2;

            if (remainding_i < skips)
            {
                printf("not enough parameters\n");
                return 1;
            }

            file_path = argv_window[1];
        }
        else if (!simplegl_strcasecmp(string, "-is_fragment"))
        {
            is_fragment = true;
        }
        else if (!simplegl_strcasecmp(string, "-is_opengl_3"))
        {
            is_opengl_3 = true;
        }
    }

    return 0;
}

simplegl_shader_temp_entry_t temp_entry[] = {
    {
        .type = SIMPLEGL_SHADER_TEMP_TYPE_TEXT,
        .text = GLSL(
            UNIFORM float uDither4x4[16];),
    },

    {.type = SIMPLEGL_SHADER_TEMP_TYPE_INSERT_CODE},
};

simplegl_shader_temp_header_t temp = {
    .temp = temp_entry,
    .entries = sizeof(temp_entry) / sizeof(*temp_entry),
};

int main(int argc, char **argv)
{
    char *newCode = NULL;
    char *code = NULL;
    simplegl_status_t status = SIMPLEGL_STATUS_SUCCESS;

    int result = processArgs(argc, argv);
    if (result)
        return result;

    if (!file_path)
    {
        printf("no file path was specified!\n");
        return 1;
    }

    code = readFile(file_path);
    if (!code)
    {
        status = SIMPLEGL_STATUS_ALLOCATION_ERROR;
        goto result;
    }

    simplegl_compiler_info_t info = {0};

    info.is_fragment = is_fragment;
    info.is_opengl_3 = is_opengl_3;
    info.temp = &temp;

    SIMPLEGL_REQUIRE_SUCCESS(simplegl_compile_shader(&info, code, &newCode), result, status);

    printf("%s\n", newCode);

result:
    if (newCode)
        free(newCode);

    if (code)
        free(code);

    return 0;
}
