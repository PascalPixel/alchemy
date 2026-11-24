/// @file
/// The text canvas renderer, and the window edges of the screen transition,
/// compiled as ARM code and copied to RAM to run.
///
/// A text window draws its text into a canvas of 4bpp tiles in VRAM
/// (TEXT_CANVAS_VRAM, mapped by Window_MapTextCanvasTiles). The canvas tiles
/// run down columns: each column of tiles is as tall as the window's interior,
/// which is the window height less its two border rows. Glyph_Draw and
/// ScanlineWindow_FillSpans copy these routines into RAM for each call, as the
/// tree walker is copied: RamCode_Load takes IWRAM when the copy fits, whose
/// 32-bit bus fetches an ARM instruction in one read, where the cartridge's
/// 16-bit bus takes two.

#include "window/glyph_arm.h"
#include "edition.h"
#include "types.h"
#include "graphics/glyph.h"
#include "graphics/graphics.h"
#include "system/io_reg.h"
#include "window/text_font.h"
#include "window/window.h"

/// The pixel rows text may reach: four tiles, the battle message window's
/// interior; a glyph is cut off below it.
#define TEXT_CANVAS_PIXEL_HEIGHT 32
/// The fonts start at the space.
#define FONT_FIRST_CODE ' '
/// In English, the last code the main font has a glyph for.
#define ENGLISH_FONT_LAST_CODE 143
/// A main-font glyph's bytes, as struct FontGlyph lays one out: its width, then
/// its rows, a halfword each.
#define FONT_GLYPH_BYTES ((1 + FONT_GLYPH_ROWS) * 2)
/// The rows of a main-font glyph Glyph_DrawArm draws. It reads the codes up to
/// TEXT_LAST_FONT_CODE MAIN_FONT_GLYPH_BYTES apart in every edition, the
/// Japanese layout of a width and 12 rows.
#define MAIN_FONT_GLYPH_ROWS 12
#define MAIN_FONT_GLYPH_BYTES ((1 + MAIN_FONT_GLYPH_ROWS) * 2)
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

  // The tile is column x / 8's, row tileY down it; within the tile a pixel
  // row is two halfwords of four pixels, the leftmost in the low nibble.
  pixelHalfword =
      (u16 *)(((u32)canvas + (((x >> 3) * (windowHeight - 2) + tileY) << 5)) |
              ((doubledPixelRow | ((x & 7) >> 2)) << 1));
  pixelMask = 15 << ((x & 3) << 2);
  *pixelHalfword = (*pixelHalfword & ~pixelMask) | (pixelMask & repeatedColor);
}

/// Draws one character into a window's text canvas with its top-left corner at
/// pixel (x, y) of the window's interior, and returns how far the text
/// advances.
///
/// With font set, the character comes from that small font: in Japanese
/// SMALL_FONT_ROWS rows of a bit a pixel, elsewhere the 8x8 characters of the
/// window tiles. With no font, it comes from the main font (gFont_Glyphs, and
/// in Japanese gFont_KanjiGlyphs from KANJI_FIRST_CODE), drawn in four passes:
/// the first three draw an outline in colour 5, offset left, right and down
/// when the window has WINDOW_FLAG_OUTLINED_TEXT, and the last draws the glyph
/// itself in colour 1. In Japanese a kana mark sits over the character before
/// it and advances 0; in English a code past ENGLISH_FONT_LAST_CODE draws
/// nothing and advances 1.
s32 Glyph_DrawArm(struct Window *window, u32 character, u32 x, u32 y,
                  const u8 *font) {
  const u16 *glyphRows;
  u32 glyphWidth;
  u32 outlinePass;
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
  u32 advance;
#endif
  u32 windowHeight;
  u32 row;
  u32 nextRow;
  u32 column;
#if LANGUAGE == LANGUAGE_ENGLISH
  u32 glyphIndex;
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
  /* LEFTOVER(frame): an unused local the tbs-ja ROM's frame reserves and never
   * touches. */
  UNUSED struct GlyphRect unusedStackFrame[4];
#endif

#if LANGUAGE == LANGUAGE_ENGLISH
  glyphIndex = character - FONT_FIRST_CODE;
#endif
  windowHeight = window->height;
#if LANGUAGE == LANGUAGE_ENGLISH
  if (glyphIndex > ENGLISH_FONT_LAST_CODE - FONT_FIRST_CODE)
    return 1;
#endif

  // character - TEXT_MARK_VOICED <= 1, unsigned, is either kana mark.
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
  if (character <= TEXT_LAST_FONT_CODE ||
      (font != NULL && character - TEXT_MARK_VOICED <= 1))
#else
  if (character <= TEXT_LAST_FONT_CODE || font != NULL)
#endif
  {
    if (font != NULL) {
      const u8 *source;
      u32 color;
#ifdef MODERN
      // A current GCC cannot see that the first column loads it.
      u32 bits = 0;
#else
      u32 bits;
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
      u32 fontWidth;

      // The small font: SMALL_FONT_ROWS rows of a byte, bit 0 the leftmost
      // pixel, in colour 1.
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
#if LANGUAGE == LANGUAGE_ENGLISH
      fontWidth = gFont_GlyphWidths[glyphIndex];
#else
      fontWidth = gFont_GlyphWidths[character - FONT_FIRST_CODE];
#endif
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
              if (((x + column) >> 3) < DISPLAY_TILE_WIDTH)
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
    // Main-font entries begin with their advance width, then rows of 16
    // pixels, the leftmost in the top bit.
    glyphRows =
        (const u16 *)((const u8 *)gFont_Glyphs +
                      (character - FONT_FIRST_CODE) * MAIN_FONT_GLYPH_BYTES);
    glyphWidth = *glyphRows++;
  } else {
#if LANGUAGE == LANGUAGE_JAPANESE
    glyphWidth = KANJI_WIDTH;
    glyphRows =
        (const u16 *)(gFont_KanjiGlyphs +
                      (character - KANJI_FIRST_CODE) * KANJI_GLYPH_SIZE);
#else
    // Its glyph's last MAIN_FONT_GLYPH_ROWS rows, after the width and the first
    // three.
    glyphRows = (const u16 *)((const u8 *)gFont_Glyphs +
                              (character - FONT_FIRST_CODE) * FONT_GLYPH_BYTES);
    glyphWidth = *glyphRows;
    glyphRows += 4;
#endif
  }
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
  if (character - TEXT_MARK_VOICED <= 1) {
    x -= 3;
    y -= 2;
    advance = 0;
  } else
    advance = glyphWidth;
#endif

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
        if (nextRow >= MAIN_FONT_GLYPH_ROWS)
          break;
        bits = *nextGlyphRow++;
        bits <<= 16;
        if (y + nextRow >= TEXT_CANVAS_PIXEL_HEIGHT)
          break;
      }
    }
  }
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
  return advance;
