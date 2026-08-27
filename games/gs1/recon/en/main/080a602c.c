#include "types.h"

extern void *Data_03001f2c;

void *Func_08077008(s32);
s32 Func_080a1ac0(s32, s32);
s32 Func_080a68ec(void *, void *, s32);
s32 Func_080a60d4(void *, void *);
void Func_080a17c4(void *cursor);
void WaitFrames(s32);

s32 Func_080a602c(s32 party_slot)
{
    s32 cursor_offset = party_slot * 4 + 20;
    void *menu = Data_03001f2c;
    void *icon = *(void **)(menu + cursor_offset);
    u8 byte_val;
    s32 offset;
    s32 owner_index;
    s32 combined_offset;
    s32 obj_id;
    void *obj_ptr;
    void *p456;
    s32 badge;
    s32 result;

    *(u8 *)(icon + 5) = 1;
    *(u16 *)(icon + 12) = 0;

    (*(u8 **)(menu + 540))[5] = 13;
    byte_val = *(u8 *)(menu + 537);

    offset = party_slot + 28;
    owner_index = *(s8 *)(menu + offset);
    ((u8 *)(menu + 2))[offset] = byte_val;

    if (owner_index == -1) {
        *(u8 *)(menu + offset) = 0;
        combined_offset = 0;
    } else {
        combined_offset = owner_index * 2;
        Func_080a1ac0(owner_index * 24 - 10, 16);
    }

    obj_id = *(u16 *)(menu + combined_offset + 520);
    obj_ptr = Func_08077008(obj_id);
    p456 = menu + 456;
    badge = Func_080a68ec(obj_ptr, p456, 2);
    *(u8 *)(menu + 536) = (u8)badge;
    result = Func_080a60d4(menu + 520, p456);

    icon = *(void **)(menu + cursor_offset);
    Func_080a17c4(icon);
    WaitFrames(1);
    return result;
}
