/// @file
/// The digits the number text is written with.

#include "types.h"

/// The digits String_FormatHex writes, one per nibble.
const u8 gString_HexDigits[] = "0123456789ABCDEF";

/// Unused: one tenth as a 32-bit fraction, 2^32 / 10 rounded up, the multiplier
/// String_FormatDecimal divides by ten with; nothing reads this copy.
const u32 gString_OneTenth = 0xffffffffU / 10 + 1;
