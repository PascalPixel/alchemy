/* NONMATCHING: 160 of 176 bytes, 85 differing halfwords, 41 halfword edits.
 * Reusing the row counter for the cleanup scan restores its lifetime.
 * WALL: the reference retains a width guard and separate pool constants;
 * literal for-loops and an inline traversal did not reproduce that shape.
 */
#include "TYPES.H"

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
                u32 tile = *cursor++ & 0x000003ff;

                if ((tile - 0x80) <= 0x7f ||
                    (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                    u32 index = ((tile & 0xff) ^ 0x80) + 0x00000da0;

                    base[index] |= 2;
                }
                column--;
            } while (column != 0);
        }
        row--;
    } while (row != 0);

    {
        u8 *attributes = base + 0x00000da0;

        for (row = 255; row >= 0; row--, attributes++) {
            if (*attributes == 1)
                *attributes = 0;
        }
    }
}
