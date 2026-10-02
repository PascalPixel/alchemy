#include "TYPES.H"
#include "GLOBAL_CELLS.H"

/* A window's frame on the 32-tile-wide text tilemap. */
struct UiWindowFrame {
    u8 unknown_00[8];
    u16 width;
    u16 height;
    u16 left;
    u16 top;
};

enum {
    TILEMAP_ENTRY_PLAIN,
    TILEMAP_ENTRY_NONE,
    TILEMAP_ENTRY_PALETTE_14,
    TILEMAP_ENTRY_PALETTE_15,
    TILEMAP_ENTRY_PALETTE_1
};

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

extern u16 *Data_03001e8c;

/* Writes one tile into a window's tilemap; modes 2-4 add palette bits. */
void UiWindow_SetTilemapEntry(
    struct UiWindowFrame *window, s32 value, s32 x, s32 y, u32 mode)
{
    u16 *map = Data_03001e8c;
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
        index = (window->top + y) * TILEMAP_WIDTH + (window->left + x);
        if ((u32)index >= TILEMAP_ENTRIES)
            return;
        *(u16 *)((u8 *)map + (index << 1)) = palette | value;
        return;
    case TILEMAP_ENTRY_PLAIN:
    default:
        index = (window->top + y) * TILEMAP_WIDTH + (window->left + x);
        if ((u32)index >= TILEMAP_ENTRIES)
            return;
        *(u16 *)((u8 *)map + (index << 1)) = value;
        return;
    }
}

struct ScaleEffect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};
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
struct AnimatedOutput {
    u8 unknown_00[5];
    u8 mode;
    u16 x;
    u16 y;
    u16 unknown_0a;
    u16 frame;
    u8 unknown_0e[2];
    struct OutputSprite sprite;
};
extern u16 Data_080366f8[];
s32 AffineMatrix_BuildForEffect(struct ScaleEffect *);

/* Steps the output's scale animation through the scale table (modes 9 and
   10 loop, 11 and 12 play eight steps once, 10 and 12 at half size) and
   sets the sprite's affine mode: none at 1.0 (256), double-size and moved
   8 pixels up and left when larger, plain affine when smaller. */
void RenderOutput_UpdateScaleAnimation(struct AnimatedOutput *output)
{
    s32 scale = 256;
    struct OutputSprite *sprite = &output->sprite;
    struct ScaleEffect effect;

    switch (output->mode) {
    case 9:
        scale = Data_080366f8[output->frame++ & 31];
        break;
    case 10:
        scale = Data_080366f8[output->frame++ & 31] >> 1;
        break;
    case 11:
        if (output->frame <= 7)
            scale = Data_080366f8[output->frame++ * 2 + 16];
        break;
    case 12:
        if (output->frame <= 7)
            scale = Data_080366f8[output->frame++ * 2 + 16] >> 1;
        break;
    }
    if (scale == 256) {
        sprite->affine_index = 0;
        sprite->affine = 0;
        sprite->x = output->x;
        sprite->y = output->y;
    } else {
        effect.x = scale;
        effect.y = scale;
        effect.angle = 0;
        sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
        if (scale > 256) {
            sprite->affine = 3;
            sprite->x = output->x - 8;
            sprite->y = output->y - 8;
        } else {
            sprite->affine = 1;
            sprite->x = output->x;
            sprite->y = output->y;
        }
    }
}
