#include "types.h"
#include "gs1_edition.h"

extern u8 *Data_03001e8c;

void Func_080ab21c(s32 x, s32 y, s32 width, s32 height, s32 alt)
{
    u8 *base = Data_03001e8c;
    u32 flag = alt << 12;

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
        s32 offset = y * 64 + x * 2;

        do {
            u16 *p = (u16 *)(base + offset);
            s32 col = width;

            if (col != 0) {
                do {
                    u32 tile = *p;
                    if (((tile >> 12) & 0xf) == 15) {
                        tile &= 0xffff0fff;
                        tile |= flag;
                        *p = tile;
                    }
                    col--;
                    p++;
                } while (col != 0);
            }

            base[RENDER_DIRTY_OFS] |= 2 << ((u32)y >> 2);
            height--;
            offset += 64;
            y++;
        } while (height != 0);
    }
}
