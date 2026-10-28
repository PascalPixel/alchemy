#include "types.h"

/// Burns 63 loop iterations and returns; callers pass an argument it ignores.
void BattleEffect_RunBusyWaitDelay(void) {
  s32 i = 63;

loop:
  i--;
  if (i >= 0)
    goto loop;
}
