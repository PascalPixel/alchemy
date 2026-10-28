// Draft, ARM C (game ARM route, GCC 2.95.2), behind the Thumb veneer
// Func_080385e0 (bx pc): matches. ☀️'s Glyph_DrawArm
// (games/tbs/src/window/glyph_arm.c) as ⚓️ English has it: the kana-mark
// test of ☀️'s Japanese and English editions, without English's bound on
// codes above 143, and no FAKEMATCH device. The listing is mixed-mode,
// which alchemy-devtools diff cannot compare; checked instead by objcopying the
// --arm draft's .text and the listing's ARM words: the 792 bytes are
// identical (196 instructions, the two pool words gFont_GlyphWidths and
// gFont_Glyphs at the same places). Its entry is 0x080385e4.
#include "types.h"
#include "uiwindow.h"
#include "glyph.h"

#define TEXT_CANVAS_PIXEL_HEIGHT 32
#define SCREEN_TILE_COLUMNS 30
#define UI_CANVAS_VRAM ((void *)0x06002500)
#define UI_WINDOW_OUTLINED_TEXT 0x40

extern const u8 gFont_GlyphWidths[];

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

s32 Func_080385e4(struct Window *window, u32 character, u32 x, u32 y,
                  const u8 *font) {
  const u16 *glyphRows;
  u32 glyphWidth;
  u32 outlinePass;
  u32 advance;
  u32 windowHeight;
  u32 row;
  u32 nextRow;
  u32 column;

  windowHeight = window->height;
  if (character <= 255 || (font != NULL && character - 222 <= 1)) {
    if (font != NULL) {
      const u8 *source;
      u32 color;
      u32 bits;
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
      return fontWidth;
    }
    glyphRows =
        (const u16 *)((const u8 *)gFont_Glyphs + (character - 32) * 26);
    glyphWidth = *glyphRows++;
  } else {
    glyphRows =
        (const u16 *)((const u8 *)gFont_Glyphs + (character - 32) * 32);
    glyphWidth = *glyphRows;
    glyphRows += 4;
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
            Text_SetCanvasPixel(windowHeight, x + column, tileY, rowLine,
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
  return advance;
}
