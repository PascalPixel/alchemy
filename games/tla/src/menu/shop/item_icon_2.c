#include "item_icon.h"

void Func_0810be3c(void)
{
    struct IconWork *work = Ram_HeapSlots->menu_runtime;
    s32 i;

    Scheduler_RemoveCallback(Func_0810bdb0);
    for (i = 0; i < ICON_COUNT; i++) {
        if (work->resources[i] != ICON_NONE)
            Resource_ResetEntry(work->resources[i]);
    }
}

void Func_0810be70(s32 index, s32 x, s32 y)
{
    struct IconWork *work = Ram_HeapSlots->menu_runtime;
    /* FAKEMATCH: the offset as its own value adds the row and the field offset before the work pointer, as the native does (three reordered rows). */
    s32 offset = index * sizeof(struct IconSprite) + 0xc18;
    struct IconAttr *attr = (struct IconAttr *)((u8 *)work + offset);

    attr->x = x;
    attr->y = y;
}

void Func_0810bea8(s32 active)
{
    struct IconWork *work = Ram_HeapSlots->menu_runtime;

    /* FAKEMATCH: the native loads the menu work before it builds the field offset (one reordered row). */
    __asm__ volatile("" : "+l"(work));
    work->active = active;
}
