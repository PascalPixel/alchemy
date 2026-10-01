#include "TYPES.H"
#include "TBS_EDITION.H"

extern u8 *gWindowWork;

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = gWindowWork;
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

extern u8 *gWindowWork;

/* Mark attributes used by the visible 30 by 20 tilemap and clear stale marks. */
void UiWindow_MarkVisibleTileAttributes(void)
{
    u8 *base = gWindowWork;
    u16 *cursor = (u16 *)base;
    s32 width = 30;
    u32 alternate = base[RENDER_ALT_OFS];
    s32 row;
    s32 x;

    for (row = 20; row != 0; row--) {
        for (x = 0; x < width; x++) {
            u32 tile = *cursor++ & 0x3ff;

            if ((tile - 0x80) <= 0x7f ||
                (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                u32 index = ((tile & 0xff) ^ 0x80) + RENDER_TILE_ATTR_OFS;

                base[index] |= 2;
            }
        }
    }

    for (row = 0; row < 256; row++, base++) {
        if (base[RENDER_TILE_ATTR_OFS] == 1)
            base[RENDER_TILE_ATTR_OFS] = 0;
    }
}
