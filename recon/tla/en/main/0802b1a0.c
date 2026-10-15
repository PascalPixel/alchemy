/* Draft, differ 150 of 142: The Broken Seal twin with this game layer type. The native keeps the
 * destination x in r11 and a 36-byte frame (three positions and three spills: the row end, the
 * destination and the source cursors); this draft spills the destination x as a fourth and keeps the
 * position array in r11 (40-byte frame). A plain rewrite without the twin devices is differ 211. */
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapBlocks[];
#define Ram_MapBlocks gMapBlocks

struct TilePos { s32 x, y; };

void Map_CopyMetatileIndicesRect(s32 srcX, s32 srcY,
    u32 dstX, u32 dstY, u32 width, u32 height)
{
    /* FAKEMATCH: needed to match: temporary sharing preserves register allocation and scheduling without changing the frame or instructions. */
    u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
    u32 *dst = (u32 *)Ram_MapCellBuffer + (dstY * 128 + dstX);
    struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
    struct TilePos tile[3];
    s32 i;
    struct TilePos *pos = tile;
    u32 y;
    u32 x;

    for (i = 2; i >= 0; i--) {
        pos->x = layer->x >> 20;
        pos->y = layer->y >> 20;
        pos++;
        layer++;
    }
    for (y = dstY; y < dstY + height; y++) {
        for (x = dstX; x < dstX + width; x++) {
            u32 val = *src++;
            u32 cell = val & 0xfff;
            u32 offset;
            u32 tmp3;
            u32 n;
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
            /* FAKEMATCH: cell and i are ordinary locals; three measured forms
             * replace the r5 pin on cell and the asm barrier on its scale.
             * 1. i = 0 here, not after the offset: i's live length (54 -> 60
             *    insns, 22 refs) drops its global-allocation priority below
             *    cell's (15 refs over 30), so cell takes r5 and i r6 as the
             *    native does (i = 0 after the offset: differ 59; at the top of
             *    the body: 32).
             * 2. The scale goes into the x-loop scratch tmp11 inside the layer
             *    loop: tmp11's first reference is outside that loop, so loop.c
             *    (reg_in_basic_block_p under maybe_never) leaves the shift in
             *    the conditional block; a fresh `u32 bytes = cell * 8` is
             *    invariant and hoisted before the window tests (differ 158).
             * 3. The scale comes after the first destination: cell's r5 is
             *    still live when reload materializes 0x6002800 for the folded
             *    add, so reload picks r2 (scale first: r5, differ 4).
             * The scr/blk variables and the dest = (u8 *)scr / tiles = (u8 *)blk
             * copies keep the layer loop at 31 real insns, above loop.c's
             * threshold of 30, so the four address constants and the 0x800
             * step stay in the loop (plain expressions: 29 insns, hoisted,
             * differ 113-153).
             * 4. The two word copies read and write through volatile casts: the
             *    VRAM store is hardware, and the volatile load from the block
             *    buffer keeps its add apart from the load (a non-volatile load
             *    combines into ldr [r, r]); these four volatiles replaced two pins
             *    and two barriers and raised the volatile count (2026-10-11). */
            i = 0;
            tmp15 = y & 15;
            tmp3 = tmp15;
            pos = tile;
            tmp13 = tmp3 << 5;
            tmp12 = tmp13;
            n = tmp12;
            n += tmp6;
            tmp13 = 4 * n;
            tmp11 = tmp13;
            tmp6 = tmp11;
            offset = tmp6;
            for (; i < 3; i++) {
                if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y && pos->y + 12 > (s32)y) {
                    u32 scr, blk;
                    u8 *dest, *tiles;

                    scr = 0x06002800;
                    dest = (u8 *)scr;
                    dest += offset;
                    tmp11 = cell * 8;
                    blk = (u32)Ram_MapBlocks;
                    tiles = (u8 *)blk;
                    tiles += tmp11;
                    *(volatile u32 *)dest = *(volatile u32 *)tiles;

                    blk = (u32)Ram_MapBlocks + 4;
                    tiles = (u8 *)blk;
                    tiles += tmp11;
                    scr = 0x06002840;
                    dest = (u8 *)scr;
                    dest += offset;
                    *(volatile u32 *)dest = *(volatile u32 *)tiles;
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
