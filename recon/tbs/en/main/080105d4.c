/* 2026-10-04 current560: initialize the consumed row before forming
 * source/destination pointers, and use it in the destination origin.
 * Same signed bounds and cursor/row updates; no new device or state.
 * This removes the extra row copy, leaving only register/operand/order
 * differences. Full-cell stores and both drawing words remain intact.
 */
/* 2026-10-04 native live-register trials after real early-row correction:
 * row6: draft: score 1894 (27 register-only, 9 stack-only, 6 operand, 10 reordered, 3 inserted, 7 deleted)
 * counter5: draft: score 995 (23 register-only, 2 operand, 7 reordered, 4 inserted)
 * row_counter: draft: score 2282 (30 register-only, 7 stack-only, 4 operand, 15 reordered, 6 inserted, 5 deleted)
 * masked7: draft: score 2275 (49 register-only, 5 stack-only, 8 operand, 12 reordered, 4 inserted, 7 deleted)
 * row_masked: draft: score 2629 (37 register-only, 4 stack-only, 6 operand, 13 reordered, 3 inserted, 12 deleted)
 * row_counter_masked: draft: score 2918 (38 register-only, 3 stack-only, 5 operand, 13 reordered, 7 inserted, 11 deleted)
 * Retain early-row560 pending any new-device review.
 */
/* Ordinary search seed7105/two jobs/10200 candidates retained560;
 * no new source changes. Native instructions all present; differences are
 *27 registers,2 operands and6 reorderings, with row still inr7 versusr6.
 */
/* 2026-10-04 endpoint trials from560:
 * end_before_test: draft: score 1368 (28 register-only, 8 stack-only, 2 operand, 9 reordered, 2 inserted, 4 deleted)
 * end_after_test: draft: score 1073 (29 register-only, 8 stack-only, 2 operand, 9 reordered, 1 inserted, 2 deleted)
 * Retain early-row560; field names unchanged.
 */
/* 2026-10-04 row ownership/lifetime trials from675:
 * before_src: draft: score 560 (27 register-only, 2 operand, 6 reordered)
 * before_dst: draft: score 1178 (30 register-only, 8 stack-only, 2 operand, 9 reordered, 2 inserted, 2 deleted)
 * after_dst: draft: score 1198 (30 register-only, 8 stack-only, 3 operand, 9 reordered, 2 inserted, 2 deleted)
 * end_before_test: malformed field-name rename; no score.
 * end_after_test: malformed field-name rename; no score.
 * Retain before_src; any endpoint model needs semantic review.
 */
/* 2026-10-04 canonical MapCell lifetime trials:
 * canonical_word_union: draft: score 675 (26 register-only, 3 operand, 6 reordered, 1 inserted)
 * typed_cell_cursors: draft: score 675 (26 register-only, 3 operand, 6 reordered, 1 inserted)
 * consumed_cell_value: draft: score 875 (18 register-only, 3 operand, 5 reordered, 2 inserted, 2 deleted)
 * Retain prior675; no new shared declaration or unused storage.
 */
/* 2026-10-04 current draft: score675 (26 register,3 operand,6 scheduling,
 * 1 inserted instruction). The consumed drawing byte-offset lifetime in r3
 * prevents hoisting of its shift; full-cell stores, signed bounds, all three
 * layers and both tile-word loads/stores remain intact. Still uncredited.
 * Neighbor120 source-pointer scheduling alone800; with in-place cell
 * conversion1385, or later x-offset handoff1380: none retained.
 * Ordinary separate byte-offset local770; native bytes/r3 binding710;
 * bytes/r3 plus masked index/r7=2225. Keep only the consumed bytes/r3 binding.
 * Seed4105/two jobs/11270 ordinary candidates then improve710 to675.
 * Subsequent counter/r5=1100; row/r6=1849; row+counter2367; row+index/r7=2714;
 * row+counter+index2794. All rejected; no unread state or new frame storage.
 * Previous checkpoint headers below preserve the earlier measurements.
 */
