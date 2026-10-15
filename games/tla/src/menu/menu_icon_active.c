#include "types.h"
#include "ram_buffer.h"

struct MenuIconWork {
    u8 unknown_000[0xc12];
    s8 active;
};

void MenuIcon_SetActive(s32 active)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menu_runtime;

    /* FAKEMATCH: ⚓️ loads the menu work before it builds the field offset */
    __asm__ volatile("" : "+l"(menu));
    menu->active = active;
}
