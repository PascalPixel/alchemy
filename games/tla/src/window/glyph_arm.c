/// @file
/// The text canvas renderer, compiled as ARM code and copied to RAM to run,
/// entered in Thumb through Glyph_DrawCode (glyph_arm_entries.s).

#include "module/window/glyph_arm.h"
#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"
#include "edition.h"

/// The pixel rows text may reach: four tiles; a glyph is cut off below it.
#define TEXT_CANVAS_PIXEL_HEIGHT 32
#define SCREEN_TILE_COLUMNS 30
#define TEXT_CANVAS_VRAM ((void *)0x06002500)
/// The window's canvas text gets an outline.
#define WINDOW_FLAG_OUTLINED_TEXT 0x40

extern const u8 gFont_GlyphWidths[];

/// Sets one pixel in the column-major text canvas. windowHeight includes the
/// two border rows; doubledPixelRow is twice the row within a tile.
/// repeatedColor holds the colour index in all four nibbles.
static __inline__ void Text_SetCanvasPixel(u32 windowHeight, u32 x, u32 tileY,
                                           u32 doubledPixelRow, u8 *canvas,
                                           u32 repeatedColor) {
  u16 *pixelHalfword;
  u32 pixelMask;

  pixelHalfword =
      (u16 *)((u32)canvas + (((x >> 3) * (windowHeight - 2) + tileY) << 5) |
              ((doubledPixelRow | ((x & 7) >> 2)) << 1));
  pixelMask = 15 << ((x & 3) << 2);
  *pixelHalfword = (*pixelHalfword & ~pixelMask) | (pixelMask & repeatedColor);
}

