#include "TYPES.H"

/* The Psynergy list's range tiles. */
#define RANGE_ICON_TILE 0xf300

void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

/* Draws a range as three tiles: the outer tile of the range's pair,
   mirrored, then the inner tile, then the outer tile again. */
void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range)
{
    s32 n;
    s32 tile;

    n = range * 2;
    tile = n + RANGE_ICON_TILE + 1;
    UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0);
    UiWindow_SetTilemapEntryFar(window, n + RANGE_ICON_TILE, x + 1, y, 0);
    UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}
