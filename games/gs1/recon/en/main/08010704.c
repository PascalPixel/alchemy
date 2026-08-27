#include "types.h"

void Func_08010704(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX, s32 dstY)
{
    u32 base = 0x02010000;
    u8 *dst = (u8 *)(base + (((dstY << 7) + dstX) << 2));
    u8 *src = (u8 *)(base + (((srcY << 7) + srcX) << 2));
    s32 row;

    if (height > 0) {
        row = 0;
        do {
            u32 *dstRow = (u32 *)(dst + (row << 9));
            u32 *srcRow = (u32 *)(src + (row << 9));
            s32 col;

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
