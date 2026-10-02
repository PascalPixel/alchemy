/* NONMATCHING: resource_3b3 .text.x02008cc0, TakaraHashira_CopyCellBlock,
 * 184/184 bytes with its pool; its twin is resource_394 .text.x02008098
 * (KorimaMagari_DrawPanel). Destined for FIELD/TAKARA_HASHIRA as its own
 * module in place of the listing section.
 * 2026-10-02 (slice 13): rewritten without the temporary chains; 13 of 86
 * instructions differ (the chained draft had 19). What the loop pass dumps
 * showed, and what this body now reproduces:
 * - the inner loop must hold 32 instructions in the first loop pass and 27
 *   in the second, so that only 0xfff and 0x06002800 are hoisted and the
 *   three later constants stay inside. A 16-bit offset gives exactly that;
 * - src and dst are each assigned twice, which keeps them out of the local
 *   allocator, so the second source address is not tied to the offset and
 *   the second pair of statements comes out exactly as the game's.
 * Remaining difference: the game hoists the column bound's copy in the
 * second loop pass, after the two constants, which needs one instruction
 * between that copy and the loop's compare. Written as a plain
 * `for (col = destX; col < destX + width; col++)` the copy has a lifetime
 * of one and stays in the loop (21 differ); the do/while below gets the
 * lifetime of two, but its compare is then `cmp ip, r0; bgt` where the game
 * has `cmp r0, ip; blt`, and the two hoisted constant loads are scheduled
 * in the other order. Wanted: a loop form that evaluates the bound before
 * the increment and still compares col against it. */
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* Draws a width by height block of map cells, starting at cell (x, y), into
 * the background plane's screen blocks at (destX, destY): each cell's two
 * rows of tile entries come from its map block. */
void TakaraHashira_CopyCellBlock(s32 x, s32 y, s32 width, s32 height, s32 plane, s32 destX, s32 destY)
{
    u32 *cell = (u32 *)Ram_MapCellBuffer + (y * 128 + x);
    s32 row;
    s32 col;

    for (row = destY; row < destY + height; row++) {
        col = destX;
        if (col < destX + width) do {
            u16 offset = (0xfff & *cell++) * 8;
            s32 index = (plane * 16 + (row & 15)) * 32 + (col & 15);
            u32 *src;
            u32 *dst;

            dst = (u32 *)0x06002800 + index;
            src = (u32 *)(Ram_MapBlocks + offset);
            *dst = *src;
            src = (u32 *)(Ram_MapBlocks + 4 + offset);
            dst = (u32 *)0x06002840 + index;
            *dst = *src;
        } while (destX + width > ++col);
        cell += 128 - width;
    }
}
