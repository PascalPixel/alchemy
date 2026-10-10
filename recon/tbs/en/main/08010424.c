/* Pin-free Map_CopyMetatileIndicesRect trial: removing the remaining cell
 * pin leaves 67 differences (141 native, 142 built, 108 common).
 * The linked byte-offset removal matches all six editions; cell instead
 * takes r1 and x takes r5, and the scale moves before the layer tests.
 * An output on cell at the scale leaves 71 differences. A generic input
 * or output before the destination load leaves 88 differences, with
 * 143 built instructions. Reusing bytes for the final tile word retains
 * 28 differences with cell pinned; reusing offset leaves 164.
 * A generic cell output before the layer tests or destination store
 * leaves 88 differences; inputs at those points leave 92 and 96.
 * Keeping cell live on the next-layer path leaves 88, or 163 with an
 * output. Signed cell or x, separate cell masking and removal of the
 * ordinary register qualifiers all retain 67.
 */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "iwram_call.h"
#include "dma.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "global_cells.h"
#include "map_scroll.h"
#include "resource.h"
#include "resource_ids.h"
#include "flag_ids.h"

struct TilePos { s32 x, y; };

void Map_CopyMetatileIndicesRect(s32 src_x, s32 src_y,
    u32 dst_x, u32 dst_y, u32 width, u32 height)
{
    /* FAKEMATCH: needed to match: temporary sharing preserves register allocation and scheduling without changing the frame or instructions. */
    u32 *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
    u32 *dst = (u32 *)Ram_MapCellBuffer + (dst_y * 128 + dst_x);
    register struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
    struct TilePos tile[3];
    s32 i;
    struct TilePos *pos = tile;
    u32 y;
    register u32 x;

    for (i = 2; i >= 0; i--) {
        pos->x = layer->x >> 20;
        pos->y = layer->y >> 20;
        pos++;
        layer++;
    }
    for (y = dst_y; y < dst_y + height; y++) {
        for (x = dst_x; x < dst_x + width; x++) {
            // Ordinary cell; allocation still differs from the native.
            u32 cell = 0xfff & *src++;
            volatile u32 *dest;
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
            dst[0] = cell | tmp3;
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
                    
                    u32 bytes;
                    u32 tmp16;
                    {
                        bytes = cell * 8;
                        tmp2 = 0x06002800;
                        tmp = (u32 *)Ram_MapBlocks;
                        tiles = tmp;
                        
                        tmp16 = offset + tmp2;
                        /* FAKEMATCH: keeping the cell live through the scaled-offset output removes the byte-offset pin in the linked function; the ordinary cell still differs. */
                        __asm__("" : "+l"(bytes) : "l"(cell));
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
                    *dest = tmp2;
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
