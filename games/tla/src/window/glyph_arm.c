/// @file
/// The text canvas renderer, compiled as ARM code and copied to RAM to run,
/// entered in Thumb through Glyph_DrawCode (glyph_arm_entries.s).

#include "window/glyph_arm.h"
#include "types.h"
#include "graphics/uiwindow.h"
#include "graphics/glyph.h"
#include "graphics/text_font.h"
#include "edition.h"
#include "graphics/graphics.h"
#include "window/window.h"

/// The pixel rows text may reach: four tiles; a glyph is cut off below it.
#define TEXT_CANVAS_PIXEL_HEIGHT 32
#define SCREEN_TILE_COLUMNS 30
/// The fonts start at the space.
#define FONT_FIRST_CODE ' '
/// The Japanese small font's rows: a byte each.
#define SMALL_FONT_ROWS 10
/// A halfword of four 4-bit pixels, all colour `color`: what
/// Text_SetCanvasPixel masks one pixel out of.
#define PIXELS_4BPP_HALFWORD(color) ((color) * 0x1111)

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
/// draws the glyph itself in colour 1. Kana marks (TEXT_MARK_VOICED and
/// TEXT_MARK_SEMIVOICED) sit over the previous character and advance by 0.
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
  UNUSED u32 unusedStackFrame[16];
#endif

  windowHeight = window->height;
  if (character <= TEXT_LAST_FONT_CODE ||
      (font != NULL && character - TEXT_MARK_VOICED <= 1)) {
    if (font != NULL) {
      const u8 *source;
      u32 color;
      u32 bits;
#if (LANGUAGE == LANGUAGE_JAPANESE)
      u32 fontWidth;

      // The small font: SMALL_FONT_ROWS rows of a byte, bit 0 the leftmost
      // pixel, in colour 1. A character is drawn a pixel in and advances 8; a
      // kana mark goes 2 further left and a row up, and advances 2.
      color = PIXELS_4BPP_HALFWORD(1);
      fontWidth = 8;
      x++;
      if (character - TEXT_MARK_VOICED <= 1) {
        character += FONT_FIRST_CODE;
        x -= 2;
        y--;
        fontWidth = 2;
      }
      character -= FONT_FIRST_CODE;
      source = font + character * SMALL_FONT_ROWS;
      row = 0;
      bits = *source++;
      while (y + row < TEXT_CANVAS_PIXEL_HEIGHT) {
        u32 tileY;
        u32 rowLine;
        u8 *vram;

        column = 0;
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
        if (row >= SMALL_FONT_ROWS)
          break;
        bits = *source++;
      }
#else
      u32 foregroundColor;
      u32 secondaryColor;
      u32 fontWidth;

      // The 8x8 font is window tiles of 4bpp pixels: a pixel of value 1 is
      // drawn in colour 1, one of 3 in colour 3, any other is left (bits & 0xd
      // is 1 only for 1 and 3).
      source = font + character * TILE_SIZE_4BPP;
      foregroundColor = PIXELS_4BPP_HALFWORD(1);
      secondaryColor = PIXELS_4BPP_HALFWORD(3);
      row = 0;
      fontWidth = gFont_GlyphWidths[character - FONT_FIRST_CODE];
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
            if ((bits & 0xd) == 1) {
              color = secondaryColor;
              if ((bits & 0xf) == 1)
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
    // Main-font entries begin with their advance width: 26 bytes, the width and
    // 12 rows of 16 pixels, the leftmost in the top bit.
    glyphRows = (const u16 *)((const u8 *)gFont_Glyphs +
                              (character - FONT_FIRST_CODE) * 26);
    glyphWidth = *glyphRows++;
  } else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    glyphWidth = KANJI_WIDTH;
    glyphRows =
        (const u16 *)(gFont_KanjiGlyphs +
                      (character - KANJI_FIRST_CODE) * KANJI_GLYPH_SIZE);
#else
    glyphRows =
        (const u16 *)((const u8 *)gFont_Glyphs +
                      (character - FONT_FIRST_CODE) * sizeof(struct FontGlyph));
    glyphWidth = *glyphRows;
    glyphRows += 4;
#endif
  }
  if (character - TEXT_MARK_VOICED <= 1) {
    x -= 3;
    y -= 2;
    advance = 0;
  } else
    advance = glyphWidth;

  // Draw the three outline offsets first, then the foreground pixels.
  for (outlinePass = 0; outlinePass <= 3; outlinePass++) {
    u32 color;
    u32 bits;
    u8 *vram;
    const u16 *nextGlyphRow;

    if (outlinePass == 3)
      color = PIXELS_4BPP_HALFWORD(1);
    else
      color = PIXELS_4BPP_HALFWORD(5);
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
