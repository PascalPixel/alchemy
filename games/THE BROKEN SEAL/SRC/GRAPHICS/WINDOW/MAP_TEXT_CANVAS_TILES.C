#include "TBS_EDITION.H"
#include "TYPES.H"

extern u8 *gWindowWork;

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
