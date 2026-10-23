#include "unit/runtime/numtext.h"
#include "types.h"
#include "system.h"

/// The hexadecimal digit characters, indexed by a nibble's value.
const u8 gString_HexDigits[] = "0123456789ABCDEF";

/// Place values for decimal display, 10^9 down to 10, which
/// String_FormatDecimal divides by; the ones digit is what remains.
// clang-format off
const s32 gString_PowersOfTen[9] = {
    1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10,
};
// clang-format on
