#include "types.h"
#include "ram_buffer.h"

struct MenuSlotEntry {
    u8 mode;
    u8 unknown_01;
    s16 x;
    u8 unknown_04[8];
};

struct MenuIconWork {
    u8 unknown_000[0xbd0];
    struct MenuSlotEntry slots[5];
};

void MenuIcon_SetSlot(s32 index, u32 x, s32 mode)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menu_runtime;
    struct MenuSlotEntry *slot;

    /* FAKEMATCH: ⚓️ loads the menu work before it scales the slot index */
    __asm__ volatile("" : "+l"(menu));
    slot = (struct MenuSlotEntry *)((u8 *)menu + (index * (s32)sizeof(struct MenuSlotEntry) + 0xbd0));
    slot->mode = mode;
    slot->x = (slot->x & ~0x1ff) | (x & 0x1ff);
}
