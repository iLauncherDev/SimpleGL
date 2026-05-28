#pragma once
#include "../default.h"

#define simplegl_init_list_entry(entry, struct_pointer) \
    _simplegl_init_list_entry(entry, (uintptr_t)struct_pointer - (uintptr_t)entry)

typedef struct simplegl_list_entry
{
    intptr_t struct_offset;
    void *prev;
    void *next;
} simplegl_list_entry_t;

void SIMPLEGL_API _simplegl_init_list_entry(simplegl_list_entry_t *entry, intptr_t struct_offset);
pvoid SIMPLEGL_API simplegl_get_list_entry_structure(simplegl_list_entry_t *entry);
void SIMPLEGL_API simplegl_insert_tail_list(simplegl_list_entry_t *head, simplegl_list_entry_t *list_to_insert);
void SIMPLEGL_API simplegl_insert_head_list(simplegl_list_entry_t *head, simplegl_list_entry_t *list_to_insert);
void SIMPLEGL_API simplegl_remove_entry_list(simplegl_list_entry_t *entry);
