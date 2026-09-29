/* Not-yet-C: complete 316-byte owner including literal pool.
 * Repairs the lift: fifth argument is width, sixth is height; saved layer
 * positions advance forward; each row advances by 128-width cells; the
 * second graphics word comes from +4, not +64. Candidate 312 bytes / 121
 * aligned edits. Scoped copy helper gives the reference's 36-byte frame
 * but 300 bytes / 123 edits; separate table symbols spill more (316 / 123).
 * Baseline retains equal branch topology; remaining table-address lifetimes,
 * mask hoisting and the 44-byte versus 36-byte frame are not matched. */
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

            *dst = (*dst & -0x1000) | cell;
            dst++;
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
