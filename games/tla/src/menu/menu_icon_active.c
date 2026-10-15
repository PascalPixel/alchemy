#include "types.h"
#include "ram_buffer.h"

struct MenuIconWork
{
    u8 unk000[0xc12];
    s8 active;
};

void MenuIcon_SetActive(s32 active)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;

    /* FAKEMATCH: ⚓️ loads the menu work before it builds the field offset */
    __asm__ volatile("" : "+l"(menu));
    menu->active = active;
}
