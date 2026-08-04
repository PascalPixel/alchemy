#include "types.h"

extern u8 *Data_03001e8c;

void Func_08022768(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    s32 x = arg0;
    s32 y = arg1;
    s32 width = arg2;
    s32 height = arg3;
    u8 *state = Data_03001e8c;
    u32 tileValue = (arg4 & 1) << 12;

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        u8 *rowFlags = state + 0xea3;
        s32 rowOffset = (y << 6) + (x * 2);

        do {
            s32 remaining = width;
            u16 *tile = (u16 *)(state + rowOffset);

            if (remaining != 0) {
                do {
                    *tile = (*tile & 0xffffefff) | tileValue;
                    remaining--;
                    tile++;
                } while (remaining != 0);
            }
            *rowFlags |= 2 << ((u32)y >> 2);
            height--;
            rowOffset += 0x40;
            y++;
        } while (height != 0);
    }
}
