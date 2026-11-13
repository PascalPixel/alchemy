/// @file
/// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
/// a number written as hexadecimal text, which nothing calls.
#include "types.h"

/// Unused: writes value as eight hexadecimal digits (gHexDigits) and a
/// terminating null.
UNUSED static void String_WriteU32AsHex(u8 *text, u32 value) {
  s32 i;

  text += 8;
  *text = 0;
  text--;
  for (i = 7; i >= 0; i--) {
    *text = gHexDigits[value & 15];
    value >>= 4;
    text--;
  }
}
