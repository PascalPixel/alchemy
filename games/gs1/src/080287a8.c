#include "types.h"
#include "global_cells.h"

extern s32 Resource_FindFreeSlot(void);
extern void Func_0802875c(s32 arg0, s32 index);

void Menu_AppendResourceEntry(s32 arg0)
{
    u8 *base;
    u8 *entry;
    s16 index;
    s32 slot;
    s32 off;
    s32 flags;

    base = *(u8 **)ADDR_03001F38;
    index = *(s16 *)(base + 142);
    if (index <= 5)
    {
        *(u16 *)(base + 142) = *(u16 *)(base + 142) + 1;
        entry = base + index * 20;
        slot = Resource_FindFreeSlot();
        Func_0802875c(slot, arg0);
        *(u16 *)(entry + 12) = index * 24 + 32;
        flags = 136;
        *(u16 *)(entry + 14) = flags;
        off = index + 132;
        *(u16 *)(entry + 18) = slot;
        base[off] = (u8) arg0;
    }
}
