#include "types.h"

extern void *Data_03001f2c;

void Func_08015270(s32);
s32 Func_080770c0(s32);
void Func_08015070(s32 window, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 Func_080a1ac0(s32, s32);
s32 Func_080a7d68(void);
s32 Func_080a7a34(void);
void Func_080a17c4(void *cursor);
void WaitFrames(s32);

s32 Func_080a77a4(s32 party_slot)
{
    s32 offset = party_slot + 28;
    s32 cursor_offset = party_slot * 4 + 20;
    void *menu = Data_03001f2c;
    void *icon = *(void **)(menu + cursor_offset);
    s32 owner_index;
    s32 result;

    *(u8 *)(icon + 5) = 1;
    *(u16 *)(icon + 12) = 0;

    owner_index = *(s8 *)(menu + offset);
    Func_08015270(*(s32 *)(menu + 16));

    if (Func_080770c0(370) != 0) {
        Func_08015070(*(s32 *)(menu + 16), 9, 1, 9, 3);
    }

    if (owner_index == -1) {
        *(u8 *)(menu + offset) = 0;
    } else {
        Func_080a1ac0(owner_index * 24 - 10, 16);
    }

    if (*(u16 *)(menu + 544) == 3) {
        result = Func_080a7d68();
    } else {
        result = Func_080a7a34();
    }

    icon = *(void **)(menu + cursor_offset);
    Func_080a17c4(icon);
    WaitFrames(1);
    return result;
}
