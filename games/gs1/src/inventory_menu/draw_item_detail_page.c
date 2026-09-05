#include "types.h"

extern void *Data_03001f2c;
extern u8 Value_00000075;

void Func_08015270(s32);
void Func_080030f8(s32);
void Func_08015080(s32, s32, s32, s32);
s32 Func_080a2268(s32, s32, s32, s32, s32, s32);

s32 InventoryMenu_DrawItemDetailPage(s32 arg0, s32 arg1, void *state)
{
    void *menu;
    s32 page;
    s32 combined;
    s32 off;
    s32 row;

    page = *(s32 *)(state + 8);
    menu = Data_03001f2c;
    combined = page * 5;
    combined += *(s32 *)(state + 16);
    *(s32 *)(state + 24) = combined;

    Func_08015270(*(s32 *)(menu + 44));
    Func_080030f8(1);

    combined = *(s32 *)(state + 24);
    off = combined * 2 + 456;
    if (*(u16 *)((char *)menu + off) != 0) {
        s32 masked = (*(u16 *)((char *)menu + off) & 0x1ff) + (s32)&Value_00000075;
        Func_08015080(masked, *(s32 *)(menu + 44), 0, 0);
    }

    row = 0;
    do {
        if (row == *(s32 *)(state + 16)) {
            Func_080a2268(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 14);
        } else {
            Func_080a2268(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    Func_080030f8(1);
    return 1;
}
