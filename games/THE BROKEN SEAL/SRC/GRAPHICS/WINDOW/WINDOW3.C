#include "TBS_EDITION.H"
#include "TYPES.H"

extern u8 *gWindowWork;
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);
u16 *Memory_FillHalfwordsDma(u16 *destination, s32 value, s32 count);

/* Draws a window frame into the text canvas: corners, edges and a blank
   interior. The alternate frame style (byte RENDER_MODE_OFS) uses the flipped corner
   tiles of the second border set. */
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = gWindowWork;
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 row;

    if (width <= 1 || height <= 1 || width > 30 || height > 30)
        return;
    UiWindow_ClearTileAttributesInRect(x, y, width, height);
    if (base[RENDER_MODE_OFS] != 0)
        *cursor++ = 0xf01c;
    else
        *cursor++ = 0xf010;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf011f011, width - 2);
    if (base[RENDER_MODE_OFS] != 0)
        *cursor++ = 0xf41c;
    else
        *cursor++ = 0xf012;
    cursor += 32 - width;
    for (row = 1; row < height - 1; row++) {
        *cursor++ = 0xf016;
        if (width != 2)
            cursor = Memory_FillHalfwordsDma(cursor, 0xf020f020, width - 2);
        *cursor++ = 0xf017;
        cursor += 32 - width;
    }
    if (base[RENDER_MODE_OFS] != 0)
        *cursor++ = 0xf81c;
    else
        *cursor++ = 0xf013;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf014f014, width - 2);
    if (base[RENDER_MODE_OFS] != 0)
        *cursor = 0xfc1c;
    else
        *cursor = 0xf015;
    base[RENDER_DIRTY_OFS] = 1;
}

/* Fills a window's interior with the text canvas tiles, numbered down each
   column from tile 0x127, so glyphs drawn into the canvas show through.
   The full-width form covers the border columns too. */
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width)
{
    u8 *base = gWindowWork;
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 row;
    u32 col;

    if (width <= 1 || height <= 1 || width > 30 || height > 30)
        return;
    cursor += 32;
    if (full_width == 0) {
        for (row = 1; row < height - 1; row++) {
            cursor++;
            for (col = 1; col < width - 1; col++)
                *cursor++ = ((0x127 + row + (col - 1) * (height - 2)) & 0xfff) | 0xf000;
            cursor++;
            cursor += 32 - width;
        }
    } else {
        for (row = 1; row < height - 1; row++) {
            for (col = 0; col < width; col++)
                *cursor++ = ((0x127 + row + col * (height - 2)) & 0xfff) | 0xf000;
            cursor += 32 - width;
        }
    }
    base[RENDER_DIRTY_OFS] = 1;
}
