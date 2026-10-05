/* Near miss: UiText_DrawShadowedGlyph, differ 11. Draws ch from the glyph
   table one pixel right and a row below in colour 1, then in colour 15,
   and returns the glyph's width. Shifting the table index by 5 keeps the
   native's subtraction of 32 (a multiply folds it into the base); the
   native still forms the second destination as dst + 384 in r3 where
   this draft advances dst itself, and loads the table before the mask.
   Precomputed offsets, unsigned x and row, and a separately added offset
   did not change it. Joins PLOT_GLYPH.C on a match. */
#include "TYPES.H"

extern u16 UiText_Glyphs[];

void UiText_PlotGlyphRows(u16 *rows, u8 *dst, s32 color);

s32 UiText_DrawShadowedGlyph(u8 *canvas, s32 ch, s32 x, s32 row)
{
    u16 *glyph = (u16 *)((u8 *)UiText_Glyphs + (((ch & 0xff) - 32) << 5));
    s32 width = *glyph;
    u8 *dst = canvas + (x + row * 192);

    glyph++;
    UiText_PlotGlyphRows(glyph, dst + 3 * 192 + 1, 1);
    UiText_PlotGlyphRows(glyph, dst + 2 * 192, 15);
    return width;
}
