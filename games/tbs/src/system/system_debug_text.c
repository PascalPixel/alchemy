/// @file
/// Number text, the debug text screen and the window graphics.
// Part of system.c.

#include "system_internal.h"

// Debug text: a 32x20-character screen of window font tiles in BG0, written
// from gDebugTextCursor onwards; characters are tile numbers in WINDOW_PALETTE.

/// Writes `value` as eight hexadecimal digits to the number buffer.
void String_FormatHex(u32 value) {
  s32 i;

  for (i = 7; i >= 0; i--) {
    gNumberBuffer[i] = gString_HexDigits[value & 0xf];
    value >>= 4;
  }
  gNumberBuffer[8] = 0;
}

/// Writes `number` to the number buffer as eleven characters: spaces, a sign (a
/// space or '-') and its decimal digits, right-aligned.
void String_FormatDecimal(s32 number) {
  const u32 *placeValues = (const u32 *)gString_PowersOfTen;
  s8 sign = ' ';
  u8 *output = gNumberBuffer;
  s32 places;
  u32 placeValue;
  s32 digit;
#ifdef UBFIX
  u32 magnitude = number;
#else
  s32 magnitude = number;
#endif

  if (number < 0) {
#ifdef UBFIX
    magnitude = 0U - magnitude;
#else
    // UB: negating INT_MIN cannot be represented as a signed s32.
    magnitude = -magnitude;
#endif
    sign = '-';
  }
  // A space for each power of ten above the number, then its sign, then a
  // digit for each power of ten left and the units.
  placeValue = *placeValues++;
  places = 9;
  if (magnitude < placeValue) {
    do {
      places--;
      *output++ = ' ';
      if (places == 0)
        break;
      placeValue = *placeValues++;
    } while (magnitude < placeValue);
  }
  *output++ = sign;
  placeValues--;
  while (places != 0) {
    placeValue = *placeValues++;
    digit = magnitude / placeValue;
    *output++ = digit + '0';
    magnitude -= digit * placeValue;
    places--;
  }
  output[0] = magnitude + '0';
  output[1] = 0;
}

/// Writes `count` blank characters (tile 0 in WINDOW_PALETTE) at the debug text
/// cursor and moves it on.
void DebugText_PrintBlanks(u32 count) {
  u16 *dst = gDebugTextCursor;
  u32 index;

  for (index = 0; index < count; index++)
    *dst++ = WINDOW_TILE(0);
  gDebugTextCursor = dst;
}

#ifdef MODERN
/// Copies size bytes from source to destination, for the modern build.
void *memcpy(void *destination, const void *source, unsigned long size) {
  u8 *to = destination;
  const u8 *from = source;

  while (size-- != 0)
    *to++ = *from++;
  return destination;
}
#endif

/// Prints the string `text` (at most 32 characters) at the debug text cursor,
/// wrapping from the end of the screen to its start, when debug text is on; the
/// cursor moves past it.
void DebugText_Print(const u8 *text) {
  if (gDebugTextEnabled != 0) {
    u16 *cursor = gDebugTextCursor;
    u32 c = *text;
    u32 count = 0;

    text++;
    if (c != 0) {
      u32 palette = WINDOW_PALETTE << 12;

      // The end of the 32 by 20 entry screen wraps to its start.
      do {
        *cursor++ = c | palette;
        if (cursor == (u16 *)(BG_SCREEN_BLOCK(4) + 32 * 20 * 2))
          cursor = (u16 *)BG_SCREEN_BLOCK(4);
        if (++count > 31)
          break;
        c = *text++;
      } while (c != 0);
    }
    gDebugTextCursor = cursor;
  }
}

/// Prints the last `digits` hexadecimal digits of `value` (1 to 8; anything
/// else prints all 8). Reached through the far-call table.
void DebugText_PrintHex(u32 value, s32 digits) {
  if (digits < 1 || digits > 8)
    digits = 8;
  String_FormatHex(value);
  DebugText_Print(gNumberBufferHexEnd - digits);
}

/// Prints the last `digits` characters of `value` in signed decimal (1 to 10;
/// anything else prints 10).
void DebugText_PrintDecimal(s32 value, s32 digits) {
  if (digits < 1 || digits > 10)
    digits = 10;
  String_FormatDecimal(value);
  DebugText_Print(gNumberBufferDecimalEnd - digits);
}

/// Blanks the debug text screen, homes the cursor and points BG0 at it.
void DebugText_Clear(void) {
  Dma_Fill32(0xf000f000, BG_SCREEN_BLOCK(4), 32 * 20 * 2);
  gDebugTextCursor = (u16 *)BG_SCREEN_BLOCK(4);
  Io_Write16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
}

/// Loads the window font and border tiles into BG character block 0, the window
/// palette into WINDOW_PALETTE with its gradient colours, and the basic sprite
/// palettes.
void Window_LoadGraphics(void) {
  // 2048 words: 8 KB, 256 tiles of 4 bits a pixel
  Dma_Set(Resource_GetTableEntry(RESOURCE_WINDOW_TILES), BG_CHAR_BLOCK(0),
          DMA_COPY_WORDS(2048), REG_DMA3);
  Dma_Set(gWindow_Palette, (void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Io_Write16(0, &PLTT_BACKDROP);
  Io_Write16(RGB(0, 12, 16), &BG_PLTT_COLOR(WINDOW_PALETTE, 4));
  Io_Write16(RGB(0, 11, 14), &BG_PLTT_COLOR(WINDOW_PALETTE, 5));
  Io_Write16(RGB(0, 10, 12), &BG_PLTT_COLOR(WINDOW_PALETTE, 6));
  Io_Write16(RGB(0, 9, 10), &BG_PLTT_COLOR(WINDOW_PALETTE, 7));
  Io_Write16(RGB(0, 13, 18), &BG_PLTT_COLOR(WINDOW_PALETTE, 8));
  Io_Write16(RGB(0, 14, 20), &BG_PLTT_COLOR(WINDOW_PALETTE, 9));
  Io_Write16(RGB(0, 15, 22), &BG_PLTT_COLOR(WINDOW_PALETTE, 10));
  Dma_Set(gBasicSpritePalettes, (void *)OBJ_PLTT, DMA_COPY_HALFWORDS(224),
          REG_DMA3);
}

/// Loads the basic sprite palettes (palettes 0 to 13).
void Palette_LoadBasicSpriteColors(void) {
  Dma_Set(gBasicSpritePalettes, (void *)OBJ_PLTT, DMA_COPY_HALFWORDS(224),
          REG_DMA3);
}
