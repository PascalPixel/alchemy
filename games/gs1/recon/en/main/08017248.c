#include "types.h"

extern u8 *Data_03001e8c;

void Func_08017248(s32 x, s32 y, s32 width, s32 height, u32 mode)
{
    u8 *base = Data_03001e8c;
    u16 *cursor = (u16 *)(base + ((y << 5) + x) * 2);
    s32 row;

    if (width <= 1 || height <= 1 || width > 30 || height > 30)
        goto done;

    cursor += 32;

    if (mode == 0) {
        for (row = 1; row < height - 1; row++) {
            s32 col;
            u16 *rowStart;

            cursor += 2;
            rowStart = cursor;
            for (col = 1; col < width - 1; col++) {
                *rowStart = (u16)(((0xF000 + row) & 0xFFF) | 0x127);
                rowStart += (height - 2);
            }
            cursor += (32 - width) * 2 + 2;
        }
    } else {
        for (row = 1; row < height - 1; row++) {
            s32 col;
            u16 *colCursor = cursor;

            for (col = 0; col < width; col++) {
                *colCursor = (u16)(((0xF000 + row) & 0xFFF) | 0x127);
                colCursor += 1;
            }
            cursor += 32;
        }
    }

    base[0xea3] = 1;

done:
    return;
}
