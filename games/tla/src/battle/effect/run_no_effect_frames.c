//--------------------------------------------------------------------------
// run_no_effect_frames.c
// Battle effects: repeat an empty effect.
//--------------------------------------------------------------------------

#include "types.h"
#include "battle.h"

//--------------------------------------------------------------------------
// Call the empty BattleFx_RunNoEffect cnt times without waiting for frames.
// cnt: call count.
//--------------------------------------------------------------------------
void BattleFx_RunNoEffectFrames(s32 cnt) {
  s32 n;

  n = 0;
  if (cnt != 0) {
    do {
      n += 1;
      BattleFx_RunNoEffect();
    } while (n != cnt);
  }
}
