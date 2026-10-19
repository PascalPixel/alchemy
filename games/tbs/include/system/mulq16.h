#ifndef ALCHEMY_MULQ16_H
#define ALCHEMY_MULQ16_H

#include "types.h"

/// Multiplies 16.16 fixed-point values, returning the middle 32 product bits.
/// Emits IwramMulQ16's three instructions (smull, lsl, orr) at the call site.
CAMELOT_ASM("smull with RdLo and RdHi in any two registers, even over its "
            "inputs, where mulsidi3 "
            "pairs them (RdHi = RdLo + 1) apart from the inputs and "
            "smulsi3_highpart discards "
            "RdLo; its orr takes a fresh output reload register beside two "
            "dying inputs and "
            "nothing is scheduled across it, as only asm statements do")
static __inline__ s32 MulQ16(s32 left, s32 right) {
  u32 low;
  s32 high;
  s32 result;

  __asm__ volatile("smull %1, %0, %3, %2"
                   : "=r"(high), "=r"(low)
                   : "r"(left), "r"(right));
  high <<= 16;
  __asm__ volatile("orr %0, %1, %2, lsr #16"
                   : "=r"(result)
                   : "r"(high), "r"(low));
  return result;
}

#endif
