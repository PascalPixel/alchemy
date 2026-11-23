#ifndef ALCHEMY_MUL_FX_H
#define ALCHEMY_MUL_FX_H

/// @file
/// MulFX, the 16.16 fixed-point multiply.

#include "fixed_math.h"
#include "types.h"

/// Multiplies 16.16 fixed-point values, returning the middle 32 product bits.
/// Emits IwramMulFX's three instructions (smull, lsl, orr) at the call site.
CAMELOT_ASM("smull with RdLo and RdHi in any two registers, even over its "
            "inputs, where mulsidi3 "
            "pairs them (RdHi = RdLo + 1) apart from the inputs and "
            "smulsi3_highpart discards "
            "RdLo; its orr takes a fresh output reload register beside two "
            "dying inputs and "
            "nothing is scheduled across it, as only asm statements do")
static __inline__ FIXED MulFX(FIXED left, FIXED right) {
  u32 low;
  s32 high;
  s32 result;

  __asm__ volatile("smull %1, %0, %3, %2"
                   : "=r"(high), "=r"(low)
                   : "r"(left), "r"(right));
  high = INTtoFIXED(high);
  __asm__ volatile("orr %0, %1, %2, lsr #16"
                   : "=r"(result)
                   : "r"(high), "r"(low));
  return result;
}

/// The high word of a times b; by RECIPROCAL_OF(d), a quick a / d.
u32 Math_UnsignedMulHigh(u32 a, u32 b);

/// The reciprocal of `d` as a 32-bit fraction, rounded up: the high word of
/// v times it is v / d, for the small v divided here.
#define RECIPROCAL_OF(d) (0xffffffffU / (d) + 1)

#endif
