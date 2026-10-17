#include "unit/effect/effect_bits.h"
#include "types.h"

// ARM immediate operands used when specializing the compositor's width.
// Encode 1 << n as a rotated 8-bit value; operand bits 8 to 11 select the
// rotation rather than forming part of the value itself.

#define ARM_IMMEDIATE_BIT(n)                                                   \
  ((n) < 8 ? 1 << (n) : (12 - ((n) - 8) / 2) << 8 | 1 << ((n) & 1))

#define ARM_IMMEDIATE_POWERS                                                   \
  ARM_IMMEDIATE_BIT(0), ARM_IMMEDIATE_BIT(1), ARM_IMMEDIATE_BIT(2),            \
      ARM_IMMEDIATE_BIT(3), ARM_IMMEDIATE_BIT(4), ARM_IMMEDIATE_BIT(5),        \
      ARM_IMMEDIATE_BIT(6), ARM_IMMEDIATE_BIT(7), ARM_IMMEDIATE_BIT(8),        \
      ARM_IMMEDIATE_BIT(9), ARM_IMMEDIATE_BIT(10), ARM_IMMEDIATE_BIT(11),      \
      ARM_IMMEDIATE_BIT(12), ARM_IMMEDIATE_BIT(13), ARM_IMMEDIATE_BIT(14),     \
      ARM_IMMEDIATE_BIT(15)

// Two identical tables; the first has no references.
u16 gBattleEffect_ColorBitOperands[] = {ARM_IMMEDIATE_POWERS};
u16 gBattleEffect_BitOperands[] = {ARM_IMMEDIATE_POWERS};
