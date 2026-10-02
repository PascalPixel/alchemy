/* Draft of KorimaMagari_DrawPanel, resource_394 at 0x02008098, for
 * FIELD/KORIMA_MAGARI (called from SCENE_SEQUENCE.C).
 * Remaining difference: 9 register-only and 4 reordered instructions in
 * this form with permute's temporaries (1441 in the plain form). The game
 * keeps the mask 15 in r7 and reloads both block table addresses from the
 * pool for every cell; this swaps r8 and ip for the 0xfff mask and the
 * column end and loads the second table address into r2. Tried: the row
 * inside and outside the inner loop, byte offsets and word indexes, and
 * four minutes of permute. 2026-10-01 (matcher 3): five more minutes (3
 * jobs, seed 100, 47,647 candidates) found nothing below 290, and the
 * plain one-statement cell body scores worse (it keeps 0x06002800 out of
 * lr and hoists the table address into r7).
 * 2026-10-02: fresh linked baseline 290. Binding the row mask 15 to r7
 * scored 1280 (44 differing instructions); binding the cell mask 0xfff
 * to r8 with an empty input/output constraint scored 2110 (50 differing
 * instructions). Both trials were discarded. */
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* Draw a width by height panel of map cells from (src_x, src_y) into
   background screen block `screen` at (dest_x, dest_y): each cell's block
   gives two words of tile entries, one for each of its tile rows. */
void KorimaMagari_DrawPanel(s32 src_x, s32 src_y, s32 width, s32 height, s32 screen, s32 dest_x,
                            s32 dest_y)
{
    u32 *cell;
    s32 x;
    s32 y;
    s32 row;
    u32 block;
    s32 at;

    cell = (u32 *)(Ram_MapCellBuffer + ((src_y << 7) + src_x) * 4);
    for (y = dest_y; y < dest_y + height; y++) {
        for (x = dest_x; x < dest_x + width; x++) {
            s32 tmp;
            s32 tmp2;
            s32 tmp3;
            u32 *tmp4;
            tmp3 = ((y & 15) + (screen << 4)) << 5;
            tmp2 = tmp3;
            block = (*cell++ & 0xfff) * 8;
            tmp = tmp2;
            row = tmp;
            tmp = x & 15;
            tmp2 = row + tmp;
            at = tmp2;
            ((u32 *)0x06002800)[at] = *(u32 *)(Ram_MapBlocks + block);
            tmp4 = (u32 *)(Ram_MapBlocks + 4 + block);
            *((u32 *)0x06002840 + at) = *tmp4;
        }
        cell += 128 - width;
    }
}
