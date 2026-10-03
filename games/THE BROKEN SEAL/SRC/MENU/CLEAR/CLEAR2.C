#include "RESOURCE.H"
#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "WINDOW.H"


/* Fills a 16 by 8 block of window tiles, numbered row by row in palette 15, into VRAM and its shadow copy. */
void SaveMenu_FillTileGrid(struct RenderInput *window, s32 value)
{
    u16 *shadow;
    u16 *vram;
    s32 saved;
    s32 row;
    s32 col;
    s32 offset;

    shadow = ((struct UiRenderWork *)gWindowWork[0])->tilemap;
    saved = (s32)Runtime_BumpAllocateAlternatePool(0x300);
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
    Runtime_BumpFree((void *)saved);
}

void SaveMenu_FillTileBlock(const struct RenderInput *window)
{
    s16 *mirror = (s16 *)((struct UiRenderWork *)gWindowWork[0])->tilemap;
    s16 *buffer = (s16 *)Runtime_BumpAllocateAlternatePool(0x300);
    s16 *vram;
    s32 cell;
    s32 row;
    s32 base;

    cell = window->y * 32 + window->x;
    vram = (s16 *)0x06002000 + cell;
    mirror += cell;
    row = 0;
    base = 0;
    do {
        s32 tile = base + 32;
        s32 remaining = 15;

        do {
            s16 value = (s16)(tile | -0x1000);

            remaining--;
            *vram = value;
            tile++;
            *mirror = value;
            vram++;
            mirror++;
        } while (remaining >= 0);
        row++;
        vram += 16;
        mirror += 16;
        base += 16;
    } while (row <= 7);
    Runtime_BumpFree(buffer);
}
