/// @file
/// Scene_SetArrivalFlags, compiled inside each Jupiter Lighthouse scene that
/// calls it and inside Mars Lighthouse room F, at its place in each overlay.
#include "types.h"
#include "flags.h"
#include "field/flag_call.h"

/// Marks the party as inside Jupiter Lighthouse; once Alex has healed the party
/// there, also ends the area's random encounters and blocks Retreat.
void Scene_SetArrivalFlags(s32 unused) {
  GameFlag_Set(FLAG_IN_JUPITER_LIGHTHOUSE);
  if (GameFlag_Get(FLAG_JUPITER_LIGHTHOUSE_ALEX_HEALED_THE_PARTY)) {
    GameFlag_Set(FLAG_NO_ENCOUNTERS_IN_AREA);
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
  }
}