/* 2026-10-04 follow-up: canonical body remains770.
 * New shared index-copy structure835; separate full-cell/index835; native
 * index/r7 binding1742. Isolated neighbor drawing block835; its layer setup,
 * explicit signed stride, index-before-base and earlier dst advance each770.
 * Early row initialization790; cached end in height1458; native row/r6=1609;
 * row/r6 plus index/r7=2654. A mixed-declaration trial was invalid C89;
 * valid separated row/cursor initialization gives3800.
 * Reuse copied word in row scalar770, in n1745; mask through row scalar770;
 * defer mask until visible915; share drawing-word multiplier770.
 * Height-first bound sum770; bottom-break row loop1836; single for-loop770.
 * Reviewed declaration-printer repair then rebuilt; seed2105/two jobs/
 * 17958 ordinary candidates found no gain. Native width/r11 binding1900;
 * mutating-row+width1923; early-row+width2003; row/r6+width/r11=2546.
 * Four direct full-copy/index expressions (combined cursor advances,
 * separate dst advance, masked destination readback, later assignment)
 * each770. No new register binding or source body is retained.
 */
/* 2026-10-04: current canonical draft, score770 (41 register, 4 operand,
 * 6 scheduling, 1 inserted instruction); complete304B/frame36. Width stays
 * in fp; source/destination cursors at sp8/sp4; row stride stays in r8.
 * Remaining: row r7 instead of r6 with an extra initial copy, hoisted cell
 * scaling, and low-register/drawing-base scheduling. No C credit claimed.
 * The adjacent index-copy structure, adapted to signed dimensions and a
 * whole-cell store before masking for drawing, improves2055 to1120.
 * Seed105/two jobs/6707 candidates reaches770; seed805/8766 makes no gain.
 * Removed unused tmp9 and ineffectual register on address-taken tile array;
 * same770. Source review preserves all three layers, complete cell values,
 * both tile-word stores, signed clipping and128-width row advances.
 *
 * At1120: mutate height/dst_y1799; separate row endpoint1639.
 * At770: cache end in height1378; new end local1403; separate y init770.
 * Live register pins after ordinary failures: y/r6=1639, i/r5=1260,
 * y+i=2224, x/r1=2495, y+x=3988, y+i+x=2819; none retained.
 * In-place cell*=2 and cell<<=1 each775 (28 register,4 operand,7 scheduling,
 * 1 inserted); byte offsets in both tile-word loads1615. Seed1805/two jobs/
 * 8490 candidates improves in-place form to770 (32 register,3 operand,
 * 7 scheduling,1 inserted); tie leaves simpler prior body retained.
 * Older headers below preserve earlier attempts, superseded by this model.
 */
/* 2026-10-02 bounded structural wave: restored the stable score-2055
 * body after three failed trials. Current output is the complete 304-byte
 * extent with a 36-byte frame and its first 84 bytes exact. Width is r11,
 * src/dst are sp+8/sp+4; older allocation warnings below are historical.
 * Remaining: row moves r6 to r7, row shift/cell scale/0x800 are hoisted,
 * and row stride is rebuilt at row end instead of retained in r8.
 * Trial 1: end_y = dst_y + height, then mutate dst_y, helper unchanged:
 * 2715 (37 register, 5 stack, 8 operand, 12 reordered, 7 inserted,
 * 9 deleted); frame40, end_y in r11, width spills and prefix regresses.
 * Trial 2: height += dst_y, then mutate dst_y, helper unchanged:
 * 2875 (36 register, 5 stack, 8 operand, 13 reordered, 7 inserted,
 * 10 deleted); same frame40 and row limit r11. Parameter axis closed.
 * Trial 3: original row loop; shared n for row sum, cell*8 and 0x800;
 * byte-pointer drawing and explicit visibility do loop:
 * 2191 (44 register, 6 stack, 6 operand, 15 reordered, 3 inserted,
 * 6 deleted). Cell scaling and 0x800 stay inside their reference blocks,
 * but frame32/prefix/spills regress and row still moves to r7.
 * All three trials rejected; no unused storage or instructions added.
 * The next blocker is retaining the reference row/stride lifetimes while
 * suppressing those drawing invariants without altering the frame. */
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
#include "RAM_BUFFER.H"
struct TilePos { s32 x, y; };

