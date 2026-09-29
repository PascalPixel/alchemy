/* NONMATCHING: 160 of 176 bytes, 85 differing halfwords, 41 halfword edits.
 * Reusing the row counter for the cleanup scan restores its lifetime.
 * WALL: the reference retains a width guard and separate pool constants;
 * literal for-loops and an inline traversal did not reproduce that shape.
 * 2026-09-29: alchemy permute took the score from 1325 to 945 (9
 * register-only, 4 operand, 5 reordered, 4 inserted, 1 deleted): the guard
 * survives once the width test passes through two flags (tagged), the row
 * count drops before the scan and the scan loop exits at its bottom. So
 * the reference's width is not a constant GCSE could fold; the flags only
 * stand in for whatever kept it a variable. */
#include "TYPES.H"

extern u8 *Data_03001e8c;

/* Mark attributes used by the visible 30 by 20 tilemap and clear stale marks. */
void UiWindow_MarkVisibleTileAttributes(void)
{
    u8 *base = Data_03001e8c;
    u16 *cursor = (u16 *)base;
    s32 row = 20;
    u32 alternate = base[0xea2];
    s32 width = 30;

    do {
        s32 wide;
        s32 scan;

        /* FAKEMATCH: the width test goes through two flags so the guard is
           not folded away; the reference keeps it although width is 30. */
        wide = 0 != width;
        scan = wide != 0;
        row--;
        if (scan) {
            s32 column = width;

            while (1) {
                u32 tile = *cursor++ & 0x000003ff;

                if ((tile - 0x80) <= 0x7f ||
                    (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                    u32 index = ((tile & 0xff) ^ 0x80) + 0x00000da0;

                    base[index] |= 2;
                }
                column--;
                if (column == 0)
                    break;
            }
        }
    } while (row != 0);

    {
        register u8 *attributes = base + 0x00000da0;

        for (row = 255; row >= 0; row--, attributes++) {
            if (*attributes == 1)
                *attributes = 0;
        }
    }
}