/// Draws one character into a window's text canvas with its top-left corner at
/// pixel (x, y) of the window's interior, and returns how far the text
/// advances.
///
/// With font set, the character comes from that small font: the 10-row, 1bpp
/// kana and numeral font in Japanese, or an 8x8 two-colour font elsewhere, 32
/// bytes a character. With no font, it comes from the main font
/// (gFont_Glyphs, and in Japanese gFont_KanjiGlyphs for codes from 256), drawn
/// in four passes: the first three draw an outline in colour 5 (offset left,
/// right and down when the window has WINDOW_FLAG_OUTLINED_TEXT) and the last
/// draws the glyph itself in colour 1. Kana marks (codes 222 and 223) sit over
/// the previous character and advance by 0.
s32 Glyph_DrawArm(struct Window *window, u32 character, u32 x, u32 y,
                  const u8 *font) {
  const u16 *glyphRows;
  u32 glyphWidth;
  u32 outlinePass;
  u32 advance;
  u32 windowHeight;
  u32 row;
  u32 nextRow;
  u32 column;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  // Unused: gives the stack frame its 64 extra bytes.
  UNUSED struct RenderInput unusedStackFrame[4];
#endif

  windowHeight = window->height;
  if (character <= 255 || (font != NULL && character - 222 <= 1)) {
    if (font != NULL) {
      const u8 *source;
      u32 color;
      u32 bits;
#if (LANGUAGE == LANGUAGE_JAPANESE)
      u32 fontWidth;

      // The small font: 10 rows of a byte, bit 0 the leftmost pixel, in
      // colour 1 (0x1111 is the colour in all four nibbles). A character is
      // drawn a pixel in and advances 8; a kana mark goes 2 further left and
      // a row up, and advances 2.
      color = 0x1111;
      fontWidth = 8;
      x++;
      if (character - 222 <= 1) {
        character += 32;
        x -= 2;
        y--;
        fontWidth = 2;
      }
      character -= 32;
      source = font + character * 10;
      row = 0;
      bits = *source++;
      while (y + row < TEXT_CANVAS_PIXEL_HEIGHT) {
        u32 tileY;
        u32 rowLine;
        u8 *vram;

        column = 0;
        // The increment is written in both arms, which keeps it after the
        // compare; written once ahead of the test it is scheduled before it.
        if (bits == 0)
          nextRow = row + 1;
        else {
          nextRow = row + 1;
          tileY = (y + row) >> 3;
          vram = TEXT_CANVAS_VRAM;
          rowLine = ((y + row) & 7) << 1;
          do {
            if (bits & 1)
              Text_SetCanvasPixel(windowHeight, x + column, tileY, rowLine,
                                  vram, color);
            column++;
            bits >>= 1;
          } while (bits != 0);
        }
        row = nextRow;
        if (row > 9)
          break;
        bits = *source++;
      }
#else
      u32 foregroundColor;
      u32 secondaryColor;
      u32 fontWidth;

      source = font + character * 32;
      foregroundColor = 0x1111;
      secondaryColor = 0x3333;
      row = 0;
      fontWidth = gFont_GlyphWidths[character - 32];
      if (y < TEXT_CANVAS_PIXEL_HEIGHT) {
        u8 *vram;

        vram = TEXT_CANVAS_VRAM;
        while (1) {
          u32 tileY;
          u32 rowLine;

          column = 0;
          nextRow = row + 1;
          tileY = (y + row) >> 3;
          rowLine = ((y + row) & 7) << 1;
          while (1) {
            if ((column & 1) != 0)
              bits >>= 4;
            else
              bits = *source++;
            if ((bits & 13) == 1) {
              color = secondaryColor;
              if ((bits & 15) == 1)
                color = foregroundColor;
              if (((x + column) >> 3) < SCREEN_TILE_COLUMNS)
                Text_SetCanvasPixel(windowHeight, x + column, tileY, rowLine,
                                    vram, color);
            }
            column++;
            if (column > 7)
              break;
          }
          row = nextRow;
          if (nextRow > 7 || y + nextRow >= TEXT_CANVAS_PIXEL_HEIGHT)
            break;
        }
      }
#endif
      return fontWidth;
    }
    glyphRows = (const u16 *)((const u8 *)gFont_Glyphs + (character - 32) * 26);
    glyphWidth = *glyphRows++;
  } else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    glyphWidth = 10;
    glyphRows = (const u16 *)(gFont_KanjiGlyphs + (character - 256) * 24);
#else
    glyphRows = (const u16 *)((const u8 *)gFont_Glyphs + (character - 32) * 32);
    glyphWidth = *glyphRows;
    glyphRows += 4;
#endif
  }
  if (character - 222 <= 1) {
    x -= 3;
    y -= 2;
    advance = 0;
  } else
    advance = glyphWidth;

  for (outlinePass = 0; outlinePass <= 3; outlinePass++) {
    u32 color;
    u32 bits;
    u8 *vram;
    const u16 *nextGlyphRow;

    if (outlinePass == 3)
      color = 0x1111;
    else
      color = 0x5555;
    if (window->flags & WINDOW_FLAG_OUTLINED_TEXT) {
      if (outlinePass <= 1) {
        if (outlinePass == 1)
          x += 2;
        else
          x--;
      } else if (outlinePass == 2) {
        x--;
        y++;
      } else
        y--;
    }
    row = 0;
    bits = glyphRows[0];
    bits <<= 16;
    nextGlyphRow = glyphRows + 1;
    if (y < TEXT_CANVAS_PIXEL_HEIGHT) {
      vram = TEXT_CANVAS_VRAM;
      while (1) {
        u32 tileY;
        u32 rowLine;

        column = 0;
        nextRow = row + 1;
        tileY = (y + row) >> 3;
        rowLine = ((y + row) & 7) << 1;
        while (1) {
          if ((s32)bits < 0)
            Text_SetCanvasPixel(windowHeight, x + column, tileY, rowLine, vram,
                                color);
          bits <<= 1;
          if (bits == 0)
            break;
          column++;
          if (column > 13)
            break;
        }
        row = nextRow;
        if (nextRow > 11)
          break;
        bits = *nextGlyphRow++;
        bits <<= 16;
        if (y + nextRow >= TEXT_CANVAS_PIXEL_HEIGHT)
          break;
      }
    }
  }
  return advance;
}
