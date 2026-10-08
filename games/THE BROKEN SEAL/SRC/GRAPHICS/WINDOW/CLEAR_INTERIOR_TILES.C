#include "TYPES.H"
#include "WINDOW.H"
#include "TBS_EDITION.H"
#include "RENDER_INPUT.H"
#include "GRAPHICS.H"

void UiWindow_ClearInteriorTiles(const struct RenderInput *window,
    u32 left, u32 top, u32 right, u32 bottom)
{
    struct UiRenderWork *work;
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
    work = (struct UiRenderWork *)gWindowWork[0];
    y = top >> 3;
    x += window->x;
    y += window->y;
    right += window->x;
    bottom += window->y;
    left = x + 1;
    top = y + 1;
    width = right - x;
    height = bottom - y;
    /* FAKEMATCH: needed to match: the ignored scalar call loads r0 after r1/r2; the void helper declaration loads r0 first. */
    UiWindow_ClearTileAttributesInRect(left, top, width, height);
    tiles = work->tilemap + top * 32 + left;
    for (row = 0; row < height; row++) {
        for (left = 0; left < width; left++)
            *tiles++ = 0xf020;
        tiles += 32 - width;
    }
    work->dirty = 1;
}
