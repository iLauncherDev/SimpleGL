#pragma once
#include "default.h"

typedef enum
{
    SIMPLEGL_SHADER_TEMP_TYPE_TEXT,
    SIMPLEGL_SHADER_TEMP_TYPE_INSERT_CODE,
} simplegl_shader_temp_type_t;

typedef struct simplegl_shader_temp_entry
{
    simplegl_shader_temp_type_t type;
    char *text;
} simplegl_shader_temp_entry_t;

typedef struct simplegl_shader_temp_header
{
    simplegl_shader_temp_entry_t *temp;
    int entries;
} simplegl_shader_temp_header_t;

typedef struct simplegl_shader_entry
{
    char *text;

    simplegl_list_entry_t list_entry;
} simplegl_shader_entry_t;

typedef struct simplegl_compiler_info
{
    char *header;

    bool is_opengl_3;
    bool is_fragment;
    simplegl_shader_temp_header_t *temp;
} simplegl_compiler_info_t;

simplegl_status_t SIMPLEGL_API simplegl_compile_shader(simplegl_compiler_info_t *info, char *code, char **out);
