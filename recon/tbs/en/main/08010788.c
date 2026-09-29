/* 2026-09-29: eight minutes of permutation found 1940 from 3060 (alchemy
 * permute --function Func_08010788). The natural form kept here scores 2470
 * (39 register-only, 5 stack-only, 6 operand, 7 reordered, 10 inserted, 7
 * deleted): the layer loop as a while with its steps at the end, the tile
 * cursor reset before the cell store, and the left-edge test kept in a
 * local. The 1940 candidate needs that test normalized twice through
 * register temporaries (flag = left <= x; flag2 = flag != 0), which no
 * programmer would write. */
/* Not-yet-C: complete 316-byte signed-coordinate index-copy owner.
 * Reuses MAP_SCROLL.H and the metatile row copy model. Corrects the lift:
 * dimensions are arguments three/four, destination coordinates five/six;
 * layer positions walk forward; rows advance by 128-width; bottom graphics
 * word is at +4, not +64. First corrected model gives 300/316 bytes,
 * 148 differing halfwords / 114 aligned edits, equal branch topology.
 * Like 08010424/080105d4, the remaining gap is loop-invariant lifetimes and
 * spilling. No further spelling sweep: inspect loop/allocation dumps first.
 */
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

void Func_08010788(s32 src_x, s32 src_y,
    s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    u32 *src = (u32 *)0x02010000 + (src_y * 128 + src_x);
    u32 *dst = (u32 *)0x02010000 + (dst_y * 128 + dst_x);
    struct MapLayerScroll *layer = gMapWork->layers;
    struct TilePos tile[3];
    struct TilePos *pos = tile;
    s32 i;
    s32 y;
    s32 x;

    i = 2;
    while (i >= 0) {
        pos->x = layer->x >> 20;
        pos->y = layer->y >> 20;
        pos++;
        i--;
        layer++;
    }
    for (y = dst_y; y < dst_y + height; y++) {
        for (x = dst_x; x < dst_x + width; x++) {
            u32 cell = *src++ & 0xfff;
            u32 offset;
            pos = tile;
            *dst = (*dst & -0x1000) | cell;
            dst++;
            offset = (((y & 15) << 5) + (x & 15)) * 4;
            for (i = 0; i < 3; i++) {
                s32 inside = pos->x <= x;

                if (inside && pos->x + 16 > x && pos->y <= y && pos->y + 12 > y) {
                    CopyCell(cell, offset);
                    break;
                }
                offset += 0x800;
                pos++;
            }
        }
        src += 128 - width;
        dst += 128 - width;
    }
}
