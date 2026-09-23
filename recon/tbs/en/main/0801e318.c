/* Draft, not exact (2026-09-24): candidate=176 reference=176 differing_halfwords=81. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
extern u8 Value_00000ea2;
extern u8 Value_000003ff;
extern u8 Value_000001ff;
extern u8 Value_0000027f;
extern u8 Value_00000da0;

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
                u32 tile = *cursor++ & (s32)&Value_000003ff;

                if ((tile - 0x80) <= 0x7f ||
                    (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                    u32 index = ((tile & 0xff) ^ 0x80) + (s32)&Value_00000da0;

                    base[index] |= 2;
                }
                column--;
            } while (column != 0);
        }
        row--;
    } while (row != 0);

    {
        s32 index;
        u8 *attributes = base + (s32)&Value_00000da0;

        for (index = 255; index >= 0; index--, attributes++) {
            if (*attributes == 1)
                *attributes = 0;
        }
    }
}
