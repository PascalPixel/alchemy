/// @file
/// Number text, the debug text screen and the window graphics.
// Part of system.c.

#include "system_internal.h"
#include "system/mul_fx.h"
#include "system/ram_buffer.h"
#include "resource_ids.h"
#include "window/window.h"

/// Writes `value` as eight hexadecimal digits to the number buffer.
void String_FormatHex(u32 value) {
  u8 *buffer = gNumberTextBuffer;
  const u8 *digits = gString_HexDigits;
  s32 index = 7;

  do {
    buffer[index] = digits[value & 0xF];
    value >>= 4;
    index--;
  } while (index >= 0);

  {
    u8 *terminator = gNumberTextBuffer;
    terminator[8] = 0;
  }
}

/// Writes `value` to the number buffer as eleven characters: spaces, a sign (a
/// space or '-') and its decimal digits, right-aligned.
void String_FormatDecimal(s32 value) {
  s32 negative;
  s32 i;
  s32 quotient;

  negative = 0;
  if (value < 0) {
    value = -value;
    negative = 1;
  }
  gNumberTextBuffer[11] = 0;
  for (i = 10; value != 0; i--) {
    // The high word of value times 2^32 / 10, rounded up, is value / 10.
    quotient = Math_UnsignedMulHigh(value, 0xffffffffU / 10 + 1);
    value -= quotient * 10;
    gNumberTextBuffer[i] = value + '0';
    value = quotient;
  }
  if (i == 10) {
    gNumberTextBuffer[i] = '0';
    i = 9;
  }
  if (negative) {
    gNumberTextBuffer[i] = '-';
    i--;
  }
  for (; i >= 0; i--)
    gNumberTextBuffer[i] = ' ';
}

/// Writes `count` blank characters (tile 0 in palette 15, the window palette)
/// at the debug text cursor and moves it on.
void DebugText_PrintBlanks(u32 count) {
  u16 *dst = gDebugTextCursor;
  u32 i;

  if (gDebugTextEnabled != 0) {
    for (i = 0; i < count; i++)
      *dst++ = BG_TILE_PALETTE(WINDOW_PALETTE);
    gDebugTextCursor = dst;
  }
}

/// Prints the string `src` (at most 32 characters) at the debug text cursor,
/// wrapping from the end of the screen to its start, when debug text is on; the
/// cursor moves past it.
void DebugText_Print(const u8 *text) {
  if (gDebugTextEnabled != 0) {
    u16 *cursor = gDebugTextCursor;
    u32 c = *text;
    u32 count = 0;

    text++;
    if (c != 0) {
      u32 palette = BG_TILE_PALETTE(WINDOW_PALETTE);

      // The end of the 32 by 20 entry screen wraps to its start.
      do {
        *cursor++ = c | palette;
        if (cursor == (u16 *)(BG_SCREEN_BLOCK(4) +
                              BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT * 2))
          cursor = (u16 *)BG_SCREEN_BLOCK(4);
        if (++count > 31)
          break;
        c = *text++;
      } while (c != 0);
    }
    gDebugTextCursor = cursor;
  }
}

// Both format into a right-aligned buffer and write digits characters from its
// end.

/// Hexadecimal, digits places (1-8, or 8 if out of range).
void DebugText_PrintHex(u32 value, s32 digits) {
  if (digits < 1 || digits > 8)
    digits = 8;
  String_FormatHex(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextHexEnd) - digits));
}

/// Signed decimal, digits places (1-10, or 10 if out of range).
void DebugText_PrintDecimal(s32 value, s32 digits) {
  if (digits < 1 || digits > 10)
    digits = 10;
  String_FormatDecimal(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextDecimalEnd) - digits));
}

/// Blanks the debug text screen, homes the cursor and points BG0 at it.
void DebugText_Clear(void) {
  Dma_Fill32(0xf000f000, BG_SCREEN_BLOCK(4),
             BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT * 2);
  gDebugTextCursor = (u16 *)BG_SCREEN_BLOCK(4);
  Io_Write16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
}

/// Loads the window font and border tiles into BG character block 0, the window
/// palette into palette 15 with its gradient colours, and the basic sprite
/// palettes.
void Window_LoadGraphics(void) {
  Dma_Set(Resource_GetTableEntry(RESOURCE_WINDOW_TILES),
          (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048), REG_DMA3);
  Dma_Set((const void *)gWindow_Palette,
          (void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), DMA_COPY_HALFWORDS(16),
          REG_DMA3);
  Io_Write16(0, &PLTT_BACKDROP);
  Io_Write16(RGB(0, 12, 16), &BG_PLTT_COLOR(WINDOW_PALETTE, 4));
  Io_Write16(RGB(0, 11, 14), &BG_PLTT_COLOR(WINDOW_PALETTE, 5));
  Io_Write16(RGB(0, 10, 12), &BG_PLTT_COLOR(WINDOW_PALETTE, 6));
  Io_Write16(RGB(0, 9, 10), &BG_PLTT_COLOR(WINDOW_PALETTE, 7));
  Io_Write16(RGB(0, 13, 18), &BG_PLTT_COLOR(WINDOW_PALETTE, 8));
  Io_Write16(RGB(0, 14, 20), &BG_PLTT_COLOR(WINDOW_PALETTE, 9));
  Io_Write16(RGB(0, 15, 22), &BG_PLTT_COLOR(WINDOW_PALETTE, 10));
  Dma_Set((const void *)gSystem_BasicColorPalette, (void *)OBJ_PLTT,
          DMA_COPY_HALFWORDS(224), REG_DMA3);
}

/// Loads the basic sprite palettes (palettes 0 to 13).
void Palette_LoadBasicSpriteColors(void) {
  Dma_Set((const void *)gSystem_BasicColorPalette, (void *)OBJ_PLTT,
          DMA_COPY_HALFWORDS(224), REG_DMA3);
}
