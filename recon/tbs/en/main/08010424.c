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
#include "TYPES.H"

struct LayerScroll {
    s32 x, y;
    u8 unknown_08[40];
};
struct MapWork {
    u8 unknown_00[0x104];
    struct LayerScroll layers[3];
};
struct TilePos { s32 x, y; };

extern struct MapWork *gMapWork;

void Map_CopyMetatileIndicesRect(s32 src_x, s32 src_y,
    u32 dst_x, u32 dst_y, u32 width, u32 height)
{
    u32 *src = (u32 *)0x02010000 + (src_y * 128 + src_x);
    u32 *dst = (u32 *)0x02010000 + (dst_y * 128 + dst_x);
    struct LayerScroll *layer = gMapWork->layers;
    struct TilePos tile[3];
    struct TilePos *pos = tile;
    s32 i;
    u32 y;
    u32 x;

    for (i = 2; i >= 0; i--) {
        pos->x = layer->x >> 20;
        pos->y = layer->y >> 20;
        layer++;
        pos++;
    }
    for (y = dst_y; y < dst_y + height; y++) {
        for (x = dst_x; x < dst_x + width; x++) {
            u32 cell = *src++ & 0xfff;
            u32 offset;
            u32 n;
            u32 *tiles;
            u32 *dest;

            *dst = (*dst & -0x1000) | cell;
            dst++;
            n = (y & 15) << 5;
            n += x & 15;
            offset = n * 4;
            pos = tile;
            for (i = 0; i < 3; i++) {
                if (pos->x <= (s32)x && pos->x + 16 > (s32)x &&
                    pos->y <= (s32)y && pos->y + 12 > (s32)y) {
                    n = cell * 2;
                    tiles = (u32 *)0x02020000;
                    dest = (u32 *)(0x06002800 + offset);
                    tiles += n;
                    *dest = *tiles;
                    tiles = (u32 *)0x02020004;
                    tiles += n;
                    dest = (u32 *)(0x06002840 + offset);
                    *dest = *tiles;
                    break;
                }
                n = 0x800;
                offset += n;
                pos++;
            }
        }
        src += 128 - width;
        dst += 128 - width;
    }
}
