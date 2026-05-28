#pragma once
#include "../default.h"

typedef struct simplegl_utf8_string
{
    uint8_t *buffer;
    size_t valid_size;
    size_t max_size;
} simplegl_utf8_string_t;

size_t SIMPLEGL_API simplegl_strnlen(const char *str, size_t n);
uint8_t SIMPLEGL_API simplegl_safe_string_fetch(simplegl_utf8_string_t *string, size_t index);
int SIMPLEGL_API simplegl_strcasecmp(char *s1, char *s2);

void SIMPLEGL_API simplegl_vprintf_ex(char *file, int line, char *fmt, va_list args);
void SIMPLEGL_API simplegl_printf_ex(char *file, int line, char *fmt, ...);

#define simplegl_vprintf(fmt, args) simplegl_vprintf_ex(__FILE__, __LINE__, fmt, args)
#define simplegl_printf(fmt, ...) simplegl_printf_ex(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
