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
    u32 *src = (u32 *)0x02010000 + (src_y * 128 + src_x);
    u32 *dst = (u32 *)0x02010000 + (dst_y * 128 + dst_x);
    struct MapLayerScroll *layer = Data_03001e70->layers;
    struct TilePos tile[3];
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
    for (y = dst_y; y < dst_y + height; y++) {
        for (x = dst_x; x < dst_x + width; x++) {
            u32 cell = *src++;
            u32 offset;

            *dst++ = cell;
            cell &= 0xfff;
            offset = (((y & 15) << 5) + (x & 15)) * 4;
            pos = tile;
            for (i = 0; i < 3; i++) {
                if (pos->x <= x && pos->x + 16 > x &&
                    pos->y <= y && pos->y + 12 > y) {
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
