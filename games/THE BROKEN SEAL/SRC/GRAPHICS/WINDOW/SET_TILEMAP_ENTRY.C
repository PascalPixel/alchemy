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

    /* FAKEMATCH: the plain write is a separate tail reached by goto so the
       palette and plain stores keep their own copies of the bounds check,
       and the byte-offset store keeps map as the strh offset register. */
    switch (mode) {
    case TILEMAP_ENTRY_PLAIN:
        goto plain;
    case TILEMAP_ENTRY_NONE:
        return;
    case TILEMAP_ENTRY_PALETTE_14:
    case TILEMAP_ENTRY_PALETTE_15:
    case TILEMAP_ENTRY_PALETTE_1:
        break;
    default:
        goto plain;
    }

    index = (window->top + y) * TILEMAP_WIDTH + (window->left + x);
    if ((u32)index >= TILEMAP_ENTRIES)
        return;
    *(u16 *)((u8 *)map + (index << 1)) = palette | value;
    return;

plain:
    index = (window->top + y) * TILEMAP_WIDTH + (window->left + x);
    if ((u32)index >= TILEMAP_ENTRIES)
        return;
    *(u16 *)((u8 *)map + (index << 1)) = value;
}
