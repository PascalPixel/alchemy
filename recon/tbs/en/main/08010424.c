/* 2026-10-04 focused candidate40: replaces only the first existing
 * destination-address addition with low-register add operands. No new
 * arithmetic, storage or table value; flags explicitly clobbered. All
 * instructions/registers match except reversed drawing/table literal loads.
 * Independent review replaced only this asm with ordinary C and recovered
 *70, without instruction insertion/deletion. Consumed u32 addition and
 *low-register/cc constraints are valid S2 register/order steering.
 *Approved compiler/era assembler confirm full316B/frame36. Uncredited.
 */
/* 2026-10-04 paired consumed-base input order trials:
 * screen_first_table_first: rejected high-register add operands before scoring.
 * screen_first_table_last: rejected high-register add operands before scoring.
 * table_first_table_first: draft: score 1773 (41 register-only, 3 stack-only, 3 operand, 11 reordered, 5 inserted, 3 deleted)
 * table_first_table_last: draft: score 1773 (41 register-only, 3 stack-only, 3 operand, 11 reordered, 5 inserted, 3 deleted)
 * table_inside_base_scope: rejected high-register add operands before scoring.
 * Retain unbound-base40 pending independent S2 review.
 */
/* 2026-10-04 existing destination-result operand trials:
 * r2_output_r: draft: score 40 (2 operand)
 * r2_output_l: draft: score 40 (2 operand)
 * plain_output_r: draft: score 40 (2 operand)
 * plain_output_l: draft: score 40 (2 operand)
 * Retain r2_output_r; new device is not admitted without independent review.
 */
/* 2026-10-04 existing address-addition register trials:
 * address_add: draft: score 855 (48 register-only, 2 operand, 9 reordered)
 * address_add_table_lifetime: draft: score 215 (6 register-only, 1 operand, 1 reordered, 1 deleted)
 * address_add_table_input: draft: score 255 (6 register-only, 3 operand, 1 reordered, 1 deleted)
 * Retain prior70; no new device until independent S2 review.
 */
/* 2026-10-04 consumed drawing-base allocation trials:
 * plain_base_reuse: draft: score 70 (2 register-only, 1 reordered)
 * base_r2: draft: score 975 (46 register-only, 2 operand, 5 reordered, 2 inserted, 2 deleted) (C89 nested scope).
 * base_r2_clobber5: draft: score 755 (46 register-only, 1 operand, 5 reordered, 1 inserted, 1 deleted) (C89 nested scope).
 * base_any_clobber5: draft: score 1883 (37 register-only, 3 stack-only, 5 operand, 11 reordered, 6 inserted, 3 deleted) (C89 nested scope).
 * base_r2_input: draft: score 565 (45 register-only, 4 operand, 1 reordered, 2 deleted) (C89 nested scope).
 * Retain prior70; no new device unless separately reviewed.
 */
/* Typed canonical MapCell/word union: draft: score 70 (2 register-only, 1 reordered).
 * Retain prior70; no additional header/type definition adopted.
 */
/* 2026-10-04 bounded-value/typed-cell trials:
 * cell_s32: draft: score 70 (2 register-only, 1 reordered)
 * cell_u16: draft: score 2033 (41 register-only, 3 stack-only, 3 operand, 12 reordered, 7 inserted, 3 deleted)
 * cell_s16: draft: score 2033 (41 register-only, 3 stack-only, 3 operand, 12 reordered, 7 inserted, 3 deleted)
 * bytes_s32: draft: score 70 (2 register-only, 1 reordered)
 * bytes_u16: draft: score 475 (3 register-only, 1 reordered, 4 inserted)
 * bytes_s16: draft: score 475 (3 register-only, 1 reordered, 4 inserted)
 * typed_cells: rejected duplicate existing MapCell type before scoring.
 * Retain prior70; no new steering device.
 */
/* 2026-10-04 ordinary follow-up seed6104/two jobs/10795 candidates:
 * no improvement from70; same two register choices and one load reorder.
 */
/* 2026-10-04 first drawing/source base lifetime trials:
 * plain: draft: score 1613 (40 register-only, 3 stack-only, 2 operand, 12 reordered, 4 inserted, 2 deleted)
 * map_pin: draft: score 70 (2 register-only, 1 reordered)
 * map_barrier: draft: score 315 (14 register-only, 1 operand, 2 reordered, 1 deleted)
 * pair_pin: draft: score 70 (2 register-only, 1 reordered)
 * pair_barrier: draft: score 590 (45 register-only, 2 operand, 2 reordered, 2 deleted)
 * Retain prior70; any new device requires instruction-presence review.
 */
/* 2026-10-04 independent follow-up: retained70 (two register choices
 * and one table-base load reorder); no new binding or barrier retained.
 * Plain first-base scalar1673; native r2 scalar/pointer bindings both70,
 * folded back to r5. Volatile r2 form76 adds an unread frame word and is
 * rejected. An empty read/write base barrier975 also regresses.
 * Existing destination/r0 binding195; tiles/r2=70; both195.
 * Reusing destination temp for first tile-word load520; both loads175;
 * table base through that temp70; offset-first destination sum70.
 * Focused ordinary search seed5104/two jobs/16998 candidates: no gain.
 * Native offset/r4 binding500; row/r7=2560; layer counter/r6=650.
 * The first drawing base must still naturally occupy r2 beside the table
 * base in r5; no reserved storage or extra operations may force it.
 */
