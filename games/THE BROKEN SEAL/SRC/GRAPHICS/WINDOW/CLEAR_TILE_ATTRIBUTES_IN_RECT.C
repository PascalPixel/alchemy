#include "TYPES.H"
#include "WINDOW.H"
#include "TBS_EDITION.H"


void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = gWindowWork[0];
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 alternate = ((struct UiRenderWork *)base)->alt;
    u32 row;

    for (row = 0; row < height; row++) {
        u32 column;

        for (column = 0; column < width; column++) {
            u32 tile = *cursor++ & 0x3FF;

            if ((tile - 0x80) <= 0x7F ||
                (alternate != 0 &&
                 tile > 0x1FF &&
                 tile <= 0x27F)) {
                u32 index = ((tile & 0xFF) ^ 0x80);
                ((struct UiRenderWork *)base)->tile_attributes[index] &= 0xFC;
            }
        }
        cursor += 32 - width;
    }
}


/* Mark attributes used by the visible 30 by 20 tilemap and clear stale marks. */
void UiWindow_MarkVisibleTileAttributes(void)
{
    u8 *base = gWindowWork[0];
    u16 *cursor = (u16 *)base;
    s32 width = 30;
    u32 alternate = ((struct UiRenderWork *)base)->alt;
    s32 row;
    s32 x;

    for (row = 20; row != 0; row--) {
        for (x = 0; x < width; x++) {
            u32 tile = *cursor++ & 0x3ff;

            if ((tile - 0x80) <= 0x7f ||
                (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                u32 index = ((tile & 0xff) ^ 0x80);

                ((struct UiRenderWork *)base)->tile_attributes[index] |= 2;
            }
        }
    }

    for (row = 0; row < 256; row++, base++) {
        if (((struct UiRenderWork *)base)->tile_attributes[0] == 1)
            ((struct UiRenderWork *)base)->tile_attributes[0] = 0;
    }
}
