#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "WINDOW.H"
#include "AFFINE.H"

enum {
    TILEMAP_ENTRY_PLAIN,
    TILEMAP_ENTRY_NONE,
    TILEMAP_ENTRY_PALETTE_14,
    TILEMAP_ENTRY_PALETTE_15,
    TILEMAP_ENTRY_PALETTE_1
};

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

/* Writes one tile into a window's tilemap; modes 2-4 add palette bits. */
void UiWindow_SetTilemapEntry(
    struct UiWindow *window, s32 value, s32 x, s32 y, u32 mode)
{
    u16 *map = ((struct UiRenderWork *)gWindowWork[0])->tilemap;
    s32 palette;
    s32 index;

    y += 1;
    x += 1;
    if ((u32)y > (u32)(window->height - 1))
        return;
    if ((u32)x > (u32)(window->width - 1))
        return;

    switch (mode) {
    case TILEMAP_ENTRY_PALETTE_15:
        palette = 0xf000;
        break;
    case TILEMAP_ENTRY_PALETTE_14:
        palette = 0xe000;
        break;
    case TILEMAP_ENTRY_PALETTE_1:
        palette = 0x1000;
        break;
    default:
        palette = 0;
        break;
    }

    /* FAKEMATCH: byte-offset stores preserve the strh base and offset
       register order; map[index] reverses them in both stores. */
    switch (mode) {
    case TILEMAP_ENTRY_NONE:
        return;
    case TILEMAP_ENTRY_PALETTE_14:
    case TILEMAP_ENTRY_PALETTE_15:
    case TILEMAP_ENTRY_PALETTE_1:
        index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
        if ((u32)index >= TILEMAP_ENTRIES)
            return;
        *(u16 *)((u8 *)map + (index << 1)) = palette | value;
        return;
    case TILEMAP_ENTRY_PLAIN:
    default:
        index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
        if ((u32)index >= TILEMAP_ENTRIES)
            return;
        *(u16 *)((u8 *)map + (index << 1)) = value;
        return;
    }
}

struct OutputSprite {
    u32 link;
    u8 y;
    u8 affine : 2;
    u8 mode : 2;
    u8 other : 4;
    u16 x : 9;
    u16 affine_index : 5;
    u16 other_x : 2;
    u16 tile;
};
extern u16 Data_080366f8[];

/* Steps the output's scale animation through the scale table (modes 9 and
   10 loop, 11 and 12 play eight steps once, 10 and 12 at half size) and
   sets the sprite's affine mode: none at 1.0 (256), double-size and moved
   8 pixels up and left when larger, plain affine when smaller. */
void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output)
{
    s32 scale = 256;
    struct OutputSprite *sprite = (struct OutputSprite *)output->unknown_10;
    struct AffineTransform effect;

    switch ((u8)output->active) {
    case 9:
        scale = Data_080366f8[output->unknown_0c++ & 31];
        break;
    case 10:
        scale = Data_080366f8[output->unknown_0c++ & 31] >> 1;
        break;
    case 11:
        if (output->unknown_0c <= 7)
            scale = Data_080366f8[output->unknown_0c++ * 2 + 16];
        break;
    case 12:
        if (output->unknown_0c <= 7)
            scale = Data_080366f8[output->unknown_0c++ * 2 + 16] >> 1;
        break;
    }
    if (scale == 256) {
        sprite->affine_index = 0;
        sprite->affine = 0;
        sprite->x = (u16)output->x;
        sprite->y = (u16)output->y;
    } else {
        effect.scale_x = scale;
        effect.scale_y = scale;
        effect.angle = 0;
        sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
        if (scale > 256) {
            sprite->affine = 3;
            /* FAKEMATCH: preserve the original unsigned halfword read at
               the logical x cell. Casting the signed member value swaps the
               x load and -8 pool load at the same 460-byte module extent. */
            sprite->x = *(u16 *)&output->x - 8;
            sprite->y = (u16)output->y - 8;
        } else {
            sprite->affine = 1;
            sprite->x = (u16)output->x;
            sprite->y = (u16)output->y;
        }
    }
}
