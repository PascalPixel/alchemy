#include "unit/window/glyph_arm.h"
#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"
#include "edition.h"
#include "graphics.h"
#include "unit/window/window.h"

// The text canvas renderer, compiled as ARM code and copied to RAM to run.
//
// A text window draws its text into a canvas of 4bpp tiles in VRAM
// (UI_CANVAS_VRAM, mapped by UiWindow_MapTextCanvasTiles). The canvas tiles
// run down columns: each column of tiles is as tall as the window's interior,
// which is the window height less its two frame rows.

// Sets one pixel of the text canvas to colour. stride is the window height in
// tiles, x the pixel column, tileY the tile row and rowLine the pixel row
// inside that tile, doubled (the byte offset of its first halfword). color
// holds the colour index in all four nibbles; only the pixel's nibble is kept.
static __inline__ void GlyphDot(u32 stride, u32 x, u32 tileY, u32 rowLine,
                                u8 *vram, u32 color) {
  u16 *p;
  u32 mask;

  p = (u16 *)((u32)vram + (((x >> 3) * (stride - 2) + tileY) << 5) |
              ((rowLine | ((x & 7) >> 2)) << 1));
  mask = 15 << ((x & 3) << 2);
  *p = (*p & ~mask) | (mask & color);
}

// Draws one character into a window's text canvas with its top-left corner at
// pixel (x, y) of the window's interior, and returns how far the text
// advances.
//
// With font set, the character comes from that small font: the 10-row, 1bpp
// kana and numeral font in Japanese, or an 8x8 two-colour font elsewhere. With
// no font, it comes from the main font (gUiText_Glyphs, and in Japanese
// gUiText_KanjiGlyphs for codes from 256), drawn in four passes: the first
// three draw an outline in colour 5 (offset left, right and down when the
// window has UI_WINDOW_OUTLINED_TEXT) and the last draws the glyph itself in
// colour 1. The kana marks (codes 222 and 223) sit over the previous character
// and advance by 0.
s32 UiText_DrawGlyphArm(struct UiWindow *window, u32 character, u32 x, u32 y,
                        const u8 *font) {
  const u16 *rows;
  u32 width;
  u32 pass;
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  u32 result;
#endif
  u32 stride;
  u32 row;
  u32 rownext;
  u32 col;
#if (LANGUAGE == LANGUAGE_ENGLISH)
  u32 index;
#endif
#if (LANGUAGE == LANGUAGE_JAPANESE)
  /* FAKEMATCH: these records reserve the native frame's 64-byte unused area.
   * Without them 6 of 193 differ in tbs-ja (sub sp, #36 for #100, and the
   * stacked argument's offset). */
  struct RenderInput frame[4];
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH)
  index = character - 32;
#endif
  stride = window->height;
#if (LANGUAGE == LANGUAGE_ENGLISH)
  if (index > 111)
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
      while (y + row <= 31) {
        u32 tileY;
        u32 rowLine;
        u8 *vram;

        col = 0;
        /* FAKEMATCH: needed to match: the row increment is written in both
         * arms, which puts ADD between CMP and BEQ; written once ahead of the
         * test it is scheduled before CMP (JA differ 2). */
        if (bits == 0)
          rownext = row + 1;
        else {
          rownext = row + 1;
          tileY = (y + row) >> 3;
          vram = UI_CANVAS_VRAM;
          rowLine = ((y + row) & 7) << 1;
          do {
            if (bits & 1)
              GlyphDot(stride, x + col, tileY, rowLine, vram, color);
            col++;
            bits >>= 1;
          } while (bits != 0);
        }
        row = rownext;
        if (row > 9)
          break;
        bits = *source++;
      }
#else
      u32 color1;
      u32 color3;
      u32 fontWidth;

      source = font + character * 32;
      color1 = 0x1111;
      color3 = 0x3333;
      row = 0;
#if (LANGUAGE == LANGUAGE_ENGLISH)
      fontWidth = gUiText_GlyphWidths[index];
#else
      fontWidth = gUiText_GlyphWidths[character - 32];
#endif
      if (y <= 31) {
        u8 *vram;

        vram = UI_CANVAS_VRAM;
        while (1) {
          u32 tileY;
          u32 rowLine;

          col = 0;
          rownext = row + 1;
          tileY = (y + row) >> 3;
          rowLine = ((y + row) & 7) << 1;
          while (1) {
            if ((col & 1) != 0)
              bits >>= 4;
            else
              bits = *source++;
            if ((bits & 13) == 1) {
              color = color3;
              if ((bits & 15) == 1)
                color = color1;
              if (((x + col) >> 3) <= 29)
                GlyphDot(stride, x + col, tileY, rowLine, vram, color);
            }
            col++;
            if (col > 7)
              break;
          }
          row = rownext;
          if (rownext > 7 || y + rownext > 31)
            break;
        }
      }
#endif
      return fontWidth;
    }
    rows = (const u16 *)((const u8 *)gUiText_Glyphs + (character - 32) * 26);
    width = *rows++;
  } else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    width = 10;
    rows = (const u16 *)(gUiText_KanjiGlyphs + (character - 256) * 24);
