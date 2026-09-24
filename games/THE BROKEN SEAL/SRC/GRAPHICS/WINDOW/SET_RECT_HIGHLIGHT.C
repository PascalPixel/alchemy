#include "TYPES.H"
#include "TBS_EDITION.H"

extern u8 *Data_03001e8c;

/* Sets (alt bit 0 = 1) or clears palette bit 12 on every tile of a
   width x height rectangle of the 32 x 32 window tilemap, clipped to the
   30 x 20 screen, and marks each touched group of four rows dirty. */
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 alt)
{
    u8 *base = Data_03001e8c;

    alt &= 1;
    alt <<= 12;

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
        do {
            u16 *p = &((u16 (*)[32])base)[y][x];
            s32 col = width;

            if (col != 0) {
                do {
                    u32 tile = *p;
                    tile = (tile & ~0x1000) | alt;
                    col--;
                    *p = tile;
                    p++;
                } while (col != 0);
            }

            /* FAKEMATCH: the do-while (0) keeps the row counter update after
               the dirty-byte store instead of filling the load delay. */
            do {
                base[RENDER_DIRTY_OFS] |= 2 << ((u32)y >> 2);
            } while (0);
            height--;
            y++;
        } while (height != 0);
    }
}
