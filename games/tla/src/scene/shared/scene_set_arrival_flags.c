/// @file
/// Scene_SetArrivalFlags, compiled inside each Jupiter Lighthouse scene that
/// calls it and inside Mars Lighthouse room F, at its place in each overlay.
#include "types.h"
#include "flags.h"
#include "flag_call.h"

void Scene_SetArrivalFlags(s32 unused) {
  GameFlag_Set(FLAG_JUPITER_LIGHTHOUSE_LIT);
  if (GameFlag_Get(FLAG_JUPITER_LIGHTHOUSE_ALEX_HEALED_THE_PARTY)) {
    GameFlag_Set(FLAG_NO_ENCOUNTERS_IN_AREA);
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
  }
}
