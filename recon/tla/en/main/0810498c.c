// Draft, differ 6 of 28, three pairs in the other order: after each byte
// store the native lets one other instruction pass before the next load
// from the counts (strb; movs r0, #1; ldrb and strb; lsls r3; ldrsb), this
// loads at once. The second pair sits inside the sign extension's shift
// pair, where no barrier fits. The same one-instruction wait after a store
// shows in Func_080dce60 and UiText_DrawResource. A cut-down twin of The
// Broken Seal's Djinn_CheckTurnBalance.
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
