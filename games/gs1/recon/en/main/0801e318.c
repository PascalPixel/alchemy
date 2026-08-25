#include "types.h"

extern u8 *Data_03001e8c;

#define UiWindow_MarkVisibleTileAttributes Func_0801e318

/* Mark attributes used by the visible 30 by 20 tilemap and clear stale marks. */
void UiWindow_MarkVisibleTileAttributes(void)
{
    u8 *base = Data_03001e8c;
    u16 *cursor = (u16 *)base;
    u32 alternate = base[0xea2];
    s32 row = 20;
    s32 width = 30;

    do {
        if (width != 0) {
            s32 column = width;

            do {
                u32 tile = *cursor++ & 0x3ff;

                if ((tile - 0x80) <= 0x7f ||
                    (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                    u32 index = ((tile & 0xff) ^ 0x80) + 0xda0;

                    base[index] |= 2;
                }
                column--;
            } while (column != 0);
        }
        row--;
    } while (row != 0);

    {
        s32 index;
        u8 *attributes = base + 0xda0;

        for (index = 255; index >= 0; index--, attributes++) {
            if (*attributes == 1)
                *attributes = 0;
        }
    }
}
