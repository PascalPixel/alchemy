#include "gs1_edition.h"
#include "types.h"

extern u8 *Data_03001e8c;

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

#define UiWindow_EraseBorderRect Func_08016178

void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = Data_03001e8c;
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    s32 tile;
    u32 bottom;
    u32 row;

    tile = 240;
    bottom = y + height;
    tile <<= 8;
    if (bottom > 20)
        height = 20 - y;
    if (width <= 1)
        width = 2;
    if (width > 30)
        width = 30;
    if (height <= 1)
        height = 2;
    if (height > 30)
        height = 30;

    UiWindow_ClearTileAttributesInRect(x, y, width, height);

    row = 0;
    if (row < height) {
        do {
            u32 column;

            if (base[RENDER_MENU_STATE_OFS] != 0) {
                if ((u32)(y + row) > 16)
                    tile = 0xF07F;
                else
                    tile = 0xF000;
            }
            column = 0;
            if (column < width) {
                do {
                    *cursor++ = tile;
                    column++;
                } while (column < width);
            }
            row++;
            cursor += 32 - width;
        } while (row < height);
    }
    base[RENDER_DIRTY_OFS] = 1;
}
