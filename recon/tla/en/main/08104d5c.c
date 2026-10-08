#include "types.h"
#include "ram_buffer.h"

struct MenuPageWork {
    u8 unknown_000[0x128];
    u16 colours[4];
};

void Menu_SetPageArrowsA(s32 index, s32 count);

void Menu_HighlightPageEntry(s32 unused, s32 index, s32 count)
{
    struct MenuPageWork *menu = Ram_HeapSlots->menu_runtime;
    s32 top = index / 4 * 4;
    s32 i;

    Menu_SetPageArrowsA(index, count);
    for (i = 0; i <= 3; i++) {
        if (top + i == index)
            menu->colours[i] = 30;
        else
            menu->colours[i] = 26;
    }
}
