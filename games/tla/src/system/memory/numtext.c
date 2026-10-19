#include "types.h"

// The digits NumberText_FormatHex writes, one per nibble.
const u8 gText_HexDigits[] = "0123456789ABCDEF";

// 2^32 / 10 rounded up, the reciprocal NumberText_FormatDecimal multiplies by
// to divide by ten. No code refers to this copy: the formatter keeps its own
// in its literal pool.
const u32 gData_08017ce4 = 0x1999999a;
