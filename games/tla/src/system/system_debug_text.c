/// @file
/// Number text, the debug text screen and the window graphics.
// Part of system.c.

#include "system_internal.h"
#include "system/mul_fx.h"
#include "system/ram_buffer.h"
#include "resource_ids.h"
#include "window/window.h"

// Debug text: a 32x20-character screen of window font tiles in BG0, written
// from gDebugTextCursor onwards; characters are tile numbers in WINDOW_PALETTE.

/// The debug text screen's bytes: 32 by 20 entries of a halfword.
#define DEBUG_TEXT_SCREEN_BYTES (BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT * 2)
/// The basic sprite palettes, sprite palettes 0 to 13.
#define BASIC_SPRITE_PALETTES 14

/// The number buffer's hexadecimal: eight digits.
#define HEX_DIGITS 8
/// The number buffer's decimal: a sign place and up to ten digits.
#define DECIMAL_DIGITS_MAX 10
/// The most characters DebugText_Print writes of a string.
#define DEBUG_TEXT_MAX_LENGTH 32

/// Writes `value` as HEX_DIGITS hexadecimal digits to the number buffer.
void String_FormatHex(u32 value) {
  u8 *buffer = gNumberTextBuffer;
  const u8 *digits = gString_HexDigits;
  s32 index = HEX_DIGITS - 1;

  do {
    buffer[index] = digits[value & 0xf];
    value >>= 4;
    index--;
  } while (index >= 0);
  gNumberTextBuffer[HEX_DIGITS] = 0;
}

/// Writes `value` to the number buffer as DECIMAL_DIGITS_MAX + 1 characters:
/// spaces, a sign (a space or '-') and its decimal digits, right-aligned.
void String_FormatDecimal(s32 value) {
  s32 negative;
  s32 i;
  s32 quotient;

  negative = 0;
  if (value < 0) {
    value = -value;
    negative = 1;
  }
  gNumberTextBuffer[DECIMAL_DIGITS_MAX + 1] = 0;
  for (i = DECIMAL_DIGITS_MAX; value != 0; i--) {
    quotient = Math_UnsignedMulHigh(value, RECIPROCAL_OF(10));
    value -= quotient * 10;
    gNumberTextBuffer[i] = value + '0';
    value = quotient;
  }
  if (i == DECIMAL_DIGITS_MAX) {
    gNumberTextBuffer[i] = '0';
    i = DECIMAL_DIGITS_MAX - 1;
  }
  if (negative) {
    gNumberTextBuffer[i] = '-';
    i--;
  }
  for (; i >= 0; i--)
    gNumberTextBuffer[i] = ' ';
}

/// Writes `count` blank characters (WINDOW_TILE_BLANK) at the debug text cursor
/// and moves it on, when debug text is on.
void DebugText_PrintBlanks(u32 count) {
  u16 *dst = gDebugTextCursor;
  u32 i;

  if (gDebugTextEnabled != 0) {
    for (i = 0; i < count; i++)
      *dst++ = WINDOW_TILE_BLANK;
    gDebugTextCursor = dst;
  }
}

/// Prints the string `text` (at most DEBUG_TEXT_MAX_LENGTH characters) at the
/// debug text cursor, wrapping from the end of the screen to its start, when
/// debug text is on; the cursor moves past it.
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
        if (cursor == (u16 *)(BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK) +
                              DEBUG_TEXT_SCREEN_BYTES))
          cursor = (u16 *)BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK);
        if (++count > DEBUG_TEXT_MAX_LENGTH - 1)
          break;
        c = *text++;
      } while (c != 0);
    }
    gDebugTextCursor = cursor;
  }
}

/// Prints the last `digits` hexadecimal digits of `value` (1 to HEX_DIGITS;
/// anything else prints all of them). Reached through the far-call table.
void DebugText_PrintHex(u32 value, s32 digits) {
  if (digits < 1 || digits > HEX_DIGITS)
    digits = HEX_DIGITS;
  String_FormatHex(value);
  DebugText_Print(gNumberTextHexEnd - digits);
}

/// Prints the last `digits` characters of `value` in signed decimal (1 to
/// DECIMAL_DIGITS_MAX; anything else prints DECIMAL_DIGITS_MAX). Reached
/// through the far-call table.
void DebugText_PrintDecimal(s32 value, s32 digits) {
  if (digits < 1 || digits > DECIMAL_DIGITS_MAX)
    digits = DECIMAL_DIGITS_MAX;
  String_FormatDecimal(value);
  DebugText_Print(gNumberTextDecimalEnd - digits);
}

/// Blanks the debug text screen, homes the cursor and points BG0 at it.
void DebugText_Clear(void) {
  Dma_Fill32(WINDOW_TILE_PAIR(WINDOW_TILE_BLANK),
             BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK), DEBUG_TEXT_SCREEN_BYTES);
  gDebugTextCursor = (u16 *)BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK);
  Io_Write16(BGCNT_SCREEN_BASE(DEBUG_TEXT_SCREEN_BLOCK), &REG_BG0CNT);
}

/// Loads the window font and border tiles into BG character block 0, the window
/// palette into WINDOW_PALETTE with its gradient colours, and the basic sprite
/// palettes.
void Window_LoadGraphics(void) {
  // The window font and borders: 256 tiles of 16 colours.
  Dma_Set(Resource_GetTableEntry(RESOURCE_WINDOW_TILES),
          (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(256 * TILE_SIZE_4BPP / 4),
          REG_DMA3);
  Dma_Set((const void *)gWindow_Palette,
          (void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  Io_Write16(0, &PLTT_BACKDROP);
  // The window's background colour, three darker shades and three lighter.
  Io_Write16(RGB(0, 12, 16),
             &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR));
  Io_Write16(RGB(0, 11, 14),
             &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 1));
  Io_Write16(RGB(0, 10, 12),
             &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 2));
  Io_Write16(RGB(0, 9, 10),
             &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 3));
  Io_Write16(RGB(0, 13, 18),
             &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 4));
  Io_Write16(RGB(0, 14, 20),
             &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 5));
  Io_Write16(RGB(0, 15, 22),
             &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 6));
  Dma_Set((const void *)gSystem_BasicColorPalette, (void *)OBJ_PLTT,
          DMA_COPY_HALFWORDS(BASIC_SPRITE_PALETTES * PALETTE_COLORS), REG_DMA3);
}

/// Loads the basic sprite palettes.
void Palette_LoadBasicSpriteColors(void) {
  Dma_Set((const void *)gSystem_BasicColorPalette, (void *)OBJ_PLTT,
          DMA_COPY_HALFWORDS(BASIC_SPRITE_PALETTES * PALETTE_COLORS), REG_DMA3);
}

#undef DEBUG_TEXT_SCREEN_BYTES
#undef BASIC_SPRITE_PALETTES
#undef HEX_DIGITS
#undef DECIMAL_DIGITS_MAX
#undef DEBUG_TEXT_MAX_LENGTH
