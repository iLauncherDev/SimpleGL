#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "api.h"
#include "type.h"
#include "lib/status.h"
#include "lib/list.h"
#include "lib/s_string.h"

#define GLSL(src) #src

#define FUNC_IS_ON_STRING(name, vector)                               \
    bool name(char *string)                                           \
    {                                                                 \
        for (size_t i = 0; i < sizeof(vector) / sizeof(*vector); i++) \
        {                                                             \
            if (!strcmp(vector[i], string))                           \
                return true;                                          \
        }                                                             \
        return false;                                                 \
    }

#define SIMPLEGL_ASSERT(cond)                                      \
    if (!(cond))                                                   \
    {                                                              \
        fprintf(stderr, "ASSERT FAILED: %s\nFile: %s\nLine: %d\n", \
                #cond, __FILE__, __LINE__);                        \
        abort();                                                   \
    }

#define SIMPLEGL_REQUIRE(condition, label) \
    if (!condition)                        \
    {                                      \
        goto label;                        \
    }

#define string_includes(str, chr) ((strchr(str, chr)) != NULL)
#define memclr(p, s) memset(p, 0, s)

#define CASE_TO_STRING(name) \
    case name:               \
        return #name

simplegl_status_t SIMPLEGL_API simplegl_init();
void SIMPLEGL_API simplegl_uninit();
