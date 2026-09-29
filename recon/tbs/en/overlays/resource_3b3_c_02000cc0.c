/* NONMATCHING: resource_3b3 at 0x02008cc0, TakaraHashira_CopyCellBlock,
 * 184/184 bytes with its pool (2026-09-29). Placed as
 * FIELD/TAKARA_HASHIRA/COPY_CELL_BLOCK.C in place of the listing section
 * .text.x02008cc0, it scores 290 under alchemy permute: 9 register-only and
 * 4 reordered instructions, frame 8 and every pool word agree.
 *
 * Remaining difference, all in the inner loop's preheader: the reference
 * hoists 0xfff (r8), 15 (r7), the row base (sp+0), 0x06002800 (lr) and only
 * then copies the column bound into ip, keeping 0x02020004 in r6 and
 * 0x06002840 in r1. Here the bound is copied into ip first and 0xfff into r8
 * last, which swaps r1/r2 and r2/r6 in the body. A named bound variable
 * (680) and a plain nested for loop (1065) are worse; three permute runs of
 * 400-580 s from this draft found nothing below 290.
 *
 * The chains of copied pointers are permute output that keeps the two
 * 0x02020000/0x02020004 loads from being CSEd into one base register; the
 * plain spelling hoists 0x02020000 and spills two more words. Tag them
 * FAKEMATCH (forced temporaries) if this is ever adopted. */
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* Draws a width by height block of map cells, starting at cell (x, y), into
 * the background plane's screen blocks at (destX, destY), each cell's tile
 * entries read from its metatile. */
void TakaraHashira_CopyCellBlock(s32 x, s32 y, s32 width, s32 height, s32 plane, s32 destX, s32 destY)
{
    u32 *cell = (u32 *)Ram_MapCellBuffer + (y * 128 + x);
    s32 row;
    s32 col;

    row = destY;
    if (row < destY + height) {
        do {
            col = destX;
            while ((u32)(col < width + destX) != 0) {
                u32 offset = (0xfff & *cell++) * 8;
                s32 index = (plane * 16 + (row & 15)) * 32 + (col & 15);
                u32 *tmp;
                u32 *tmp3;
                u32 *tmp2;
                u32 *tmp4;
                u32 *tmp5;
                tmp3 = (u32 *)0x06002800 + index;
                tmp = tmp3;
                tmp5 = (u32 *)(Ram_MapBlocks + offset);
                tmp4 = tmp5;
                tmp3 = tmp4;
                tmp2 = tmp3;
                *tmp = *tmp2;
                col++;
                tmp5 = (u32 *)0x06002840;
                tmp2 = tmp5;
                tmp = tmp2;
                tmp4 = (u32 *)(offset + (Ram_MapBlocks + 4));
                tmp[index] = *tmp4;
            }
            cell += 128 - width;
            (u32)row++;
        } while (row < destY + height);
    }
}
