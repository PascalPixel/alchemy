#include "types.h"

void Func_08022768(s32 x, s32 y, s32 width, s32 height, u32 field)
{
    u8 *base = *(u8 **)0x03001e8c;

    field &= 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        u8 *dirty = base + 0xea3;
        s32 offset = (y << 6) + (x << 1);
        u32 dirty_bit = 2;

        do {
            u16 *pixel = (u16 *)((u32)offset + (u32)base);
            s32 remaining = width;

            while (remaining != 0) {
                u32 value = *pixel;

                value &= 0xffffefff;
                value |= field;
                remaining--;
                *pixel = value;
                pixel++;
            }
            *dirty = (dirty_bit << ((u32)y >> 2)) | *dirty;
            height--;
            offset += 64;
            y++;
        } while (height != 0);
    }
}