/* 2026-10-04: byte-offset lifetime in r3 plus an ordinary shared
 * destination temporary improves120 to70 (2 register,1 reordered).
 * Seed3004 searched34039 candidates; full316B/frame36 is preserved with
 * no inserted/deleted instructions. Remaining difference is the first
 * drawing base allocation/load order. Both tile stores and row updates
 * were reviewed against the current listing; still uncredited. */
/* alchemy permute: Map_CopyMetatileIndicesRect against recon/tbs/raw/08010424.s: score 70 (2 register-only, 1 reordered).
   Job 0, iteration 522; rewrites: 1x share one temporary between two statements. */
/* 2026-10-04: fixed declarator printing enables another ordinary search;
 * 32342 candidates/seed2004 improve155 to120 by moving the second source
 * pointer addition before the independent destination calculation.
 * Retained full316B: 11 register differences and one reordered shift. */
/* 2026-10-04: a cell-only r5 binding after the ordinary rewrites improves
 * score210 to155: 10 register,2 literal-load operand,1 reordered differences.
 * No instruction insertion/deletion, frame or extent changes. Moving the
 * cell-index assignment before the drawing bases gives the same155.
 * The retained tagged binding is only allocation steering; the remaining
 * mismatch is the drawing block and row-limit reload. Still not credited. */
/* 2026-10-04: second ordinary rewrite wave (seed1004,29583 candidates)
 * improves495 to210 (21 register,2 operand,1 reordered; no size/frame
 * or inserted/deleted differences). Reviewed all stores and cursor updates.
 * A hard r1 column binding instead scored2700 and was rejected. */
/* alchemy permute: Map_CopyMetatileIndicesRect against recon/tbs/raw/08010424.s: score 210 (21 register-only, 2 operand, 1 reordered).
   Job 3, iteration 6794; rewrites: 6x reorder independent statements, 5x swap commutative operands, 4x introduce a temporary, 4x remove a temporary, 3x share one temporary between two statements, 3x add a same-width cast, 3x change loop form, 3x toggle register, 1x reorder local declarations, 1x drop a same-width cast, 1x split or join a compound assignment, 1x move an assignment into or out of a condition or call. */
/* 2026-10-03: shared MapScrollWork/RAM_BUFFER definitions preserve the
 * old 2683 baseline. Explicit byte-pointer/base lifetime trial repeats
 * score 3459/frame28 and is rejected. Four ordinary-rewrite jobs, seed104,
 * 30572 candidates: retained score495, 35 register/5 scheduling differences,
 * no stack, operand, inserted or deleted instructions. Full 316-byte owner.
 * Assignment/cursor review confirms three layers, low-12-bit copy, both
 * tile-word stores and complete source/destination row advances survive. */
/* alchemy permute: Map_CopyMetatileIndicesRect against recon/tbs/raw/08010424.s: score 495 (35 register-only, 5 reordered).
   Job 0, iteration 8400; rewrites: 11x share one temporary between two statements, 10x reorder independent statements, 10x introduce a temporary, 8x add a same-width cast, 6x reorder local declarations, 6x remove a temporary, 6x drop a same-width cast, 5x change loop form, 5x split or join a compound assignment, 4x swap commutative operands, 4x toggle register, 3x pointer arithmetic or indexing, 2x test truth or compare with zero, 1x move an assignment into or out of a condition or call. */
/* 2026-10-02 register trial 1: pinning every low/high lifetime lowers
 * register differences to 26 but changes both three-layer loops and source
 * arithmetic; score3716 (1 stack,13 operand,26 reordered,6 inserted,11
 * deleted). Low hard-register axis stopped. */
/* 2026-10-02 register trial 2: pinning only destination-column/tile-base
 * plus byte-offset drawing and do-while visibility restores its bottom test
 * but frees both cell pointers into registers; score3577/frame28.
 * Persistent-pointer pins alone do not restore the reference spill model. */
/* 2026-10-02 register trial 3: delaying initial cursor binding and spelling
 * both layer loops as do-while reduces score to2456 (19 register,1 stack,4
 * operand,14 reordered,2 inserted,12 deleted), but still changes
 * cell-pointer spills, first row/column bounds and row calculation.
 * Pin-heavy form not retained; natural score2683/frame36 remains canonical. */
/* 2026-10-02 lifetime trial 2: byte offsets on twice-set drawing pointers
 * retain the bases and cell scaling in the drawing block, but shorten it
 * enough for GCC to rotate the layer loop to a top test; score 3459 with
 * a 28-byte frame. Trial 1 retained; stop this pointer scaling axis. */
/* 2026-10-02 lifetime trial 1: split row calculation and reused n, twice-set
 * table pointers, and a reused 0x800 step restore the honest 36-byte frame;
 * score 2683 (52 register, 3 stack, 6 operand, 19 reordered, 3 inserted,
 * 8 deleted). RTL shows cell scaling and temporary VRAM bases still hoist. */
