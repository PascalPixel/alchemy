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
