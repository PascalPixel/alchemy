#include "unit/window/glyph_arm.h"
#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"
#include "edition.h"
#include "graphics.h"
#include "unit/window/window.h"

#define TEXT_CANVAS_PIXEL_HEIGHT 32
#define SCREEN_TILE_ROWS 20
#define SCREEN_TILE_COLUMNS 30

// The text canvas renderer, compiled as ARM code and copied to RAM to run.
//
// A text window draws its text into a canvas of 4bpp tiles in VRAM
// (UI_CANVAS_VRAM, mapped by UiWindow_MapTextCanvasTiles). The canvas tiles
// run down columns: each column of tiles is as tall as the window's interior,
// which is the window height less its two frame rows.

/// Sets one pixel in the column-major text canvas. windowHeight includes the
/// two frame rows; doubledPixelRow is twice the row within a tile.
/// repeatedColor holds the colour index in all four nibbles.
static __inline__ void UiText_SetCanvasPixel(u32 windowHeight, u32 x, u32 tileY,
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
/// kana and numeral font in Japanese, or an 8x8 two-colour font elsewhere. With
/// no font, it comes from the main font (gUiText_Glyphs, and in Japanese
/// gUiText_KanjiGlyphs for codes from 256), drawn in four passes: the first
/// three draw an outline in colour 5 (offset left, right and down when the
/// window has UI_WINDOW_OUTLINED_TEXT) and the last draws the glyph itself in
/// colour 1. In Japanese, kana marks (codes 222 and 223) sit over the previous
/// character and advance by 0.
s32 UiText_DrawGlyphArm(struct UiWindow *window, u32 character, u32 x, u32 y,
                        const u8 *font) {
  const u16 *glyphRows;
  u32 glyphWidth;
  u32 outlinePass;
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  u32 advance;
#endif
  u32 windowHeight;
  u32 row;
  u32 nextRow;
  u32 column;
#if (LANGUAGE == LANGUAGE_ENGLISH)
  u32 glyphIndex;
#endif
#if (LANGUAGE == LANGUAGE_JAPANESE)
  /* FAKEMATCH: these records reserve the native frame's 64-byte unused area.
   * Without them 6 of 193 differ in tbs-ja (sub sp, #36 for #100, and the
   * stacked argument's offset). */
  struct RenderInput unusedFrame[4];
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH)
  // The English font accepts printable codes 32 through 143.
  glyphIndex = character - 32;
#endif
  windowHeight = window->height;
#if (LANGUAGE == LANGUAGE_ENGLISH)
  if (glyphIndex > 111)
    return 1;
#endif

#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  if (character <= 255 || (font != NULL && character - 222 <= 1))
#else
  if (character <= 255 || font != NULL)
#endif
  {
    if (font != NULL) {
      const u8 *source;
      u32 color;
      u32 bits;

#if (LANGUAGE == LANGUAGE_JAPANESE)
      u32 fontWidth;

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
        /* FAKEMATCH: needed to match: the row increment is written in both
         * arms, which puts ADD between CMP and BEQ; written once ahead of the
         * test it is scheduled before CMP (JA differ 2). */
        if (bits == 0)
          nextRow = row + 1;
        else {
          nextRow = row + 1;
          tileY = (y + row) >> 3;
          vram = UI_CANVAS_VRAM;
          rowLine = ((y + row) & 7) << 1;
          do {
            if (bits & 1)
              UiText_SetCanvasPixel(windowHeight, x + column, tileY, rowLine,
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
#if (LANGUAGE == LANGUAGE_ENGLISH)
      fontWidth = gUiText_GlyphWidths[glyphIndex];
#else
      fontWidth = gUiText_GlyphWidths[character - 32];
#endif
      if (y < TEXT_CANVAS_PIXEL_HEIGHT) {
        u8 *vram;

        vram = UI_CANVAS_VRAM;
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
                UiText_SetCanvasPixel(windowHeight, x + column, tileY, rowLine,
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
    // Main-font entries begin with their advance width.
    glyphRows =
        (const u16 *)((const u8 *)gUiText_Glyphs + (character - 32) * 26);
    glyphWidth = *glyphRows++;
  } else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    glyphWidth = 10;
    glyphRows = (const u16 *)(gUiText_KanjiGlyphs + (character - 256) * 24);
#else
    glyphRows =
        (const u16 *)((const u8 *)gUiText_Glyphs + (character - 32) * 32);
    glyphWidth = *glyphRows;
    glyphRows += 4;
#endif
  }
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  if (character - 222 <= 1) {
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
      color = 0x1111;
    else
      color = 0x5555;
    if (window->flags & UI_WINDOW_OUTLINED_TEXT) {
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
      vram = UI_CANVAS_VRAM;
      while (1) {
        u32 tileY;
        u32 rowLine;

        column = 0;
        nextRow = row + 1;
        tileY = (y + row) >> 3;
        rowLine = ((y + row) & 7) << 1;
        while (1) {
          if ((s32)bits < 0)
            UiText_SetCanvasPixel(windowHeight, x + column, tileY, rowLine,
                                  vram, color);
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
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  return advance;
#else
  return glyphWidth;
#endif
}

/// Builds the hardware window's horizontal extent for each of the screen's
/// 160 scanlines from frameTiles in the UI tilemap (32 tiles a row,
/// 30 shown). Each scanline gets (left << 8) | right, the WINxH format, written
/// to the second halfword of its pair in scanlineBounds. A row without a frame
/// gets 0. The rounded corner tiles trim the first or last scanline of a row by
/// one pixel on that side, so the window follows the frame's rounded corners.
void Tile_BuildMetatiles(u16 *scanlineBounds, u16 *frameTiles) {
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

  for (tileRow = 0; tileRow < SCREEN_TILE_ROWS; tileRow++) {
    bottomLeft = 0;
    left = bottomLeft;
    topLeft = bottomLeft;
    bottomRight = bottomLeft;
    right = bottomRight;
    topRight = bottomRight;
    foundLeftEdge = bottomRight;
    tileColumn = bottomRight;
    scanlineBounds++;
    for (; tileColumn < SCREEN_TILE_COLUMNS; tileColumn++) {
      u16 tile = *frameTiles++;

      if (tile == 0)
        continue;
      // The first frame tile establishes the left edge and a one-tile span.
      // A later right frame tile extends that span.
      if (foundLeftEdge == 0) {
        switch (tile) {
        case 0xf010: // top-left corner
          left = tileColumn * 8;
          right = left + 8;
          topLeft = left + 1;
          bottomLeft = left;
          topRight = bottomLeft + 7;
          bottomRight = right;
          foundLeftEdge++;
          break;
        case 0xf013: // bottom-left corner
          left = tileColumn * 8;
          right = left + 8;
          topLeft = left;
          bottomLeft = left + 1;
          topRight = right;
          bottomRight = left + 7;
          foundLeftEdge++;
          break;
        case 0xf007:
        case 0xf016: // left side
#if LANGUAGE != LANGUAGE_JAPANESE
        case 0xf01a:
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
        case 0xf012: // top-right corner
          right = tileColumn * 8 + 8;
          topRight = tileColumn * 8 + 7;
          bottomRight = right;
          break;
        case 0xf015: // bottom-right corner
          right = tileColumn * 8 + 8;
          topRight = right;
          bottomRight = tileColumn * 8 + 7;
          break;
        case 0xf007:
        case 0xf017: // right side
#if LANGUAGE != LANGUAGE_JAPANESE
        case 0xf01b:
#endif
          bottomRight = tileColumn * 8 + 8;
          right = bottomRight;
          topRight = bottomRight;
          break;
        }
      }
    }
    // The tileRow's eight scanlines: its top line, six middle lines and its
    // bottom line.
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
    frameTiles += 2;
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