/* Not-yet-C: complete 316-byte owner including literal pool.
 * Repairs the lift: fifth argument is width, sixth is height; saved layer
 * positions advance forward; each row advances by 128-width cells; the
 * second graphics word comes from +4, not +64. Candidate 312 bytes / 121
 * aligned edits. Scoped copy helper gives the reference's 36-byte frame
 * but 300 bytes / 123 edits; separate table symbols spill more (316 / 123).
 * The original baseline retained equal branch topology. The retained
 * lifetime trial restores the 36-byte frame; table-address lifetimes,
 * mask and index hoisting, and src/dst allocation remain unmatched. */
#include "MAP_SCROLL.H"
#include "RAM_BUFFER.H"
struct TilePos { s32 x, y; };

void Map_CopyMetatileIndicesRect(s32 src_x, s32 src_y,
    u32 dst_x, u32 dst_y, u32 width, u32 height)
{
    /* FAKEMATCH: measured ordinary rewrites retain the reference frame and
       instruction stream; temporary sharing steers register allocation
       and scheduling (score 2683 to 495, no added/deleted instructions). */
    u32 *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
    u32 *dst = (u32 *)Ram_MapCellBuffer + (dst_y * 128 + dst_x);
    register struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
    struct TilePos tile[3];
    s32 i;
    struct TilePos *pos = tile;
    u32 y;
    register u32 x;

    i = 2;
    if (i >= 0) {
        while (1) {
            s32 tmp5;
            s32 tmp8;
            tmp8 = layer->x;
            tmp5 = tmp8 >> 20;
            pos->x = tmp5;
            tmp8 = layer->y;
            tmp5 = tmp8;
            pos->y = tmp5 >> 20;
            ++pos;
            layer++;
            i--;
            if (0 > i)
                break;
        }
    }
    y = dst_y;
    if (y < dst_y + height) {
        do {
            u32 tmp20;
            x = dst_x;
            while (x < dst_x + width) {
                /* FAKEMATCH: the cell value remains live in r5 while layers are tested. */
                register u32 cell __asm__ ("r5") = 0xfff & *src++;
                u32 *dest;
                u32 offset;
                u32 tmp3;
                u32 n;
                register u32 *tiles;
                u32 tmp6;
                u32 tmp9;
                u32 tmp12;
                u32 tmp11;
                u32 tmp13;
                u32 tmp15;
                tmp9 = *dst;
                tmp3 = tmp9 & -0x1000;
                tmp12 = x & 15;
                (s32)(dst[0] = cell | tmp3);
                tmp6 = tmp12;
                dst++;
                tmp15 = y & 15;
                tmp3 = tmp15;
                pos = tile;
                tmp13 = tmp3 << 5;
                tmp12 = tmp13;
                n = tmp12;
                n += tmp6;
                tmp13 = 4 * n;
                i = 0;
                tmp11 = tmp13;
                tmp6 = tmp11;
                offset = tmp6;
                for (; i < 3; i++) {
                    if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y && pos->y + 12 > (s32)y) {
                        u32 *tmp;
                        u32 tmp2;
                        u32 tmp4;
                        u32 *tmp7;
                        u32 tmp10;
                        u32 tmp14;
                        u32 tmp18;
                        u32 tmp21;
                        /* FAKEMATCH: consumed offset in r3 changes only registers/order, preserving all instructions. */
                        register u32 bytes __asm__ ("r3") = cell * 8;
                        u32 tmp16;
                        tmp = (u32 *)Ram_MapBlocks;
                        tiles = tmp;
                        {
                            u32 screen_base = 0x06002800;
                            /* FAKEMATCH: ordinary address addition scores70; low-register operands score40 with only the two literal loads swapped. */
                            __asm__ ("add %0, %1, %2" : "=l" (tmp16) : "l" (offset), "l" (screen_base) : "cc");
                        }
                        tmp21 = tmp16;
                        tmp18 = tmp21;
                        tmp2 = tmp18;
                        tmp = (u32 *)tmp2;
                        dest = tmp;
                        tiles = (u32 *)((u8 *)tiles + bytes);
                        tmp14 = *tiles;
                        tmp10 = tmp14;
                        *dest = tmp10;
                        tmp7 = (u32 *)(Ram_MapBlocks + 4);
                        tiles = tmp7;
                        tiles = (u32 *)((u8 *)tiles + bytes);
                        tmp16 = 0x06002840 + offset;
                        tmp18 = tmp16;
                        tmp14 = tmp18;
                        tmp21 = *tiles;
                        tmp10 = tmp21;
                        tmp4 = tmp10;
                        tmp2 = tmp4;
                        tmp4 = tmp14;
                        tmp7 = (u32 *)tmp4;
                        dest = tmp7;
                        (s32)(*dest = tmp2);
                        break;
                    }
                    n = 0x800;
                    offset += n;
                    pos++;
                }
                x++;
            }
            src += (s32)(128 - width);
            tmp20 = 128 - width;
            dst = dst + tmp20;
            y++;
        } while (y < dst_y + height);
    }
}