#else
    rows = (const u16 *)((const u8 *)gUiText_Glyphs + (character - 32) * 32);
    width = *rows;
    rows += 4;
#endif
  }
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  if (character - 222 <= 1) {
    x -= 3;
    y -= 2;
    result = 0;
  } else
    result = width;
#endif

  for (pass = 0; pass <= 3; pass++) {
    u32 color;
    u32 bits;
    u8 *vram;
    const u16 *next;

    if (pass == 3)
      color = 0x1111;
    else
      color = 0x5555;
    if (window->flags & UI_WINDOW_OUTLINED_TEXT) {
      if (pass <= 1) {
        if (pass == 1)
          x += 2;
        else
          x--;
      } else if (pass == 2) {
        x--;
        y++;
      } else
        y--;
    }
    row = 0;
    bits = rows[0];
    bits <<= 16;
    next = rows + 1;
    if (y <= 31) {
      vram = UI_CANVAS_VRAM;
      while (1) {
        u32 tileY;
        u32 rowLine;

        col = 0;
        rownext = row + 1;
        tileY = (y + row) >> 3;
        rowLine = ((y + row) & 7) << 1;
        while (1) {
          if ((s32)bits < 0)
            GlyphDot(stride, x + col, tileY, rowLine, vram, color);
          bits <<= 1;
          if (bits == 0)
            break;
          col++;
          if (col > 13)
            break;
        }
        row = rownext;
        if (rownext > 11)
          break;
        bits = *next++;
        bits <<= 16;
        if (y + rownext > 31)
          break;
      }
    }
  }
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  return result;
#else
  return width;
#endif
}

// Builds the hardware window's horizontal extent for each of the screen's
// 160 scanlines from the window frames in the UI tilemap src (32 tiles a row,
// 30 shown). Each scanline gets (left << 8) | right, the WINxH format, written
// to the second halfword of its pair in dst. A tile row with no frame tile
// gets 0. The rounded corner tiles trim the first or last scanline of a row by
// one pixel on that side, so the window follows the frame's rounded corners.
void Tile_BuildMetatiles(u16 *dst, u16 *src) {
  u32 row;
  u32 col;
  s32 left;
  s32 right;
  s32 topLeft;
  s32 bottomLeft;
  s32 topRight;
  s32 bottomRight;
  s32 found;
  s32 span;

  for (row = 0; row < 20; row++) {
    bottomLeft = 0;
    left = bottomLeft;
    topLeft = bottomLeft;
    bottomRight = bottomLeft;
    right = bottomRight;
    topRight = bottomRight;
    found = bottomRight;
    col = bottomRight;
    dst++;
    for (; col < 30; col++) {
      u16 tile = *src++;

      if (tile == 0)
        continue;
      // The row's first frame tile sets its left edge, and its right edge one
      // tile on until a right frame tile moves it.
      if (found == 0) {
        switch (tile) {
        case 0xf010: // top-left corner
          left = col * 8;
          right = left + 8;
          topLeft = left + 1;
          bottomLeft = left;
          topRight = bottomLeft + 7;
          bottomRight = right;
          found++;
          break;
        case 0xf013: // bottom-left corner
          left = col * 8;
          right = left + 8;
          topLeft = left;
          bottomLeft = left + 1;
          topRight = right;
          bottomRight = left + 7;
          found++;
          break;
        case 0xf007:
        case 0xf016: // left side
#if (LANGUAGE != LANGUAGE_JAPANESE) || GAME_TLA
        case 0xf01a:
#endif
          left = col * 8;
          right = left + 8;
          bottomLeft = left;
          topLeft = bottomLeft;
          bottomRight = right;
          topRight = bottomRight;
          found++;
        }
      } else {
        switch (tile) {
        case 0xf012: // top-right corner
          right = col * 8 + 8;
          topRight = col * 8 + 7;
          bottomRight = right;
          break;
        case 0xf015: // bottom-right corner
          right = col * 8 + 8;
          topRight = right;
          bottomRight = col * 8 + 7;
          break;
        case 0xf007:
        case 0xf017: // right side
#if (LANGUAGE != LANGUAGE_JAPANESE) || GAME_TLA
        case 0xf01b:
#endif
          bottomRight = col * 8 + 8;
          right = bottomRight;
          topRight = bottomRight;
          break;
        }
      }
    }
    // The row's eight scanlines: its top line, six middle lines and its
    // bottom line.
    span = topRight + (topLeft << 8);
    *dst++ = span;
    span = right + (left << 8);
    dst++;
    *dst++ = span;
    dst++;
    *dst++ = span;
    dst++;
    *dst++ = span;
    dst++;
    src += 2;
    *dst++ = span;
    dst++;
    *dst++ = span;
    dst++;
    *dst++ = span;
    span = bottomRight + (bottomLeft << 8);
    dst++;
    *dst++ = span;
  }
}
