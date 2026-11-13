/// @file
/// Scene_SetArrivalFlags, compiled inside each Jupiter Lighthouse scene that
/// calls it and inside Mars Lighthouse room F, at its place in each overlay.
#include "types.h"
#include "flag_call.h"

void Scene_SetArrivalFlags(s32 unused) {
  GameFlag_Set(0x113);
  if (GameFlag_Get(0xa25)) {
    GameFlag_Set(0x161);
    GameFlag_Set(0x144);
  }
}
