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
    struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
    register struct TilePos tile[3];
    s32 i;
    struct TilePos *pos = tile;
    u32 y;
    register u32 x;

    i = 2;
    if (i >= 0) {
        do {
            s32 tmp5;
            tmp5 = layer->x >> 20;
            pos->x = tmp5;
            tmp5 = layer->y;
            pos->y = tmp5 >> 20;
            pos++;
            layer++;
            i--;
        } while (i >= 0);
    }
    if ((y = dst_y) < dst_y + height) {
        do {
            x = dst_x;
            while (x < dst_x + width) {
                u32 cell = 0xfff & *src++;
                u32 offset;
                u32 *dest;
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
                dst[0] = cell | tmp3;
                tmp12 = x & 15;
                tmp6 = tmp12;
                tmp15 = y & 15;
                tmp3 = tmp15;
                tmp13 = tmp3 << 5;
                dst++;
                tmp12 = tmp13;
                n = tmp12;
                pos = tile;
                n += tmp6;
                tmp13 = n * 4;
                tmp11 = tmp13;
                tmp6 = tmp11;
                offset = tmp6;
                for (i = 0; i < 3; i++) {
                    if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y && pos->y + 12 > (s32)y) {
                        u32 *tmp;
                        u32 tmp2;
                        u32 tmp4;
                        u32 *tmp7;
                        u32 tmp10;
                        u32 tmp14;
                        s32 tmp8;
                        tmp = (u32 *)Ram_MapBlocks;
                        n = cell * 2;
                        tiles = tmp;
                        tmp2 = 0x06002800 + offset;
                        tmp = (u32 *)tmp2;
                        dest = tmp;
                        tmp8 = (s32)n;
                        tiles += tmp8;
                        tmp14 = *tiles;
                        tmp10 = tmp14;
                        *dest = tmp10;
                        tmp7 = (u32 *)(Ram_MapBlocks + 4);
                        tiles = tmp7;
                        tiles += n;
                        tmp10 = *tiles;
                        tmp4 = tmp10;
                        tmp2 = tmp4;
                        tmp14 = 0x06002840 + offset;
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
            src += 128 - width;
            dst = 128 - width + dst;
            y++;
        } while (y < dst_y + height);
    }
}
