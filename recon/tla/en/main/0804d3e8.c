#include "mem.h"
#include "types.h"
#include "resource.h"
#include "resmenu.h"
#include "ram_buffer.h"

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resource_base)
{
    struct ResourceMenuWork *menu = Ram_HeapSlots->menu_select_work;
    s32 x;
    s32 i;
    s32 count;

    menu->width = width + 2;
    menu->resource_base = resource_base;
    menu->row = row;
    /* FAKEMATCH: ⚓️ stores the row before it addresses the count */
    asm("" : "+r"(menu) : "m"(menu->row));
    count = menu->count;
    x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
    for (i = 0; i < menu->count; i++) {
        struct ResourceMenuEntry *entry = &menu->entries[i];

        entry->x = x * 8;
        entry->y = row * 8;
        x += 3;
    }
    menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}
