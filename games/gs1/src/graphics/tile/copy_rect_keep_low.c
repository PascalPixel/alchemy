#include "types.h"

void Func_08010704(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX, s32 dstY)
{
    u8 *src = (u8 *)0x02010000;
    u8 *dst = src + (((dstY << 7) + dstX) << 2);
    u16 row;

    src += ((srcY << 7) + srcX) << 2;

    if (height > 0) {
        row = 0;
        do {
            u32 *srcRow;
            u32 *dstRow;
            u16 col;

            dstRow = (u32 *)(dst + (row << 9));
            srcRow = (u32 *)(src + (row << 9));

            if (width > 0) {
                col = 0;
                do {
                    u32 value = (*dstRow & 0xfff) | (*srcRow++ & 0xfffff000);
                    *dstRow++ = value;
                    col++;
                } while (col < width);
            }

            row++;
        } while (row < height);
    }
}
