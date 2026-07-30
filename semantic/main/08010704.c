#include "types.h"

void Func_08010704(
    s32 source_x,
    s32 source_y,
    s32 width,
    s32 height,
    s32 destination_x,
    s32 destination_y)
{
    s32 height_copy = height;
    s32 width_copy = width;
    u8 *base = (u8 *)0x02010000;
    u8 *destination_base = base
        + (((destination_y << 7) + destination_x) << 2);
    u8 *source_base = base
        + (((source_y << 7) + source_x) << 2);
    u32 row;

    if (height_copy > 0) {
        row = 0;
        do {
            u32 offset = row >> 7;
            u32 *destination = (u32 *)(destination_base + offset);
            u32 *source = (u32 *)(source_base + offset);

            if (width_copy > 0) {
                u32 column;
                column = 0;
                do {
                    u32 destination_value = *destination;
                    u32 source_value = *source++;

                    *destination++ = (destination_value & 0xFFF)
                        | (source_value & 0xFFFFF000);
                    {
                        u32 next_column = column + 0x10000;
                        column = next_column;
                    }
                } while ((s32)(column >> 16) < width_copy);
            }
            {
                u32 next_row = row + 0x10000;
                row = next_row;
            }
        } while ((s32)(row >> 16) < height_copy);
    }
}
