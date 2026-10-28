/// @file
/// Text: format a number as a hexadecimal string.

#include "types.h"

/// Writes the lowest count digits of value (up to five), most significant
/// first, to out as uppercase hexadecimal, without a terminating zero.
void Text_FormatHex(u32 value, u32 count, u8 *out) {
  u8 buf[8];
  u32 i;

  if (count > 5)
    count = 5;

  for (i = 0; i != count; i++) {
    u32 d = value & 15;
    if (d <= 9)
      buf[i] = d + '0';
    else
      buf[i] = d + 'A' - 10;
    value >>= 4;
  }

  for (i = count - 1; (s32)i >= 0; i--)
    *out++ = buf[i];
}
