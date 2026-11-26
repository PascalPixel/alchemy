#ifndef ALCHEMY_MUL_FX_H
#define ALCHEMY_MUL_FX_H

/// @file
/// MulFX, the 16.16 fixed-point multiply.

#include "fixed_math.h"
#include "types.h"

#include "system/asm.h"

/// Multiplies 16.16 fixed-point values, returning the middle 32 product bits:
/// IwramMulFX's three instructions written in place.
static __inline__ FIXED MulFX(FIXED left, FIXED right) {
  u32 low;
  s32 high;
  s32 result;

  ASM_SMULL(high, low, left, right);
  high = INTtoFIXED(high);
  ASM_ORR_SHIFTED(result, high, low);
  return result;
}

/// The high word of a times b; by RECIPROCAL_OF(d), a quick a / d.
u32 Math_UnsignedMulHigh(u32 a, u32 b);

/// The reciprocal of `d` as a 32-bit fraction, rounded up: the high word of
/// v times it is v / d, for the small v divided here.
#define RECIPROCAL_OF(d) (0xffffffffU / (d) + 1)

#endif
