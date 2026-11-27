/* The digits the number text is written with. */

#include "types.h"

/* The digits syStringFormatHex writes, one per nibble. */
const Uint8 StringHexDigits[] = "0123456789ABCDEF";

/* Unused: one tenth as a 32-bit fraction, 2^32 / 10 rounded up, the multiplier
   syStringFormatDecimal divides by ten with; nothing reads this copy. */
const Uint32 StringOneTenth = 0xffffffffU / 10 + 1;
