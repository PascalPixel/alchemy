/* 2026-09-30 (Mercury): 49 differing halfwords, 150 of 152 bytes, plain C
   (the permuter's version was 60). The edges are shifted first and the
   window offset added in separate statements: written as one expression,
   CSE cancels window->x out of right - x, where the reference subtracts the
   two offset edges. Left: register roles (the reference copies top to r7
   and left to r5 first and gives x0/y0 r4/r2, the interior origin r5/r7 and
   width/height r6/r4) and the tile pointer formed after the call. */
#include "RENDER_INPUT.H"

extern u8 *gWindowWork;
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

void UiWindow_ClearInteriorTiles(const struct RenderInput *window,
    u32 left, u32 top, u32 right, u32 bottom)
{
    u8 *work = gWindowWork;
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    u16 *tiles;
    u32 row;
    u32 column;

    x = left >> 3;
    y = top >> 3;
    right = (right + 7) >> 3;
    bottom = (bottom + 7) >> 3;
    x += window->x;
    y += window->y;
    right += window->x;
    bottom += window->y;
    width = right - x;
    height = bottom - y;
    x++;
    y++;
    UiWindow_ClearTileAttributesInRect(x, y, width, height);
    tiles = (u16 *)work + y * 32 + x;
    for (row = 0; row < height; row++) {
        for (column = 0; column < width; column++)
            *tiles++ = 0xf020;
        tiles += 32 - width;
    }
    work[0xea3] = 1;
}
