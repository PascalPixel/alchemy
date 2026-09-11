#include "scene.h"
#include "abi/item_menu/prep_owner.h"
#include "item_menu.h"

s32 Runtime_GetObject(s32);

void WaitFrames(s32);

s32 ItemMenu_PrepOwner(s32 party_slot)
{
    register void *menu = gIw;
    register s32 offset = party_slot + 28;
    s32 result = 0;
    register s32 owner_index;
    u8 *owner_slots;

    owner_index = *(s8 *)(menu + offset);
    Sys_Do(*(s32 *)(menu + 44));
    owner_slots = menu + 2;
    owner_slots[offset] = *(u8 *)(menu + 0x219);
    if (owner_index == -1) {
        *(u8 *)(menu + offset) = result;
        offset = 0;
    } else {
        offset = owner_index * 2;
        Sys_Apply(owner_index * 24 - 10, 16);
    }

    {
        s32 off = offset + 0x208;
        result = Runtime_GetObject(*(u16 *)(menu + off));
    }
    offset = (s32)(menu + 0x1C8);
    *(s8 *)(menu + 0x218) =
        (s8)ItemMenu_Collect((void *)result, (u16 *)offset, 0);
    result = Sys_Apply2(menu + 0x208, (void *)offset);
    {
        s32 cursor_offset = party_slot * 4 + 20;
        Sys_Do2(*(s32 *)(menu + cursor_offset));
    }
    WaitFrames(1);
    return result;
}
