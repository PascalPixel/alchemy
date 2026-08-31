#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define UiWindow_ClearInteriorTiles Func_080164d4

struct UiWindowTileOrigin {
    u8 padding[12];
    u16 x;
    u16 y;
};

void UiWindow_ClearInteriorTiles(const struct UiWindowTileOrigin *window,
                                 u32 left, u32 top, u32 right, u32 bottom)
{
    u8 *base;
    u16 *cursor;
    s32 tile_left;
    s32 tile_top;
    u32 width;
    u32 height;
    u32 row;
    u32 column;

    base = *(u8 **)ADDR_03001E8C;
    tile_left = (left >> 3) + window->x;
    tile_top = (top >> 3) + window->y;
    width = ((right + 7) >> 3) + window->x - tile_left;
    height = ((bottom + 7) >> 3) + window->y - tile_top;
    tile_left++;
    tile_top++;

    UiWindow_ClearTileAttributesInRect(tile_left, tile_top, width, height);

    cursor = (u16 *)(base + ((tile_top << 5) + tile_left) * 2);
    row = 0;
    if (row < height) {
        do {
            column = 0;
            if (column < width) {
                do {
                    column++;
                    *cursor++ = 0xf020;
                } while (column < width);
            }
            row++;
            cursor += 32 - width;
        } while (row < height);
    }
    base[RENDER_DIRTY_OFS] = 1;
}
