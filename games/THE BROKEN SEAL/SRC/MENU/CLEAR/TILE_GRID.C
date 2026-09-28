#include "TYPES.H"

s32 Runtime_BumpAllocateAlternatePool(s32 mask);
void Resource_DecodeType01(s32 value, s32 saved);
void Runtime_BumpFree(s32 saved);

extern u8 *gWindowWork;

struct TileWindow {
    u8 unknown_00[8];
    u16 width;
    u8 unknown_0a[2];
    u16 x;
    u16 y;
};

/* Fills a 16 by 8 block of window tiles, numbered row by row in palette 15, into VRAM and its shadow copy. */
void SaveMenu_FillTileGrid(struct TileWindow *window, s32 value)
{
    u16 *shadow;
    u16 *vram;
    s32 saved;
    s32 row;
    s32 col;
    s32 offset;

    shadow = (u16 *)gWindowWork;
    saved = Runtime_BumpAllocateAlternatePool(0x300);
    Resource_DecodeType01(value, saved);
    offset = (window->y * 32 + window->x) * 2;
    vram = (u16 *)(0x06002000 + offset);
    shadow = (u16 *)((u8 *)shadow + offset);
    for (row = 0; row <= 7; row++) {
        for (col = 0; col <= 15; col++) {
            s16 tile = (window->width * row + col) | 0xf000;

            *vram++ = tile;
            *shadow++ = tile;
        }
        vram += 16;
        shadow += 16;
    }
    Runtime_BumpFree(saved);
}
