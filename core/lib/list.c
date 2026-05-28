#include <lib/list.h>

void SIMPLEGL_API _simplegl_init_list_entry(simplegl_list_entry_t *entry, intptr_t struct_offset)
{
    SIMPLEGL_ASSERT(entry != NULL);

    memclr(entry, sizeof(*entry));

    entry->struct_offset = struct_offset;
    entry->prev = entry;
    entry->next = entry;
}

pvoid SIMPLEGL_API simplegl_get_list_entry_structure(simplegl_list_entry_t *entry)
{
    SIMPLEGL_ASSERT(entry != NULL);
    SIMPLEGL_ASSERT(entry->prev != NULL);
    SIMPLEGL_ASSERT(entry->next != NULL);

    return (char *)entry + entry->struct_offset;
}

void SIMPLEGL_API simplegl_insert_tail_list(simplegl_list_entry_t *head, simplegl_list_entry_t *list_to_insert)
{
    SIMPLEGL_ASSERT(list_to_insert != NULL);
    SIMPLEGL_ASSERT(list_to_insert->next != NULL);
    SIMPLEGL_ASSERT(list_to_insert->prev != NULL);

    SIMPLEGL_ASSERT(head != NULL);
    SIMPLEGL_ASSERT(head->next != NULL);
    SIMPLEGL_ASSERT(head->prev != NULL);

    list_to_insert->next = head;
    list_to_insert->prev = head->prev;

    simplegl_list_entry_t *list_to_insert_prev = list_to_insert->prev;

    head->prev = list_to_insert;
    list_to_insert_prev->next = list_to_insert;
}

void SIMPLEGL_API simplegl_insert_head_list(simplegl_list_entry_t *head, simplegl_list_entry_t *list_to_insert)
{
    SIMPLEGL_ASSERT(list_to_insert != NULL);
    SIMPLEGL_ASSERT(list_to_insert->next != NULL);
    SIMPLEGL_ASSERT(list_to_insert->prev != NULL);

    SIMPLEGL_ASSERT(head != NULL);
    SIMPLEGL_ASSERT(head->next != NULL);
    SIMPLEGL_ASSERT(head->prev != NULL);

    list_to_insert->next = head->next;
    list_to_insert->prev = head;

    simplegl_list_entry_t *list_to_insert_next = list_to_insert->next;

    head->next = list_to_insert;
    list_to_insert_next->prev = list_to_insert;
}

void SIMPLEGL_API simplegl_remove_entry_list(simplegl_list_entry_t *entry)
{
    simplegl_list_entry_t *prev;
    simplegl_list_entry_t *next;

    SIMPLEGL_ASSERT(entry != NULL);

    prev = entry->prev;
    next = entry->next;

    SIMPLEGL_ASSERT(prev != NULL);
    SIMPLEGL_ASSERT(next != NULL);

    if (prev && next)
    {
        prev->next = next;
        next->prev = prev;
    }

    entry->prev = entry;
    entry->next = entry;
}
