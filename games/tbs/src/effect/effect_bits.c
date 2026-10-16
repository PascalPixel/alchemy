#include "unit/effect/effect_bits.h"
#include "types.h"

//--------------------------------------------------------------------------
// Operands added to the compositor's width
// Encode 1 << n as an ARM immediate: 4-bit rotation plus 8-bit value.
// Operands the compositing routines add to their widths: 1 << n as ARM
// data-processing immediates.
//--------------------------------------------------------------------------

#define IMM_BIT(n)                                                             \
  ((n) < 8 ? 1 << (n) : (12 - ((n) - 8) / 2) << 8 | 1 << ((n) & 1))

#define IMM_BITS                                                               \
  IMM_BIT(0), IMM_BIT(1), IMM_BIT(2), IMM_BIT(3), IMM_BIT(4), IMM_BIT(5),      \
      IMM_BIT(6), IMM_BIT(7), IMM_BIT(8), IMM_BIT(9), IMM_BIT(10),             \
      IMM_BIT(11), IMM_BIT(12), IMM_BIT(13), IMM_BIT(14), IMM_BIT(15)

// Two identical tables; the first has no references.
const u16 gBattleEffect_ColorBitOperands[] = {IMM_BITS};
const u16 gBattleEffect_BitOperands[] = {IMM_BITS};
