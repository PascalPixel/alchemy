#include "types.h"

extern u8 gNumberTextBuffer[];
extern const u8 gText_HexDigits[];
extern u8 gData_03001110;
extern u16 *gData_030011c4;

u32 Math_UnsignedMulHigh(u32 a, u32 b);

void Text_FormatHexToWork(u32 value) {
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

void Text_FormatSignedDecimalToWork(s32 value) {
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

void Func_08014aa8(u32 count) {
  u16 *dst = gData_030011c4;
  u32 i;

  if (gData_03001110 != 0) {
    for (i = 0; i < count; i++)
      *dst++ = 0xf000;
    gData_030011c4 = dst;
  }
}

void Runtime_WriteDebugTextTiles(const u8 *src) {
  if (gData_03001110 != 0) {
    u32 addr = ((u32)&gData_030011c4);
    u32 c = *src;
    u16 *dst = *(u16 **)addr;
    u32 cnt = 0;
    src++;

    if (c != 0) {
      u32 mask;

      addr = 0x06002500;
      /* FAKEMATCH: loads the screen end before it builds the tile mask. */
      __asm__("");
      mask = 0xf000;
      do {
        *dst++ = c | mask;
        if (dst == (u16 *)addr)
          dst = (u16 *)0x06002000;
        cnt++;
        if (cnt > 31)
          break;
        c = *src++;
      } while (c != 0);
      addr = ((u32)&gData_030011c4);
    }
    *(u16 **)addr = dst;
  }
}
