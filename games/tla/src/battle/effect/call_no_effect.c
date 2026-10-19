/// @file
/// Battle effects: repeat an empty effect.

#include "types.h"
#include "battle.h"

/// Call the empty BattleFx_DoNothing cnt times without waiting for frames.
/// cnt: call count.
void BattleFx_CallDoNothing(s32 cnt) {
  s32 n;

  n = 0;
  if (cnt != 0) {
    do {
      n += 1;
      BattleFx_DoNothing();
    } while (n != cnt);
  }
}
