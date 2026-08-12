#include "types.h"

extern u8 *Data_03001e8c;

void Func_08022768(s32 x, s32 y, s32 width, s32 height, u32 palette)
{
    u8 *base;
    u8 *status;
    u32 palette_bits;
    u32 status_bit;
    s32 clipped_width;
    s32 row;

    base = Data_03001e8c;
    clipped_width = width;
    palette_bits = palette;
    palette_bits &= 1;
    palette_bits <<= 12;

    if (x < 0) {
        clipped_width += x;
        x = 0;
    }
    if (x + clipped_width > 29)
        clipped_width = 30 - x;

    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (clipped_width > 0 && height > 0) {
        status = base + 0xea3;
        status_bit = 2;
        row = (y << 6) + (x << 1);

        do {
            u16 *entry = (u16 *)(base + row);
            s32 remaining = clipped_width;

            if (remaining != 0) {
                u32 clear_mask = 0xffffefff;

                do {
                    u32 value = *entry;

                    value &= clear_mask;
                    value |= palette_bits;
                    *entry = value;
                    remaining--;
                    entry++;
                } while (remaining != 0);
            }

            *status |= status_bit << ((u32)y >> 2);
            height--;
            row += 64;
            y++;
        } while (height != 0);
    }
}