#else
  return glyphWidth;
#endif
}

/// Builds the screen transition's window 1 edges for each of the screen's 160
/// scanlines from the window borders in the UI tilemap at `tilemap`
/// (BG_SCREEN_WIDTH tiles a row, DISPLAY_TILE_WIDTH shown), so the windows
/// stay in view while the transition covers the rest: each line takes a pair
/// of halfwords in `scanlineBounds`, the second (left << 8) | right, and a
/// corner's line is a pixel narrower.
void ScanlineWindow_BuildEdges(u16 *scanlineBounds, u16 *tilemap) {
  u32 tileRow;
  u32 tileColumn;
  s32 left;
  s32 right;
  s32 topLeft;
  s32 bottomLeft;
  s32 topRight;
  s32 bottomRight;
  s32 foundLeftEdge;
  s32 horizontalBounds;

  for (tileRow = 0; tileRow < DISPLAY_TILE_HEIGHT; tileRow++) {
    bottomLeft = 0;
    left = bottomLeft;
    topLeft = bottomLeft;
    bottomRight = bottomLeft;
    right = bottomRight;
    topRight = bottomRight;
    foundLeftEdge = bottomRight;
    tileColumn = bottomRight;
    scanlineBounds++;
    for (; tileColumn < DISPLAY_TILE_WIDTH; tileColumn++) {
      u16 tile = *tilemap++;

      if (tile == 0)
        continue;
      // The first border tile establishes the left edge and a one-tile span.
      // A later right border tile extends that span.
      if (foundLeftEdge == 0) {
        switch (tile) {
        case WINDOW_TILE_BORDER_TOP_LEFT:
          left = tileColumn * 8;
          right = left + 8;
          topLeft = left + 1;
          bottomLeft = left;
          topRight = bottomLeft + 7;
          bottomRight = right;
          foundLeftEdge++;
          break;
        case WINDOW_TILE_BORDER_BOTTOM_LEFT:
          left = tileColumn * 8;
          right = left + 8;
          topLeft = left;
          bottomLeft = left + 1;
          topRight = right;
          bottomRight = left + 7;
          foundLeftEdge++;
          break;
        // Outside Japanese the left edge meeting a divider counts as a side
        // too.
        // XXX (no evidence): why tile 7 counts as a side; it is blank in every
        // edition's window tiles and no code writes it into the UI tilemap, so
        // no screen shows it.
        case WINDOW_TILE(7):
        case WINDOW_TILE_BORDER_LEFT:
#if LANGUAGE != LANGUAGE_JAPANESE
        case WINDOW_TILE_BORDER_LEFT_TEE:
#endif
          left = tileColumn * 8;
          right = left + 8;
          bottomLeft = left;
          topLeft = bottomLeft;
          bottomRight = right;
          topRight = bottomRight;
          foundLeftEdge++;
        }
      } else {
        switch (tile) {
        case WINDOW_TILE_BORDER_TOP_RIGHT:
          right = tileColumn * 8 + 8;
          topRight = tileColumn * 8 + 7;
          bottomRight = right;
          break;
        case WINDOW_TILE_BORDER_BOTTOM_RIGHT:
          right = tileColumn * 8 + 8;
          topRight = right;
          bottomRight = tileColumn * 8 + 7;
          break;
        case WINDOW_TILE(7):
        case WINDOW_TILE_BORDER_RIGHT:
#if LANGUAGE != LANGUAGE_JAPANESE
        case WINDOW_TILE_BORDER_RIGHT_TEE:
#endif
          bottomRight = tileColumn * 8 + 8;
          right = bottomRight;
          topRight = bottomRight;
          break;
        }
      }
    }
    // The tile row's eight scanlines, the second halfword of each pair: its
    // top line, six middle lines and its bottom line.
    horizontalBounds = topRight + (topLeft << 8);
    *scanlineBounds++ = horizontalBounds;
    horizontalBounds = right + (left << 8);
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    tilemap += BG_SCREEN_WIDTH - DISPLAY_TILE_WIDTH;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
    horizontalBounds = bottomRight + (bottomLeft << 8);
    scanlineBounds++;
    *scanlineBounds++ = horizontalBounds;
  }
}
