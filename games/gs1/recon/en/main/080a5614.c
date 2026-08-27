#include "types.h"

extern void *Data_03001f2c;
extern u8 Value_00000075;

void Func_08015270(s32);
void Func_080030f8(s32);
void Func_08015080(s32, s32, s32, s32);
s32 Func_080a2268(s32, s32, s32, s32, s32, s32);

s32 Func_080a5614(s32 arg0, s32 arg1, void *state)
{
    void *menu;
    s32 page;
    s32 entry_count;
    s32 combined;
    s32 flag_val;
    s32 row;
    s32 y;
    s32 param5;

    page = *(s32 *)(state + 8);
    menu = Data_03001f2c;
    combined = page * 5;
    entry_count = *(s32 *)(state + 16);
    combined += entry_count;
    *(s32 *)(state + 24) = combined;

    Func_08015270(*(s32 *)(menu + 44));
    Func_080030f8(1);

    combined = *(s32 *)(state + 24);
    flag_val = *(u16 *)(menu + combined * 2 + 456);
    if (flag_val != 0) {
        s32 masked = (flag_val & 0x1ff) + (s32)&Value_00000075;
        Func_08015080(masked, *(s32 *)(menu + 44), 0, 0);
    }

    row = 0;
    y = 1;
    param5 = 1;
    do {
        if (row == entry_count) {
            Func_080a2268(*(s32 *)(menu + 32), 1, y, 14, param5, 14);
        } else {
            Func_080a2268(*(s32 *)(menu + 32), 1, y, 14, param5, 15);
        }
        row++;
        y += 2;
    } while (row <= 4);

    Func_080030f8(1);
    return 1;
}
