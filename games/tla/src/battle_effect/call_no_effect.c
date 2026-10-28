/// @file
/// Battle effects: repeat an empty effect.

#include "types.h"
#include "battle.h"

/// Calls the empty BattleEffect_DoNothing cnt times without waiting for frames.
/// cnt: call count.
void BattleEffect_CallDoNothing(s32 cnt) {
  s32 n;

  n = 0;
  if (cnt != 0) {
    do {
      n += 1;
      BattleEffect_DoNothing();
    } while (n != cnt);
  }
}
