#include "types.h"

/*
 * Scroll the three background layers to a new view point and refill each
 * enabled layer's screen block from the map at 0x02010000 and the tile
 * quadrant tables at 0x02020000.
 *
 * The view point is first biased - x by -0x00780000 and y by -0x00600000 -
 * and then clamped into the box the context keeps at +0xec/+0xf0 (minimum)
 * and +0xf4/+0xf8 (maximum, less 0x00f00000 and 0x00a00000 respectively).
 * The clamped point is published at +0xe4 and +0xe8 before any layer is
 * touched.
 *
 * The enable bytes live at context+0x100, one per layer, and the 48-byte
 * layer descriptors start at context+0x104.  A disabled layer does not
 * advance the descriptor cursor, so the descriptors are consumed only by the
 * layers that are actually drawn.
 *
 * A drawn layer scales the view point by its own +0x10/+0x14 factors, adds
 * its running scroll - the +0x18/+0x1c steps accumulated at +0x20/+0x24 and
 * masked by the halfword at +0x28/+0x2a shifted up 19 - and adds the fixed
 * offsets at +0x08/+0x0c.  A layer whose y step is live is 16 rows tall
 * instead of 11.  The 16.19 result is converted to whole tiles with the usual
 * truncating `if (v < 0) v += 0x7ffff; v >>= 19`.
 *
 * Each of the 16 columns of a row reads the map word, takes its low twelve
 * bits scaled by eight as the offset of a four-entry quadrant record, and
 * writes the record's first word to the screen entry and its second 64 bytes
 * further on - the lower half of the same 2x2 tile.  Row and column indices
 * wrap in the map (127 and 30) and in the screen block (0x3f80 and 960)
 * rather than being bounds-checked.
 *
 * The four `mov ip, pc ; bx r3` sequences at 0x080102c4 and 0x080102d4 call
 * the pooled 0x03000118, the IWRAM-relocated ARM fixed-point multiply, in the
 * same frame - the reading established for 0x0800ebec - and are written as
 * ordinary calls through a function pointer.  Nothing in the row is retained
 * assembly.
 *
 * The epilogue at 0x080103e6 ends `pop {r0} ; bx r0`, so the owner is void.
 * One direct caller, 0x0800feb6, and the address is also published as a Thumb
 * pointer at 0x08009124.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_08010230)(s32 a, s32 b);

struct Layer_08010230 {
    u8 unknown_00[8];
    s32 offset_x;
    s32 offset_y;
    s32 scale_x;
    s32 scale_y;
    s32 step_x;
    s32 step_y;
    s32 scroll_x;
    s32 scroll_y;
    u16 wrap_x;
    u16 wrap_y;
    u8 unknown_2c[4];
};

void Func_08010230(s32 x, s32 y)
{
    Multiply_08010230 multiply = (Multiply_08010230)0x03000118;
    u8 *context;
    struct Layer_08010230 *layer;
    const u32 *map = (const u32 *)0x02010000;
    s32 limit;
    s32 view_x;
    s32 view_y;
    s32 rows;
    s32 biased_x;
    s32 biased_y;
    s32 tile_x;
    s32 tile_y;
    u32 index;
    u32 row;
    u32 column;
    u32 map_row;
    u32 map_column;
    u32 screen_row;
    u32 screen_column;
    u32 *block;

    context = *(u8 **)0x03001e70;
    layer = (struct Layer_08010230 *)(context + 260);

    view_y = y - 0x00600000;
    view_x = x - 0x00780000;

    limit = *(s32 *)(context + 236);
    if (view_x < limit) {
        view_x = limit;
    }
    limit = *(s32 *)(context + 244) - 0x00f00000;
    if (view_x > limit) {
        view_x = limit;
    }
    limit = *(s32 *)(context + 240);
    if (view_y < limit) {
        view_y = limit;
    }
    limit = *(s32 *)(context + 248) - 0x00a00000;
    if (view_y > limit) {
        view_y = limit;
    }

    *(s32 *)(context + 228) = view_x;
    *(s32 *)(context + 232) = view_y;

    for (index = 0; index <= 2; index++) {
        if (context[256 + index] == 0) {
            continue;
        }

        rows = 22;
        biased_x = multiply(*(s32 *)(context + 228), layer->scale_x);
        biased_y = multiply(*(s32 *)(context + 232), layer->scale_y);

        if (layer->step_x != 0) {
            layer->scroll_x += layer->step_x;
            biased_x += layer->scroll_x;
            biased_x &= ((s32)layer->wrap_x << 19) | 0x0007ffff;
        }
        if (layer->step_y != 0) {
            layer->scroll_y += layer->step_y;
            biased_y += layer->scroll_y;
            biased_y &= ((s32)layer->wrap_y << 19) | 0x0007ffff;
            rows = 32;
        }

        biased_x += layer->offset_x;
        biased_y += layer->offset_y;
        layer++;

        tile_x = biased_x;
        if (biased_x < 0) {
            tile_x = biased_x + 0x7ffff;
        }
        tile_x >>= 19;
        tile_y = biased_y;
        if (biased_y < 0) {
            tile_y = biased_y + 0x7ffff;
        }
        tile_y >>= 19;

        block = (u32 *)(0x06002800 + (index << 11));

        map_row = (u32)((tile_y + (s32)((u32)(biased_y < 0
                                              ? biased_y + 0x7ffff
                                              : biased_y) >> 31)) >> 1) & 127;
        map_row <<= 7;
        screen_row = ((u32)tile_y & 30) << 5;

        rows >>= 1;
        for (row = 0; (s32)row < rows; row++) {
            map_column = (u32)(((tile_x
                                 + (s32)((u32)(biased_x < 0
                                               ? biased_x + 0x7ffff
                                               : biased_x) >> 31)) >> 1)) & 127;
            screen_column = (u32)tile_x & 30;

            for (column = 0; column <= 15; column++) {
                u32 quadrant = (map[map_row + map_column] & 0xfff) * 8;
                u32 *entry = (u32 *)((u8 *)block
                                     + ((screen_row + screen_column) << 1));

                entry[0] = *(const u32 *)(0x02020000 + quadrant);
                entry[16] = *(const u32 *)(0x02020004 + quadrant);

                map_column = (map_column + 1) & 127;
                screen_column = (screen_column + 2) & 30;
            }

            map_row = (map_row + 128) & 0x3f80;
            screen_row = (screen_row + 64) & 960;
        }
    }
}
