#include "types.h"

void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

void PsynergyMenu_DrawRange(
    s32 window, s32 x, s32 y, s32 range, s32 unused)
{
    s32 n;
    s32 tile;

    (void)unused;

    n = range * 2;
    tile = n + 0xf281;
    /* Keep the empty zero path; GCC emits the reference branch shape. */
    if (!n) {
    }
    UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0);
    UiWindow_SetTilemapEntryFar(window, n + 0xf280, x + 1, y, 0);
    UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}