void Map_CopyMetatileCellsRect(s32 src_x, s32 src_y,
    s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    /* FAKEMATCH: after ordinary helper/lifetime forms remained at2055,
       temporary sharing, casts and statement order steer allocation/
       scheduling; the current measured result is560 with no new state
       or source side effects. */
    s32 y = dst_y;
    u32 *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
    u32 *dst = (u32 *)Ram_MapCellBuffer + (y * 128 + dst_x);
    struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
    struct TilePos tile[3];
    s32 i;
    struct TilePos *pos = tile;
    register s32 x;

    i = 2;
    while (i >= 0) {
        register s32 tmp5;
        tmp5 = layer->x >> 20;
        pos->x = tmp5;
        tmp5 = (*layer).y;
        pos->y = tmp5 >> 20;
        pos++;
        layer++;
        i--;
    }
    if (y < dst_y + height) {
        do {
            for (x = dst_x; x < dst_x + width; x++) {
                u32 cell = *src++;
                u32 offset;
                u32 *dest;
                u32 tmp3;
                u32 n;
                register u32 *tiles;
                u32 tmp6;
                u32 tmp12;
                u32 tmp13;
                u32 tmp11;
                dst[0] = cell;
                cell &= 0xfff;
                tmp12 = x & 15;
                tmp6 = tmp12;
                tmp3 = (u32)(y & 15);
                tmp13 = tmp3 << 5;
                dst++;
                tmp12 = tmp13;
                n = tmp12;
                pos = tile;
                n += tmp6;
                tmp13 = n * 4;
                tmp11 = tmp13;
                i = 0;
                tmp6 = tmp11;
                offset = tmp6;
                while (i < 3) {
                    if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y && pos->y + 12 > (s32)y) {
                        u32 *tmp;
                        u32 tmp2;
                        u32 tmp4;
                        u32 *tmp7;
                        u32 tmp10;
                        u32 tmp14;
                        u32 tmp16;
                        /* FAKEMATCH: keep the consumed drawing byte offset in native r3 after ordinary cell/offset lifetime trials failed to retain its native scheduling. */
                        register u32 bytes __asm__ ("r3") = cell * 8;
                        u32 tmp8;
                        u32 *tmp9;
                        u32 tmp17;
                        tmp = (u32 *)Ram_MapBlocks;
                        tmp8 = 0x06002800 + offset;
                        tiles = tmp;
                        tmp2 = tmp8;
                        tmp = (u32 *)tmp2;
                        dest = tmp;
                        tiles = (u32 *)((u8 *)tiles + bytes);
                        tmp16 = *tiles;
                        tmp14 = tmp16;
                        tmp10 = tmp14;
                        *dest = tmp10;
                        tmp7 = (u32 *)(Ram_MapBlocks + 4);
                        tiles = tmp7;
                        tmp17 = 0x06002840 + offset;
                        tmp8 = tmp17;
                        tmp14 = tmp8;
                        tiles = (u32 *)((u8 *)tiles + bytes);
                        tmp17 = *tiles;
                        tmp16 = tmp17;
                        tmp10 = tmp16;
                        tmp4 = tmp10;
                        tmp2 = tmp4;
                        tmp4 = tmp14;
                        tmp9 = (u32 *)tmp4;
                        tmp7 = tmp9;
                        dest = tmp7;
                        (s32)(*dest = tmp2);
                        break;
                    }
                    n = 0x800;
                    offset += n;
                    pos++;
                    i += 1;
                }
            }
            src += 128 - width;
            dst = 128 - width + dst;
            y++;
        } while (y < (s32)dst_y + height);
    }
}
