// Near miss, differ 6 of 28 (tla-en, 2026-10-31): three pairs in the other
// order: after each byte store the native lets one other instruction pass
// before the next load from the counts (strb; movs r0, #1; ldrb and strb;
// lsls r3; ldrsb), this loads at once. The second pair sits inside the sign
// extension's shift pair, where no barrier fits. The same one-instruction
// wait after a store shows in BattleEffect_AdvanceHueCycle and
// Text_DrawMessage. sched2 gives each store-to-load dependence a cost of 1,
// so the load is ready the next cycle and outranks movs r0, #1 (priority 10
// against 1) and lsls (5 against 3); the native's order needs a cost of 2.
// The compare as to - from > 1, as a returned test, on u8 counts with casts,
// through a difference, and with balanced set before the call all differ
// more (8 to 17), and a 4-minute permuter run (12,493 candidates) found
// nothing better. A cut-down twin of The Broken Seal's
// Djinn_CheckTurnBalance.
#include "types.h"

void Func_081049c8(u8 *counts);

// Test whether moving one from from to to preserves the balance.
s32 Func_0810498c(s32 from, s32 to) {
  s8 counts[16];
  s32 balanced;

  Func_081049c8((u8 *)counts);
  counts[from] -= 1;
  counts[to] += 1;
  balanced = 1;
  if (counts[from] + 2 <= counts[to])
    balanced = 0;
  return balanced;
}
