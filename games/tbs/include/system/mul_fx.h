#ifndef ALCHEMY_MUL_FX_H
#define ALCHEMY_MUL_FX_H

/// @file
/// MulFX, the 16.16 fixed-point multiply.

#include "fixed_math.h"
#include "types.h"

#include "system/asm.h"

/// Multiplies two 16.16 fixed-point values, returning the middle 32 bits of
/// the product: IwramMulFX written out where it is called.
static __inline__ FIXED MulFX(FIXED left, FIXED right) {
  u32 low;
  s32 high;
  s32 result;

  ASM_SMULL(high, low, left, right);
  high = INTtoFIXED(high);
  ASM_ORR_SHIFTED(result, high, low);
  return result;
}

#endif
