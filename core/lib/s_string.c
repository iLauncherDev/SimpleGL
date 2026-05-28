#include <lib/s_string.h>

size_t SIMPLEGL_API simplegl_strnlen(const char *str, size_t n)
{
    size_t i = 0;
    while (i < n && str[i] != '\0')
        i++;

    return i;
}

uint8_t SIMPLEGL_API simplegl_safe_string_fetch(simplegl_utf8_string_t *string, size_t index)
{
    if (index >= string->valid_size)
        return '\0';

    return string->buffer[index];
}

int SIMPLEGL_API simplegl_strcasecmp(char *s1, char *s2)
{
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    int result;

    if (p1 == p2)
        return 0;

    while ((result = tolower(*p1) - tolower(*p2)) == 0)
    {
        if (*p1 == '\0')
            break;
        p1++;
        p2++;
    }

    return result;
}

bool isBreakingLine = true;

void SIMPLEGL_API simplegl_vprintf_ex(char *file, int line, char *fmt, va_list args)
{
    if (isBreakingLine)
    {
        isBreakingLine = false;
        printf("(%s:%d) ", file, line);
    }

    if (strchr(fmt, '\n'))
    {
        isBreakingLine = true;
    }

    vprintf(fmt, args);
}

void SIMPLEGL_API simplegl_printf_ex(char *file, int line, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    simplegl_vprintf_ex(file, line, fmt, args);

    va_end(args);
}
