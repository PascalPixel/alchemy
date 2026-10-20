/// @file
/// Number text, the debug text screen and the window graphics.
/// Part of runtime.c, the runtime object, compiled only through it.

#include "../runtime_internal.h"

// Debug text: a 32x20-character screen of window font tiles in BG0, written
// from gDebugTextCursor onwards; characters are tile numbers in palette 15.

/// Writes `value` as eight hexadecimal digits to the number buffer.
void NumberText_FormatHex(u32 value) {
  u8 *buffer = gNumberTextBuffer;
  const u8 *digits = gText_HexDigits;
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

/// Writes `number` to the number buffer as eleven characters: spaces, a sign
/// (a space or '-') and its decimal digits, right-aligned.
void NumberText_FormatDecimal(s32 number) {
  u32 *placeValues;
  s8 *output;
  s32 placesLeft;
  u32 placeValue;
  s32 digit;
#ifdef UBFIX
  u32 magnitude;
#else
  s32 magnitude;
#endif
  s8 sign;

  digit = number;
  magnitude = digit;
  placeValues = (u32 *)gText_PowersOfTen;
  sign = ' ';
  output = gNumberTextBuffer;
  if (number < 0) {
#ifdef UBFIX
    magnitude = 0U - magnitude;
#else
    // UB: negating INT_MIN cannot be represented as a signed s32.
    magnitude = -magnitude;
#endif
    sign = '-';
  }
  // Pad with a space for each power of ten above the number
  placeValue = *placeValues++;
  placesLeft = 9;
  if (magnitude < placeValue) {
    do {
      placesLeft -= 1;
      *output++ = ' ';
      if (placesLeft == 0)
        break;
      placeValue = *placeValues++;
    } while (magnitude < placeValue);
  }
  *output++ = sign;
  // Then one digit for each remaining power of ten
  placeValues -= 1;
  if (placesLeft != 0) {
    do {
      placeValue = *placeValues++;
      digit = magnitude / placeValue;
      *output++ = digit + '0';
      magnitude -= digit * placeValue;
      placesLeft -= 1;
    } while (placesLeft != 0);
  }
  output[0] = magnitude + '0';
  output[1] = 0;
}

/// Writes `count` blank characters at the debug text cursor.
void DebugText_PrintBlanks(u32 count) {
  u16 *dst = gDebugTextCursor;
  u32 index;

  for (index = 0; index < count; index++)
    *dst++ = 0xf000;
  gDebugTextCursor = dst;
}

#ifdef MODERN
/// Copies size bytes from source to destination, for the modern build.
// Camelot's compiler copied the task inline; a modern one calls memcpy, and the
// game links no C library.
void *memcpy(void *destination, const void *source, unsigned long size) {
  u8 *to = destination;
  const u8 *from = source;

  while (size-- != 0)
    *to++ = *from++;
  return destination;
}
#endif

/// Prints the string `src` (at most 32 characters) at the debug text cursor,
/// wrapping from the end of the screen to its start, when debug text is on.
void DebugText_Print(const u8 *src) {
  if (gDebugTextEnabled != 0) {
    u32 addr = ((u32)&gDebugTextCursor);
    u32 c = *src;
    u16 *dst = *(u16 **)addr;
    u32 cnt = 0;
    src++;

    if (c != 0) {
      u32 mask = 0xf000;
      addr = (u32)(BG_SCREEN_BLOCK(4) + 0x500);
      do {
        *dst++ = c | mask;
        if (dst == (u16 *)addr)
          dst = (u16 *)BG_SCREEN_BLOCK(4);
        cnt++;
        if (cnt > 31)
          break;
        c = *src++;
      } while (c != 0);
      addr = ((u32)&gDebugTextCursor);
    }
    *(u16 **)addr = dst;
  }
}

/// Prints the last `digits` hexadecimal digits of `value` (1 to 8; anything
/// else prints all 8).
void DebugText_PrintHex(u32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 7U) {
    n = 8;
  }
  NumberText_FormatHex(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextHexEnd) - n));
}

/// Prints the last `digits` characters of `value` in signed decimal (1 to 10;
/// anything else prints 10).
void DebugText_PrintDecimal(s32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 9U) {
    n = 10;
  }
  NumberText_FormatDecimal(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextDecimalEnd) - n));
}

/// Blanks the debug text screen, homes the cursor and points BG0 at it.
void DebugText_Clear(void) {
  Dma_Fill32(0xf000f000, BG_SCREEN_BLOCK(4), 0x500);
  gDebugTextCursor = (u16 *)BG_SCREEN_BLOCK(4);
  Io_Write16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
}

/// Loads the window font and frame tiles into BG character block 0, the window
/// palette into BG palette 15 with its gradient colours, and the basic sprite
/// palettes.
void Ui_LoadWindowGraphics(void) {
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles),
          BG_CHAR_BLOCK(0), DMA_COPY_WORDS(0x800), REG_DMA3);
  Dma_Set(gUi_WindowPalette, (void *)&BG_PLTT_COLOR(15, 0),
          DMA_ENABLE << 16 | 0x10, REG_DMA3);
  Io_Write16(0, &PLTT_BACKDROP);
  Io_Write16(RGB(0, 12, 16), &BG_PLTT_COLOR(15, 4));
  Io_Write16(RGB(0, 11, 14), &BG_PLTT_COLOR(15, 5));
  Io_Write16(RGB(0, 10, 12), &BG_PLTT_COLOR(15, 6));
  Io_Write16(RGB(0, 9, 10), &BG_PLTT_COLOR(15, 7));
  Io_Write16(RGB(0, 13, 18), &BG_PLTT_COLOR(15, 8));
  Io_Write16(RGB(0, 14, 20), &BG_PLTT_COLOR(15, 9));
  Io_Write16(RGB(0, 15, 22), &BG_PLTT_COLOR(15, 10));
  Dma_Set(gSystem_BasicColorPalette, (void *)OBJ_PLTT, DMA_ENABLE << 16 | 0xe0,
          REG_DMA3);
}

/// Loads the basic sprite palettes (palettes 0 to 13).
void Palette_LoadBasicObjColors(void) {
  Dma_Set(gSystem_BasicColorPalette, (void *)OBJ_PLTT, DMA_ENABLE << 16 | 0xe0,
          REG_DMA3);
}
