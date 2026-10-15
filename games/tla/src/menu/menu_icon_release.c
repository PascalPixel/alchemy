#include "types.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"

struct MenuIconWork {
    u8 unknown_000[0xc08];
    u16 resources[5];
};

void MenuIcon_PushSlots(void);
s32 Resource_ResetEntry(u32 resource);

void MenuIcon_ReleaseSlots(void)
{
    struct MenuIconWork *menu = Ram_HeapSlots->menu_runtime;
    u16 *resource;
    s32 i;

    Scheduler_RemoveCallback(MenuIcon_PushSlots);
    resource = menu->resources;
    for (i = 4; i >= 0; i--)
        Resource_ResetEntry(*resource++);
}

struct MenuSlotAttr {
    u8 mode;
    u8 unknown_01;
    u16 x : 9;
    u8 unknown_04[8];
};

void MenuIcon_SetSlot(s32 index, u32 x, s32 mode)
{
    u8 *menu = Ram_HeapSlots->menu_runtime;
    /* FAKEMATCH: the offset as its own value adds the row and the field offset before the work pointer, as in Func_0810be70 (item_icon_2.c). */
    s32 offset = index * sizeof(struct MenuSlotAttr) + 0xbd0; /* slots[index] + 4 */
    struct MenuSlotAttr *slot = (struct MenuSlotAttr *)(menu + offset);

    slot->x = x;
    slot->mode = mode;
}
