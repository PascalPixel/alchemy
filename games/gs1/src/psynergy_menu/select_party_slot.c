#include "types.h"

#define PsynergyMenu_SelectPartySlot Func_080a602c

struct Rec5 { u8 pad[5]; unsigned int flag : 8; };
struct Cur { unsigned short mark : 8; };

extern void *Data_03001f2c;

void *Func_08077008(s32);
s32 Func_080a1ac0(s32, s32);
s32 Func_080a68ec(void *, void *, s32);
s32 Func_080a60d4(void *, void *);
void Func_080a17c4(void *cursor);
void WaitFrames(s32);

s32 PsynergyMenu_SelectPartySlot(s32 party_slot)
{
    void *menu = Data_03001f2c;
    s32 offset = party_slot + 28;
    s32 cursor_offset = party_slot * 4 + 20;
    void *icon;
    u8 byte_val;
    s32 owner_index;
    s32 combined_offset;
    s32 obj_off;
    s32 obj_id;
    void *obj_ptr;
    void *p456;
    u8 *p2;
    s32 badge;
    s32 result;
    s32 cursor_offset2;

    result = 0;
    icon = *(void **)(menu + cursor_offset);
    *(u8 *)(icon + 5) = 1;
    *(u16 *)(icon + 12) = result;
    ((struct Rec5 *)(*(u8 **)(menu + 540)))->flag = 13;
    p2 = (u8 *)menu + 2;
    owner_index = *(s8 *)(menu + offset);
    byte_val = ((struct Cur *)(menu + 537))->mark;
    p2[offset] = byte_val;

    if (owner_index == -1) {
        *(u8 *)(menu + offset) = 0;
        combined_offset = 0;
    } else {
        combined_offset = owner_index * 2;
        Func_080a1ac0(owner_index * 24 - 10, 16);
    }

    obj_off = combined_offset + 520;
    obj_id = *(u16 *)(menu + obj_off);
    obj_ptr = Func_08077008(obj_id);
    p456 = menu + 456;
    badge = Func_080a68ec(obj_ptr, p456, 2);
    *(u8 *)(menu + 536) = (u8)badge;
    result = Func_080a60d4(menu + 520, p456);

    cursor_offset2 = party_slot * 4 + 20;
    icon = *(void **)(menu + cursor_offset2);
    Func_080a17c4(icon);
    WaitFrames(1);
    return result;
}
