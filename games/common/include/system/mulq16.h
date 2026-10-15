#ifndef ALCHEMY_MULQ16_H
#define ALCHEMY_MULQ16_H

#include "types.h"

// 16.16 固定小数の掛け算。64 ビットの積の中央 32 ビットを返す。
// IwramMulQ16 と同じ 3 命令 (smull, lsl, orr) を呼び出し側に置く。
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
