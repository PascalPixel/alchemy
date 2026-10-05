/* Draft: Menu_CenterResourceEntries, demoted from MENU7.C when ⚓️ moved to -mtune=arm9tdmi
   (Pascal, 2026-10-05). Under the new tuning the row store and the count address tie at priority 43 in sched2 and the independent movs wins, 3 instructions from the listing; barriers, volatile stores, register pins and 2000 hill-climb steps did not fix it. */
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "RESMENU.H"
#include "RAM_BUFFER.H"

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resource_base)
{
    struct ResourceMenuWork *menu = Ram_HeapSlots->menu_select_work;
    s32 x;
    s32 i;
    s32 count;

    menu->width = width + 2;
    menu->resource_base = resource_base;
    menu->row = row;
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
