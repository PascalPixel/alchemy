/* 2026-10-01 (wave 1, slice 1): what the reference's loops say, for the
   next attempt (this body still scores best, 2055). (1) Nothing is hoisted
   out of the three-layer loop: the cell's << 3 stays in the drawing block
   because its result goes into a variable that is also set outside the
   block (the one that held ((y & 15) << 5) + (x & 15), r3), and loop.c
   does not move such a set past a conditional branch. Spelling that reuse
   (n = cell * 8 in the block) keeps the shift there. (2) For the same
   reason only y & 15 and the 15 leave the x loop: the << 5 is assigned to
   that variable too (n = (y & 15) << 5; n += x & 15;). (3) The 0x800 step
   and the block's four bases are built beside their adds, as twice-set
   pointers or a ++ on a 0x800-byte type give. (4) The layer loop keeps its
   test at the bottom only while its break lies more than 30 RTL insns
   from the loop's start (stmt.c's exit-test scan); shorter spellings of
   the block get rotated around the break. (5) y is the dst_y parameter
   itself (r6) and its limit a stack slot. Plain C with (1)-(3) reaches the
   reference's block and frame but not yet (4) and (5) together.
   2026-09-29 alchemy permute: score 2602 to 2055 on the permuter's scorer
   (0 is exact); remaining 28 register-only, 11 operand, 17 reordered, 3
   inserted, 3 deleted. Kept rewrites: 7x reorder independent statements,
   4x reorder local declarations, 3x add a same-width cast, 3x change loop
   form, 2x swap commutative operands, 2x drop a same-width cast, 1x
   introduce a temporary, 1x remove a temporary, 1x move an assignment into
   or out of a condition, 1x toggle register. FAKEMATCH: the permuter's
   temporaries, register hints and swapped operand orders below only steer
   allocation and scheduling; no programmer would write them, so they stay
   tagged until a natural spelling replaces them. */
/* Not-yet-C: complete 304-byte owner. Width/height are arguments three/four;
 * destination x/y are five/six. Copies complete cells, then draws low 12 bits.
 * Reusing the neighbouring renderer's scoped CopyCell recovers the 36-byte
 * frame: 288/304 bytes, 137 halfwords / 69 aligned edits (baseline 296/90).
 * Remaining: width spills instead of fp, dst stays in r7 instead of sp+4,
 * row shift/cell index/0x800 are hoisted outside the reference's blocks.
 * Moving the visibility loop into another helper gave 292/84, not retained.
 * Stop this helper axis; inspect loop-invariant decisions before new trials. */
#include "MAP_SCROLL.H"
struct TilePos { s32 x, y; };

static __inline__ void CopyCell(u32 cell, u32 offset)
{
    u32 *tiles;
    u32 *dest;
    u32 index = cell * 2;

    tiles = (u32 *)0x02020000;
    dest = (u32 *)(0x06002800 + offset);
    tiles += index;
    *dest = *tiles;
    tiles = (u32 *)0x02020004;
    tiles += index;
    dest = (u32 *)(0x06002840 + offset);
    *dest = *tiles;
}

void Map_CopyMetatileCellsRect(s32 src_x, s32 src_y,
    s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    u32 *src = (u32 *)0x02010000 + ((u32)src_y * 128 + src_x);
    u32 *dst = (u32 *)0x02010000 + (dst_y * 128 + dst_x);
    struct TilePos tile[3];
    register struct MapLayerScroll *layer = gCam->layers;
    struct TilePos *pos = tile;
    s32 i;
    s32 y;
    s32 x;

    for (i = 2; i >= 0; i--) {
        pos->x = layer->x >> 20;
        pos->y = layer->y >> 20;
        layer++;
        pos++;
    }
    y = dst_y;
    if (y < height + dst_y) {
        while (1) {
            if ((x = dst_x) < dst_x + width) {
                do {
                    u32 offset;
                    u32 cell = *src++;
                    pos = tile;
                    *dst++ = cell;
                    offset = 4 * (((y & 15) << 5) + (x & 15));
                    cell &= 0xfff;
                    for (i = 0; i < 3; i++) {
                        if (pos->x <= x && pos->x + 16 > x && pos->y <= y && pos->y + 12 > y) {
                            CopyCell(cell, offset);
                            break;
                        }
                        offset += 0x800;
                        pos++;
                    }
                    x++;
                } while (x < dst_x + width);
            }
            src += 128 - width;
            dst += 128 - width;
            y++;
            if (y >= height + dst_y)
                break;
        }
    }
}
