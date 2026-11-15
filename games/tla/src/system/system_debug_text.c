/// @file
/// Number text, the debug text screen and the window graphics.
// Part of system.c.

#include "system_internal.h"
#include "system/mul_q16.h"
#include "system/ram_buffer.h"
#include "resource_ids.h"

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
  s32 q;

  negative = 0;
  if (value < 0) {
    value = -value;
    negative = 1;
  }
  gNumberTextBuffer[11] = 0;
  for (i = 10; value != 0; i--) {
    q = Math_UnsignedMulHigh(value, 0x1999999a);
    value -= q * 10;
    gNumberTextBuffer[i] = value + '0';
    value = q;
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
      *dst++ = 0xf000;
    gDebugTextCursor = dst;
  }
}

/// Prints the string `src` (at most 32 characters) at the debug text cursor,
/// wrapping from the end of the screen to its start, when debug text is on; the
/// cursor moves past it.
void DebugText_Print(const u8 *src) {
  if (gDebugTextEnabled != 0) {
    u32 addr = ((u32)&gDebugTextCursor);
    u32 c = *src;
    u16 *dst = *(u16 **)addr;
    u32 cnt = 0;
    src++;

    if (c != 0) {
      u32 mask;

      addr = 0x06002500;
      mask = 0xf000;
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

// Both format into a right-aligned buffer and write digits characters from its
// end.

/// Hexadecimal, digits places (1-8, or 8 if out of range).
void DebugText_PrintHex(u32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 7U) {
    n = 8;
  }
  String_FormatHex(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextHexEnd) - n));
}

/// Signed decimal, digits places (1-10, or 10 if out of range).
void DebugText_PrintDecimal(s32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 9U) {
    n = 10;
  }
  String_FormatDecimal(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextDecimalEnd) - n));
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
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles),
          (void *)BG_CHAR_BLOCK(0), 0x84000800, REG_DMA3);
  Dma_Set((const void *)gWindow_Palette, (void *)&BG_PLTT_COLOR(15, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Io_Write16(0, &PLTT_BACKDROP);
  {
    volatile u16 *pal;
    s32 color;

    color = 0x4180;
    pal = (volatile u16 *)(BG_PLTT + 244);
    *pal = color;
    color = 0x3960;
    pal++;
    *pal = color;
    color = 0x3140;
    pal++;
    *pal = color;
    color = 0x2920;
    pal++;
    *pal = color;
    color = 0x49a0;
    pal++;
    *pal = color;
    color = 0x51c0;
    pal++;
    *pal = color;
    color = 0x59e0;
    pal++;
    *pal = color;
  }
  Dma_Set((const void *)gSystem_BasicColorPalette, (void *)OBJ_PLTT,
          DMA_COPY_HALFWORDS(224), REG_DMA3);
}

/// Loads the basic sprite palettes (palettes 0 to 13).
void Palette_LoadBasicSpriteColors(void) {
  Dma_Set((const void *)gSystem_BasicColorPalette, (void *)OBJ_PLTT,
          DMA_COPY_HALFWORDS(224), REG_DMA3);
}
