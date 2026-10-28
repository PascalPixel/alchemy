#include "types.h"

u32 Math_UnsignedMulHigh(u32 a, u32 b);
u32 Math_UnsignedMulHighAdd(u32 a, u32 b, s32 c);

/// Writes value as decimal text into buffer (14 bytes) and returns where the
/// text starts. With width 0 the number has no padding and a minus sign when
/// negative; otherwise it is right-aligned in width characters (up to 12) and
/// unsigned. Each digit divides by 10 through the long-multiply helpers:
/// 0x1999999a is 2^32 / 10, rounded up.
// As ☀️'s String_FormatNumber.
u8 *String_FormatNumber(u8 *buffer, s32 input, s32 width) {
  s32 offset;
  s32 value;
  s32 negative;
  s32 spaceCharacter;
  s32 minusCharacter;
  u8 *leading;
  u8 *trim;

  value = input;
  negative = 0;
  if (value < 0) {
    if (width == 0)
      negative = 1;
    value = 0U - value;
  }

  // Twelve digits, then spaces for the leading zeros and the sign in front.
  buffer[0] = ' ';
  for (offset = 12; offset != 0; offset--) {
    buffer[offset] = Math_UnsignedMulHighAdd(value, 0x1999999a, -10) + '0';
    value = Math_UnsignedMulHigh(value, 0x1999999a);
  }

  offset = 0;
  buffer[13] = offset;
  spaceCharacter = ' ';
  offset = 1;
  minusCharacter = '-';
  for (leading = buffer; offset != 13; leading++, offset++) {
    if (leading[1] == '0') {
      if (offset != 12)
        leading[1] = spaceCharacter;
    } else {
      if (negative != 0)
        leading[0] = minusCharacter;
      break;
    }
  }

  if (width == 0) {
    offset = 0;
    if (buffer[0] == ' ') {
      trim = buffer;
      do {
        offset++;
        if (offset == 12)
          break;
        trim++;
      } while (*trim == ' ');
    }
    return buffer + offset;
  }

  if ((u32)width > 12)
    width = 12;
  return buffer + 13 - width;
}
