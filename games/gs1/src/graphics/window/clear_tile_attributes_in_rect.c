#include "types.h"
#include "gs1_edition.h"

extern u8 *Data_03001e8c;

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = Data_03001e8c;
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 alternate = base[RENDER_ALT_OFS];
    u32 row;

    for (row = 0; row < height; row++) {
        u32 column;

        for (column = 0; column < width; column++) {
            u32 tile = *cursor++ & 0x3FF;

            if ((tile - 0x80) <= 0x7F ||
                (alternate != 0 &&
                 tile > 0x1FF &&
                 tile <= 0x27F)) {
                u32 index = ((tile & 0xFF) ^ 0x80) + RENDER_TILE_ATTR_OFS;
                base[index] &= 0xFC;
            }
        }
        cursor += 32 - width;
    }
}
