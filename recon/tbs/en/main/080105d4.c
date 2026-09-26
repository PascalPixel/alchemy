/* Not-yet-C: complete 304-byte owner and literal pool, split from 08010000.
 * Corrected ABI: width/height are arguments three/four, destination x/y
 * are five/six. Unlike 08010424 this copies the whole map cell, not only
 * its low 12 bits. Recovered forward layer-position save, 128-width row
 * advance, and the graphics second word at +4 (old draft used +64).
 * Candidate 296/304 bytes, 113 differing halfwords / 90 aligned edits,
 * equal branch topology. Remaining: 44-byte versus 36-byte frame and
 * table-address/stride lifetimes, like the neighbouring copy owner. */
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

extern struct MapWork *Data_03001e70;

void Map_CopyMetatileCellsRect(s32 src_x, s32 src_y,
    s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    u32 *src = (u32 *)0x02010000 + (src_y * 128 + src_x);
    u32 *dst = (u32 *)0x02010000 + (dst_y * 128 + dst_x);
    struct LayerScroll *layer = Data_03001e70->layers;
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
                if (pos->x <= (s32)x && pos->x + 16 > (s32)x &&
                    pos->y <= (s32)y && pos->y + 12 > (s32)y) {
                    *(u32 *)(0x06002800 + offset) = ((u32 *)0x02020000)[cell * 2];
                    *(u32 *)(0x06002840 + offset) = ((u32 *)0x02020004)[cell * 2];
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
