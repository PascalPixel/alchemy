#include "TYPES.H"
#include "TBS_EDITION.H"
#include "RENDER_INPUT.H"

extern u8 *gWindowWork;

/* Value-returning here: the reference loads r0 last of the four arguments,
   as it does when the call itself sets r0. */
s32 UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

/* Blanks the tiles inside a window's frame over a rectangle given in pixels
   from the window's corner, rounded out to whole tiles, releases the glyph
   tiles that stood there, and marks the tilemap dirty. */
void UiWindow_ClearInteriorTiles(const struct RenderInput *window,
    u32 left, u32 top, u32 right, u32 bottom)
{
    u8 *work;
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    u16 *tiles;
    u32 row;

    right += 7;
    bottom += 7;
    right >>= 3;
    bottom >>= 3;
    x = left >> 3;
    work = gWindowWork;
    y = top >> 3;
    x += window->x;
    y += window->y;
    right += window->x;
    bottom += window->y;
    left = x + 1;
    top = y + 1;
    width = right - x;
    height = bottom - y;
    UiWindow_ClearTileAttributesInRect(left, top, width, height);
    tiles = (u16 *)work + top * 32 + left;
    for (row = 0; row < height; row++) {
        for (left = 0; left < width; left++)
            *tiles++ = 0xf020;
        tiles += 32 - width;
    }
    work[RENDER_DIRTY_OFS] = 1;
}
