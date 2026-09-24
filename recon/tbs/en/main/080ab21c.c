/* Draft, not exact (2026-09-24): candidate=208 reference=208 differing_halfwords=3.
   UI: set palette `palette` on every tile of a clipped rectangle of the
   32x32 tile map that uses palette 15, and mark the touched rows dirty.
   Open: one scheduling tie at the row tail. sched2 issues the reload of
   the spilled row offset (priority 4) before height-- (priority 2); the
   reference issues height-- first. Row statement orders, pointer
   spellings and an explicit offset all keep the tie (-fsched-verbose=5). */
#include "TYPES.H"
#include "TBS_EDITION.H"

extern u8 *Data_03001e8c;

void Func_080ab21c(s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    u8 *base;
    u16 *p;
    s32 col;
    u32 tile;

    base = Data_03001e8c;
    palette <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        do {
            p = &((u16 (*)[32])base)[y][x];
            for (col = width; col != 0; col--) {
                tile = *p;
                if (((tile >> 12) & 15) == 15)
                    *p = (tile & 0xffff0fff) | palette;
                p++;
            }
            base[RENDER_DIRTY_OFS] |= 2 << ((u32)y >> 2);
            height--;
            y++;
        } while (height != 0);
    }
}
