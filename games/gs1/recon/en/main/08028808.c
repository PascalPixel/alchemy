#include "types.h"

extern u8 Data_03001f38;

s32 Func_080022ec(s32, s32);
s32 Func_080162d4(s32, s32, s32, s32, s32);

void Func_08028808(s32 p1, s32 p2, s32 p3)
{
    u8 *state = *(u8 **)&Data_03001f38;
    s32 count;
    s32 width;
    s32 x;
    s32 i;

    *(u16 *)(state + 144) = (u16)(p2 + 2);
    *(u16 *)(state + 146) = (u16)p3;
    *(u16 *)(state + 148) = (u16)p1;

    count = *(s16 *)(state + 142);
    width = Func_080022ec(*(s16 *)(state + 144) * 2, 3);
    x = 15 - (count * 3 + width) / 2;

    for (i = 0; i < count; i++) {
        *(u16 *)(state + i * 20 + 12) = (u16)(x << 3);
        *(u16 *)(state + i * 20 + 14) = (u16)(p1 << 3);
        x += 3;
    }

    *(s32 *)(state + 120) =
        Func_080162d4(x, p1, *(s16 *)(state + 144), 3, 2);
}
