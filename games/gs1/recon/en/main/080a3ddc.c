#include "types.h"
#include "owner_state.h"

#define InventoryMenu_CollectItems Func_080a3ddc

s32 InventoryMenu_CollectItems(
    struct OwnerInventoryState *owner,
    u16 *items,
    s32 unused)
{
    u16 *base;
    u16 *dst;
    u16 *src;
    s32 count;
    s32 offset;
    s32 i;
    u32 item;

    base = items;
    dst = base + 31;
    do {
        *dst-- = 0;
    } while ((s32)dst >= (s32)base);

    count = 0;
    dst = base;
    src = owner->inventory;
    offset = 0;
    i = 14;
    do {
        *(u16 *)((u8 *)base + offset) = 0;
        item = *src++;
        if (item != 0) {
            *dst++ = item;
            count++;
        }
        offset += 2;
        i--;
    } while (i >= 0);
    return count;
}
