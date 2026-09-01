#include "types.h"
#include "owner_state.h"

#define InventoryMenu_CollectItems Func_080a3ddc

s32 InventoryMenu_CollectItems(struct OwnerInventoryState *owner, u16 *items)
{
    u16 *src;
    u16 *dst;
    s32 count;
    s32 i;
    s32 offset;

    for (i = 0; i < 32; i++) {
        items[i] = 0;
    }
    count = 0;
    dst = items;
    src = owner->inventory;
    offset = 0;
    for (i = 0; i < 15; i++) {
        *(u16 *)(offset + (u32)items) = 0;
        offset += 2;
        if (*src != 0) {
            *dst++ = *src;
            count++;
        }
        src++;
    }
    return count;
}
