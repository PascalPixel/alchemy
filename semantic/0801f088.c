#include "layout_guard.h"
#include "types.h"

typedef struct DmaChannel_0801f088 {
    const void *source;
    void *destination;
    u32 control;
} DmaChannel_0801f088;

typedef struct TileRegion_0801f088 {
    u8 padding00[0x0c];
    u16 x_offset;
    u16 y_offset;
} TileRegion_0801f088;

typedef union DisplayRuntime_0801f088 {
    u16 tilemap[0x753];
    struct {
        u8 padding000[0xea5];
        u8 palette_owned;
    } control;
} DisplayRuntime_0801f088;

LAYOUT_OFFSET_GUARD(
    TileRegion0801f088_XOffset,
    TileRegion_0801f088,
    x_offset,
    0x0c);
LAYOUT_OFFSET_GUARD(
    TileRegion0801f088_YOffset,
    TileRegion_0801f088,
    y_offset,
    0x0e);
LAYOUT_OFFSET_GUARD(
    DisplayRuntime0801f088_PaletteOwned,
    DisplayRuntime_0801f088,
    control.palette_owned,
    0xea5);

extern DisplayRuntime_0801f088 *Data_03001e8c;

const void *Func_080045e8(void);

static void BuildReplacementMasks_0801f088(
    s32 boundary,
    u32 *color_e,
    u32 *color_1)
{
    *color_e = 0x22222222;
    *color_1 = 0xcccccccc;

    if (boundary > 7) {
        *color_e = 0x88888888;
        *color_1 = 0xdddddddd;
    } else if (boundary >= 0) {
        u32 shift = (u32)boundary * 4;
        u32 opposite = 32 - shift;
        u32 e_tail = opposite == 32 ? 0 : 0x88888888 >> opposite;
        u32 one_tail = opposite == 32 ? 0 : 0xdddddddd >> opposite;

        *color_e = (0x22222222 << shift) | e_tail;
        *color_1 = (0xcccccccc << shift) | one_tail;
    }
}

static u32 RecolorPixels_0801f088(
    u32 pixels,
    u32 color_e,
    u32 color_1)
{
    u32 recolored = 0;
    u32 pixel;

    for (pixel = 0; pixel < 8; pixel++) {
        u32 shift = (u32)pixel * 4;
        u32 value = pixels & 0x0f;
        u32 nibble_mask = 0x0f << shift;

        if (value == 0x0e)
            recolored |= color_e & nibble_mask;
        else if (value == 1)
            recolored |= color_1 & nibble_mask;
        else
            recolored |= value << shift;
        pixels >>= 4;
    }

    return recolored;
}

/*
 * Recolor a five-tile horizontal strip as a wipe crosses it.  Palette indices
 * 0xe and 1 are replaced with complementary dither masks; other pixels are
 * preserved.  A nonzero mode updates three tile rows, while mode zero updates
 * only the bottom row.
 */
void Func_0801f088(
    TileRegion_0801f088 *region,
    s32 x,
    s32 y,
    s32 boundary)
{
    DisplayRuntime_0801f088 *runtime = Data_03001e8c;
    s32 tile_x = x + region->x_offset;
    s32 tile_row = (y + region->y_offset) << 5;
    s32 initial_boundary = boundary;
    u32 column;

    if (runtime->control.palette_owned == 0) {
        volatile DmaChannel_0801f088 *dma =
            (volatile DmaChannel_0801f088 *)0x040000d4;

        dma->source = Func_080045e8();
        dma->destination = (void *)0x050001c0;
        dma->control = 0x80000010;
        *(volatile u16 *)0x050001dc = *(volatile u16 *)0x050001e8;
    }

    for (column = 0; column < 5; column++) {
        u32 color_e;
        u32 color_1;
        u16 tile_id = runtime->tilemap[tile_row + tile_x] & 0x03ff;
        volatile u32 *tile_end =
            (volatile u32 *)(0x0600001c + ((u32)tile_id << 5));
        s32 row_count = initial_boundary != 0 ? 3 : 1;
        s32 row;

        BuildReplacementMasks_0801f088(boundary, &color_e, &color_1);
        for (row = 0; row < row_count; row++) {
            volatile u32 *pixels = tile_end - row;

            *pixels = RecolorPixels_0801f088(*pixels, color_e, color_1);
        }

        boundary -= 8;
        tile_x++;
    }
}
