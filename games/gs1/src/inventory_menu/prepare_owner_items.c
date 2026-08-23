#include "inventory_menu.h"

void Func_08015278(s32);
void Func_080a1ac0(s32, s32);
s32 Runtime_GetObject(s32);
s32 Func_080a35f8(void *, void *);
void Func_080a17c4(void *icon);
void WaitFrames(s32);

s32 InventoryMenu_PrepareOwnerItems(s32 party_slot)
{
    register void *menu = Data_03001f2c;
    register s32 offset = party_slot + 28;
    s32 result = 0;
    register s32 owner_index;
    u8 *owner_slots;

    owner_index = *(s8 *)(menu + offset);
    Func_08015278(*(s32 *)(menu + 44));
    owner_slots = menu + 2;
    owner_slots[offset] = *(u8 *)(menu + 0x219);
    if (owner_index == -1) {
        *(u8 *)(menu + offset) = result;
        offset = 0;
    } else {
        offset = owner_index * 2;
        Func_080a1ac0(owner_index * 24 - 10, 16);
    }

    {
        s32 owner_id_offset = offset + 0x208;
        result = Runtime_GetObject(*(u16 *)(menu + owner_id_offset));
    }
    offset = (s32)(menu + 0x1C8);
    *(s8 *)(menu + 0x218) =
        (s8)InventoryMenu_CollectItems((void *)result, (u16 *)offset, 0);
    result = Func_080a35f8(menu + 0x208, (void *)offset);
    {
        s32 cursor_offset = party_slot * 4 + 20;
        Func_080a17c4(*(s32 *)(menu + cursor_offset));
    }
    WaitFrames(1);
    return result;
}
