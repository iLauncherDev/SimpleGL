#include <compiler.h>

static simplegl_status_t add_shader_concat(simplegl_list_entry_t *shader_entry_list, char *text)
{
    SIMPLEGL_ASSERT(shader_entry_list != NULL);
    SIMPLEGL_ASSERT(text != NULL);

    simplegl_status_t status = SIMPLEGL_STATUS_SUCCESS;
    simplegl_shader_entry_t *entry = malloc(sizeof(*entry));
    if (!entry)
    {
        status = SIMPLEGL_STATUS_ALLOCATION_ERROR;
        goto result;
    }

    memclr(entry, sizeof(*entry));

    int text_len = strlen(text);

    entry->text = malloc(text_len + 1);
    if (!entry->text)
    {
        status = SIMPLEGL_STATUS_ALLOCATION_ERROR;
        goto result;
    }

    memcpy(entry->text, text, text_len);
    entry->text[text_len] = '\0';

    simplegl_init_list_entry(&entry->list_entry, entry);
    simplegl_insert_tail_list(shader_entry_list, &entry->list_entry);

result:
    if (!SIMPLEGL_SUCCESS(status))
    {
        if (entry && entry->text)
            free(entry->text);

        if (entry)
            free(entry);
    }

    return status;
}

static simplegl_status_t add_compiler_define(simplegl_compiler_info_t *info, simplegl_list_entry_t *shader_entry_list, char *name, char *vertex, char *fragment)
{
    SIMPLEGL_ASSERT(vertex != NULL || fragment != NULL);

    simplegl_status_t status = SIMPLEGL_STATUS_SUCCESS;

    char *currentString = info->is_fragment ? fragment : vertex;
    if (!currentString)
        currentString = vertex ? vertex : fragment;

    char tempBuffer[4096];
    tempBuffer[sizeof(tempBuffer) - 1] = '\0';

    snprintf(tempBuffer, sizeof(tempBuffer) - 1, "#define %s %s\n", name, currentString);
    SIMPLEGL_REQUIRE_SUCCESS(add_shader_concat(shader_entry_list, tempBuffer), result, status);

result:
    return status;
}

static size_t simplegl_get_code_size(simplegl_list_entry_t *shader_entry_list)
{
    size_t code_size = 0;

    simplegl_list_entry_t *end_shader_entry = shader_entry_list;
    simplegl_list_entry_t *current_shader_entry = end_shader_entry->next;

    while (current_shader_entry != end_shader_entry)
    {
        simplegl_shader_entry_t *shader_entry = simplegl_get_list_entry_structure(current_shader_entry);

        code_size += strlen(shader_entry->text);

        current_shader_entry = current_shader_entry->next;
    }

    return code_size;
}

static char *simplegl_get_code_text(simplegl_list_entry_t *shader_entry_list)
{
    size_t cursor_offset = 0;
    size_t code_size = simplegl_get_code_size(shader_entry_list);
    char *out_code = malloc(code_size + 1);
    if (!out_code)
    {
        return NULL;
    }

    simplegl_list_entry_t *end_shader_entry = shader_entry_list;
    simplegl_list_entry_t *current_shader_entry = end_shader_entry->next;

    while (current_shader_entry != end_shader_entry)
    {
        simplegl_shader_entry_t *shader_entry = simplegl_get_list_entry_structure(current_shader_entry);

        size_t text_len = strlen(shader_entry->text);

        memcpy(&out_code[cursor_offset], shader_entry->text, text_len);
        cursor_offset += text_len;

        current_shader_entry = current_shader_entry->next;
    }

    out_code[code_size] = '\0';
    return out_code;
}

static void simplegl_free_list(simplegl_list_entry_t *shader_entry_list)
{
    simplegl_list_entry_t *end_shader_entry = shader_entry_list;
    simplegl_list_entry_t *current_shader_entry = end_shader_entry->next;

    while (current_shader_entry != end_shader_entry)
    {
        simplegl_list_entry_t *next_shader_entry = current_shader_entry->next;
        simplegl_shader_entry_t *shader_entry = simplegl_get_list_entry_structure(current_shader_entry);

        free(shader_entry->text);
        free(shader_entry);

        current_shader_entry = next_shader_entry;
    }
}

simplegl_status_t SIMPLEGL_API simplegl_compile_shader(simplegl_compiler_info_t *info, char *code, char **out)
{
    SIMPLEGL_ASSERT(out != NULL);

    simplegl_status_t status = SIMPLEGL_STATUS_SUCCESS;

    simplegl_list_entry_t shader_entry_list;
    simplegl_init_list_entry(&shader_entry_list, &shader_entry_list);

    if (info->header)
        SIMPLEGL_REQUIRE_SUCCESS(add_shader_concat(&shader_entry_list, info->header), result, status);

    if (info->is_opengl_3)
    {
        char tempBuffer[4096];
        tempBuffer[sizeof(tempBuffer) - 1] = '\0';

        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "UNIFORM", "uniform", NULL), result, status);
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "INPUT", "in", NULL), result, status);
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "OUTPUT", "out", NULL), result, status);
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "TEXTURE", "texture", NULL), result, status);
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "FRAG_COORD", "gl_FragCoord", NULL), result, status);

        snprintf(tempBuffer, sizeof(tempBuffer) - 1, "out mediump vec4 FRAG_COLOR;\n");
        SIMPLEGL_REQUIRE_SUCCESS(add_shader_concat(&shader_entry_list, tempBuffer), result, status);
    }
    else
    {
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "UNIFORM", "uniform", NULL), result, status);
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "INPUT", "attribute", "varying"), result, status);
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "OUTPUT", "varying", NULL), result, status);
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "TEXTURE", "texture2D", NULL), result, status);
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "FRAG_COORD", "gl_FragCoord", NULL), result, status);
        SIMPLEGL_REQUIRE_SUCCESS(add_compiler_define(info, &shader_entry_list, "FRAG_COLOR", "gl_FragColor", NULL), result, status);
    }

    simplegl_shader_temp_header_t *temp_header = info->temp;

    for (int i = 0; i < temp_header->entries; i++)
    {
        simplegl_shader_temp_entry_t *entry = &temp_header->temp[i];

        switch (entry->type)
        {
        case SIMPLEGL_SHADER_TEMP_TYPE_TEXT:
            SIMPLEGL_REQUIRE_SUCCESS(add_shader_concat(&shader_entry_list, entry->text), result, status);
            break;

        case SIMPLEGL_SHADER_TEMP_TYPE_INSERT_CODE:
            SIMPLEGL_REQUIRE_SUCCESS(add_shader_concat(&shader_entry_list, code), result, status);
            break;

        default:
            SIMPLEGL_ASSERT("unknown entry->type" && false);
            break;
        }
    }

    char *out_code = simplegl_get_code_text(&shader_entry_list);
    if (!out_code)
    {
        status = SIMPLEGL_STATUS_ALLOCATION_ERROR;
        goto result;
    }

    *out = out_code;
result:
    simplegl_free_list(&shader_entry_list);

    return status;
}
